#include <stdio.h>
#include <cstring>
#include <string> // for std::char_traits
#include <time.h>
#include <vector>
#include <algorithm> // for std::sort


constexpr static const char* const DT_FMT = "%a %Y-%m-%d %H:%M:%S";


template<typename T>
struct Map {
	struct Pair {
		const uint64_t key;
		const T val;
		Pair(const uint64_t _key,  const T _val)
		: key(_key)
		, val(_val)
		{}
	};
	std::vector<Pair> v;
	
	void set(const uint64_t _key,  const T _val){
		v.emplace_back(_key, _val);
	}
	
	T get(const uint64_t _key) const {
		for (const Pair p : this->v){
			if (p.key == _key)
				return p.val;
		}
		return 0;
	}
};


Map<const char*> user_id2name;
Map<time_t> user_id2last_active;

struct Msg {
	const uint64_t other_user_id;
	const uint64_t author;
	const time_t datetime;
	const unsigned int n_msgs;
	const unsigned int n_unread_msgs;
	const char* const content;
	Map<time_t> user_id2last_seen;
	
	Msg(
		  const uint64_t _other_user_id
		, const uint64_t _author
		, const time_t _datetime
		, const unsigned int _n_msgs
		, const unsigned int _n_unread_msgs
		, const char* const _content
	)
	: other_user_id(_other_user_id)
	, author(_author)
	, datetime(_datetime)
	, n_msgs(_n_msgs)
	, n_unread_msgs(_n_unread_msgs)
	, content(_content)
	{}
	
	Msg(const Msg& m)
	: other_user_id(m.other_user_id)
	, author(m.author)
	, datetime(m.datetime)
	, n_msgs(m.n_msgs)
	, n_unread_msgs(m.n_unread_msgs)
	, content(m.content)
	{}
	
	void print() const {
		const time_t _last_seen = this->user_id2last_seen.get(this->other_user_id);
		
		char buf[60];
		struct tm ts;
		
		ts = *localtime(&this->datetime);
		strftime(buf, sizeof(buf), DT_FMT, &ts);
		
		printf("%s\n", user_id2name.get(this->other_user_id));
		printf("%u [%u]\n", this->n_msgs, this->n_unread_msgs);
		printf("%s @ %s %s\n", user_id2name.get(this->author), buf,  (_last_seen >= this->datetime) ? "(Viewed by comrade)" : "");
		printf("%s\n", this->content);
		
		const time_t _t = user_id2last_active.get(this->other_user_id);
		ts = *localtime(&_t);
		strftime(buf, sizeof(buf), DT_FMT, &ts);
		printf("Last active		%s\n",  buf);
		
		ts = *localtime(&_last_seen);
		strftime(buf, sizeof(buf), DT_FMT, &ts);
		printf("Last aviewed		%s\n",  buf);
		
		printf("\n");
	};
	
	bool operator=(const Msg& m){
		return (this->datetime == m.datetime);
	};
	
	bool operator<(const Msg& m){
		return (this->datetime < m.datetime);
	};
};

static
std::vector<Msg> msgs;


template<typename T,  typename Char>
T str2n(Char*& str,  const Char c = '"',  const Char d = 0){
	T n = 0;
	while((*str != c)  &&  (*str != d)){
		n *= 10;
		n += *str - '0';
		++str;
	}
	return n;
}



template<typename Char>
Char* search_for(const char* const compare_to,  Char* str){
	const char* cmp = compare_to;
	while(*str != 0){
		if (*str == *cmp){
			++cmp;
			if (*cmp == 0){
				break;
			}
		} else {
			cmp = compare_to;
		}
		++str;
	}
	
	if (*cmp != 0){
		// Match was not found
		return nullptr;
	}
	
	return str;
};


bool startswith(const char* cmp,  const char* str){
	while(*str == *cmp  &&  *cmp != 0){
		++str;
		++cmp;
	}
	
	return (*cmp == 0);
}


void populate_id2last_active(const char* str){
	str = search_for(",\"lastActiveTimes\":{", str);
	do {
		str += 2;
		
		const uint64_t user_id = str2n<uint64_t, const char>(str, '"');
		
		str += 2; // Skip ':'
		const time_t t = str2n<time_t, const char>(str, ',', '}');
		
		user_id2last_active.set(user_id, t);
	} while(*str != '}');
}


