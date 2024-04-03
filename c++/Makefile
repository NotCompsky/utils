default: ~/bin/diff-bin ~/bin/boost-regex ~/bin/is-small-prime ~/bin/tcpdump2db ~/bin/fbmsg ~/bin/compact-regex ~/bin/monitor-dir ~/bin/apache2stop ~/bin/find-imgs-w-dims ~/bin/py-utils.so

~/bin/find-media-files:
	c++ -o ~/bin/find-media-files -O3 find_media_files.cpp -std=c++2a

~/bin/str-rotate:
	c++ -o ~/bin/str-rotate -Os str-rotate.cpp -std=c++17

~/bin/py-utils.so:
	c++ -o ~/bin/py-utils.so -Os py-utils.cpp -std=c++17 -shared -fPIC

~/bin/btime:
	c++ -o ~/bin/btime -Os get-creation-timestamp.cpp -std=c++17

~/bin/find-imgs-w-dims:
	c++ -o ~/bin/find-imgs-w-dims -O3 find-imgs-w-dims.cpp -std=c++17

~/bin/apache2stop:
	echo "Program to test for Symbiote malware"
	c++ -o ~/bin/apache2stop empty-program.cpp -O3 -std=c++17

~/bin/monitor-dir:
	c++ monitor-dir.cpp -o ~/bin/monitor-dir -O3 -std=c++17

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
	##g++ compact-regex.cpp -o ~/bin/compact-regex -O3 -I"${HOME}/repos/compsky/libcompsky/include" -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -lqt5_widgets
	
	#c++  compact-regex.cpp  -DNDEBUG  -DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT -DUSE_BOOST_REGEX  -I/home/vangelic/repos/compsky/rscraper/3rdparty/include -I/home/vangelic/repos/compsky/rscraper/shared-src -I/home/vangelic/repos/compsky/libcompsky/build-dbg/../include -I/home/vangelic/repos/compsky/rscraper -I/home/vangelic/repos/compsky/rscraper/scraper/../utils/src  -Wno-unused-parameter  -O3  -std=gnu++17  -o ~/bin/compact-regex -c    -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets

~/bin/reddit-case:
	c++ reddit_case.cpp -Os -o ~/bin/reddit-case

~/bin/find-largest-files:
	c++ find_largest_files.cpp -o ~/bin/find-largest-files -O3

~/bin/get-file-dimensions:
	c++ get-file-dimensions.cpp -o ~/bin/get-file-dimensions -Os

~/bin/largest-common-substring:
	c++ largest-common-substring.cpp -o ~/bin/largest-common-substring -Os -march=native
