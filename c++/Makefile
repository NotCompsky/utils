default: ~/bin/diff-bin ~/bin/boost-regex ~/bin/is-small-prime ~/bin/tcpdump2db ~/bin/fbmsg ~/bin/compact-regex ~/bin/html-lexer

~/bin/boost-regex:
	c++ boost-regex.cpp -o ~/bin/boost-regex -O3 -I"${HOME}/repos/compsky/libcompsky/include" -lboost_regex

~/bin/is-small-prime:
	c++ is-small-prime.cpp -o ~/bin/is-small-prime -O3

~/bin/diff-bin:
	c++ diff-bin.cpp -o ~/bin/diff-bin -O3

~/bin/tcpdump2db:
	c++ tcpdump2db.cpp -O3 -o ~/bin/tcpdump2db -L${HOME}/repos/compsky/libcompsky/include

~/bin/fbmsg:
	c++ fbmsg.cpp -Os -o ~/bin/fbmsg

~/bin/compact-regex:
	#g++ compact-regex.cpp -o ~/bin/compact-regex -O3 -I"${HOME}/repos/compsky/libcompsky/include" -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -lqt5_widgets
	
	c++  compact-regex.cpp  -DNDEBUG  -DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT -DUSE_BOOST_REGEX  -I/home/vangelic/repos/compsky/rscraper/3rdparty/include -I/home/vangelic/repos/compsky/rscraper/shared-src -I/home/vangelic/repos/compsky/libcompsky/build-dbg/../include -I/home/vangelic/repos/compsky/rscraper -I/home/vangelic/repos/compsky/rscraper/scraper/../utils/src  -Wno-unused-parameter  -O3  -std=gnu++17  -o ~/bin/compact-regex -c    -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets

~/bin/html-lexer222:
	c++ html_lexer.cpp -Os -o ~/bin/html-lexer -llexbor-html -llexbor-dom