void populate_id2name(char*& str){
	const char* s;
	while((s = search_for("\"type\":\"friend\",\"is_friend\":true,\"mThumbSrcSmall\":", str))){
		str = const_cast<char*>(s);
		if (str == nullptr)
			break;
		
		while(*(--str) != '{');
		
		str += std::char_traits<char>::length("{\"id\":\"");
		const uint64_t user_id = str2n<uint64_t, char>(str, '"');
		
		str += std::char_traits<char>::length("\",\"name\":\"");
		char* const user_name = str;
		while(*str != '"')
			++str;
		*str = 0;
		
		user_id2name.set(user_id, user_name);
		
		while(*str != '}')
			// for next iteration
			++str;
	}
}


char* get_all_matches(char* str){
	constexpr
	static
	const char* const search_for_1 = "\"},\"name\":null,\"last_message\":{\"nodes\":[{\"snippet\":\"";
	
	str = search_for(search_for_1, str);
	if (str == nullptr)
		return nullptr;
	
	const char* str_other_user_id = str - std::char_traits<char>::length(search_for_1);
	while(*str_other_user_id != '"')
		--str_other_user_id;
	++str_other_user_id;
	const uint64_t other_user_id = str2n<uint64_t, const char>(str_other_user_id, '"');
	
	++str;
	const char* const snippet = str;
	
	while(*str != '"')
		++str;
	*str = 0;
	
	str += std::char_traits<char>::length("\",\"message_sender\":{\"messaging_actor\":{\"id\":\"");
	
	const uint64_t user_id = str2n<uint64_t, char>(str, '"');
	
	if (startswith("You are now connected on Messenger.", snippet))
		return str + 1;
	if (startswith("Say hello to ", snippet))
		return str + 1;
	
	str += std::char_traits<char>::length("\"}},\"timestamp_precise\":\"");
	
	const time_t time = str2n<time_t, char>(str) / 1000;
	
	str = search_for(",\"unread_count\":", str) + 1;
	const unsigned int n_unread_msgs = str2n<unsigned int, char>(str, ',');
	str += std::char_traits<char>::length(",\"messages_count\":");
	const unsigned int n_msgs = str2n<unsigned int, char>(str, ',');
	
	msgs.emplace_back(other_user_id,  user_id,  time,  n_msgs,  n_unread_msgs,  snippet);
	Msg* const m = &msgs[msgs.size() - 1];
	
	str = search_for(",\"read_receipts\":{\"nodes\":[", str);
	// Array of {"watermark":"TIMESTAMP","action":"TIMESTAMP","actor":{"id":"USER_ID"}}
	while(true){
		str += std::char_traits<char>::length("{\"watermark\":\"") + 1;
		const time_t _timestamp_of_last_viewed_msg = str2n<time_t, char>(str) / 1000;
		
		str += std::char_traits<char>::length(",\"action\":\"") + 1;
		const time_t _time = str2n<time_t, char>(str) / 1000;
		
		while(*(++str) != '{');
		str += std::char_traits<char>::length("\"id\":\"") + 1;
		
		const uint64_t _user_id = str2n<uint64_t, char>(str);
		
		m->user_id2last_seen.set(_user_id, _time);
		
		while(*str != '}')
			++str;
		
		++str; // Skip second '}'
		
		if(*(++str) != ',')
			break;
	}
	
	m->print();
	
	return str + 1;
}


int main(const int argc,  const char* const* argv){
	/*
	 * Use the browser developer console to "Copy as cURL" the request to "https://www.facebook.com/messages" while logged in.
	 * Simply pipe this cURL command to this fbmsg program
	 */
	constexpr static size_t buf_sz = 4 * 1024 * 1024; // 4 MiB
	char* buf = (char*)malloc(buf_sz);
	if (buf == nullptr)
		return 1;
	
	for (auto i = 1;  i < argc;  ++i){
		const char* arg = argv[i];
		
		// USER_ID=USER_NAME
		const uint64_t user_id = str2n<uint64_t, const char>(arg, '=');
		const char* const user_name = arg + 1;
		user_id2name.set(user_id, user_name);
	}
	
	fread(buf, 1, buf_sz, stdin);
	populate_id2last_active(buf);
	populate_id2name(buf);
	while((buf = get_all_matches(buf)));
	
	return 0;
}
