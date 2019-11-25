#include "name_dialog.hpp"
#include <QApplication>
#include <QCompleter>
#include <QStringList>


int main(const int argc,  const char** argv){
	QStringList slist;
	int dummy_argc = 0;
	char** dummy_argv = {};
	QApplication app(dummy_argc, dummy_argv);
	NameDialog dialog(argv[1], argv[2]);
	QCompleter* completer;
	if (argc > 3){
		for (auto i = 3;  i < argc;  ++i)
			slist << argv[i];
		completer = new QCompleter(slist);
		dialog.name_edit->setCompleter(completer);
	}
	const int rc = dialog.exec();
	if (rc != QDialog::Accepted)
		return 1;
	const QString q = dialog.name_edit->text();
	const QByteArray b = q.toLocal8Bit();
	const char* const s = b.data();
	printf("%s\n", s);
	return 0;
}
