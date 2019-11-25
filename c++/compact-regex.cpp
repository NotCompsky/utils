#include <compsky/regex/named_groups.hpp>
#include "/home/vangelic/repos/compsky/rscraper/hub/src/msgbox.hpp"
#include <QString>
#include <QStringRef>


char* BUF;
size_t BUF_SZ;


void ensure_buf_sized(const size_t buf_sz){
	if (buf_sz < BUF_SZ)
		return;
	// Ensure there is sufficient space to run the compsky::mysql::exec commands below
	BUF_SZ = 2*buf_sz;
	void* dummy = malloc(BUF_SZ);
	if (unlikely(dummy == nullptr))
		exit(4096);
	free(BUF);
	BUF = (char*)dummy;
}


bool to_final_format(const bool optimise,  const QString& q,  QString& buf,  int i,  int j,  int last_optimised_group_indx,  int var_depth){ // Use seperate buffer to avoid overwriting text_editor contents
	// WARNING: Does not currently support special encodings, i.e. non-ASCII characters are likely to be mangled.
	// TODO: Add utf8 support.
	
	int group_start = 0;
	int group_start_offset;
	static std::vector<QStringRef> var_names;
	static std::vector<QStringRef> var_values;
	static std::vector<int> var_starts;
	for(;  i < q.size();  ){
		QChar c = q.at(i);
		if (c == QChar('\\')){
			// Recognised escapes: \\, \n, \r, \t, \v
			// All others simply become the literal value of the next character
			if (++i == q.size())
				break;
			QChar ch = q.at(i);
			if	  (ch == QChar('n'))  ch = QChar('\n');
			else if (ch == QChar('r'))  ch = QChar('\r');
			else if (ch == QChar('t'))  ch = QChar('\t');
			else if (ch == QChar('v'))  ch = QChar('\v'); // Vertical tab
			else if (ch == QChar('\\'));
			else if (ch == QChar('\n'));
			else if (ch == QChar('\t'));
			else if (ch == QChar(' '));
			else if (ch == QChar('{'));
			else if (ch == QChar('}'));
			else if (ch == QChar('('));
			else if (ch == QChar(')'));
			else {
				constexpr static const int ctx = 10;
				MsgBox* msgbox = new MsgBox(
					0,  
					"Unrecognised escape sequence: \\" + QString(ch) + " at line " + QString::number(get_line_n(q, i)),
					QStringRef(&q,  (i >= ctx) ? i - ctx : 0,  (i + ctx < q.size()) ? i + ctx : q.size() - 1).toString()
				);
				msgbox->exec();
				delete msgbox;
				goto goto_RE_tff_cleanup;
			}
			
			buf[j++] = ch;
			++i;
			continue;
		}
		if (c == QChar('$')  &&  q.at(i+1) == QChar('{')){
			i += 2; // Skip ${
			const int substitute_var_name_start = i;
			while(q.at(i++) != QChar('}'));
			const QStringRef substitute_var_name(&q,  substitute_var_name_start,  i - 1 /* backtrack } */ - substitute_var_name_start);
			QStringRef var = nullptr;
			for (size_t k = var_names.size();  k != 0;  ){
				--k;
				if (var_names[k] != substitute_var_name)
					continue;
				var = var_values[k];
			}
			if (var == nullptr){
				// Variable of the given name was not declared before
				QString msg = "Previously defined variables:";
				for (size_t k = var_names.size();  k != 0;  ){
					msg += "\n";
					msg += var_names[--k];
				}
				MsgBox* msgbox = new MsgBox(
					0,  
					"Undeclared variable: " + substitute_var_name + "\nAt line " + QString::number(get_line_n(q, i)),
					msg
				);
				msgbox->exec();
				delete msgbox;
				goto goto_RE_tff_cleanup;
			}
			buf += var;
			j += var.size();
			ensure_buf_sized(j);
			continue;
		}
		if (c == QChar('{')){
			if (q.at(i+1) == QChar('?')  &&  q.at(i+2) == QChar('P')  && q.at(i+3) == QChar('<')){
				i += 4;
				const int var_name_start = i;
				while(q.at(i++) != QChar('>'));
				var_names.emplace_back(&q,  var_name_start,  i - 1 /* Backtrack > */ - var_name_start);
				var_values.push_back(nullptr);
				var_starts.push_back(j);
				continue;
			}
		}
		if (c == QChar('}')){
			size_t k = var_values.size();
			while(true){
				if (k == 0){
					QMessageBox::warning(0,  "Unacceptable Syntax",  QString("Line %1: Encountered unescaped '}' without preceding '{?P<VARNAME>' or '${VARNAME'").arg(get_line_n(q, i)));
					goto goto_RE_tff_cleanup;
				}
				if (var_values[--k] == nullptr)
					break;
			}
			var_values[k] = QStringRef(&buf,  var_starts[k],  j - var_starts[k]);
			++i;
			continue;
		}
		if (c == QChar('\n')){
			++i;
			while((i < q.size())  &&  (q.at(i) == QChar(' ')  ||  q.at(i) == QChar('\t')))
				++i;
			continue;
		}
		if (c == QChar('#')  &&  (i == 0  ||  q.at(i-1) == QChar(' ')  ||  q.at(i-1) == QChar('\t')  ||  q.at(i-1) == QChar('\n'))){
			++i;
			do {
				// Remove all preceding unescaped whitespace
				--j;
			} while (
				(j >= 0  && buf.at(j) == QChar(' '))  ||
				(j >= 1  &&  buf.at(j) == QChar('\t')  &&  buf.at(j-1) != QChar('\\'))
			);
			++j;
			while((i < q.size())  &&  (q.at(i) != QChar('\n')))
				++i;
			continue;
		}
		if (optimise){
			if (c == QChar('(')  &&  j != last_optimised_group_indx){ // Minimum offset for non-trivial group: (ab|c)
				group_start_offset = 0;
				if (q.at(i+1) == QChar('?')  &&  q.at(i+2) == QChar(':'))
					group_start_offset += 3;
				else if (q.at(i+1) == QChar('?')  &&  q.at(i+2) == QChar('P')  &&  q.at(i+3) == QChar('<')){
					group_start_offset += 4;
					while(q.at(i+group_start_offset) != QChar('>')){
						++group_start_offset;
					}
					++group_start_offset;
				} else group_start_offset += 1;
				group_start = j;
			} else if (c == QChar(')')  &&  group_start != 0){
				const int group_start_actual = group_start + group_start_offset;
				const QStringRef group_text(&buf,  group_start_actual,  j - group_start_actual);
				QString group_replacement;
				const QString group_str = group_text.toString();
				QString s = group_str;
				optimise_regex(s, group_replacement);
				buf.replace(group_start_actual,  j - group_start_actual,  group_replacement);
				return to_final_format(optimise,  q,  buf,  i,  group_start_actual + group_replacement.size(),  group_start,  var_depth);
			}
		}
		
		buf[j++] = c;
		
		++i;
	}
	buf.resize(j); // Strips excess space, as buf is guaranteed to be smaller than q (until variable substitution is implemented)
	
	var_names.clear();
	var_values.clear();
	var_starts.clear();
	
	return true;

	goto_RE_tff_cleanup:
	var_names.clear();
	var_values.clear();
	var_starts.clear();
	return false;
}


int main(const int argc,  const char** const argv){
	
	return 0;
}
