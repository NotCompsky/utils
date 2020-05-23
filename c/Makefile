default: ~/bin/countdir ~/bin/lsdir ~/bin/ascii ~/bin/bits2i ~/bin/i2bits ~/bin/f2c ~/bin/f2m ~/bin/pound2kg ~/bin/camel2snake ~/bin/get_yt_ids ~/bin/capitalise ~/bin/dblqt ~/bin/duration-of ~/bin/echo-if-contains ~/bin/countdir-r ~/bin/ip2i ~/bin/extract-audio

~/bin/countdir:
	cc countdir.c -o ~/bin/countdir -O3
~/bin/countdir-r:
	cc countdir.c -o ~/bin/countdir-r -O3 -DRECURSIVE

~/bin/lsdir:
	cc lsdir.c -o ~/bin/lsdir -O3

~/bin/ascii:
	cc ascii.c -o ~/bin/ascii -O3

~/bin/bits2i:
	cc bits2i.c -o ~/bin/bits2i -O3

~/bin/i2bits:
	cc i2bits.c -o ~/bin/i2bits -O3

~/bin/f2c:
	cc x2y.c -o ~/bin/f2c -O3 -DOFFSET=-32 -DSCALE=/1.8

~/bin/f2m:
	cc x2y.c -o ~/bin/f2m -O3 -DOFFSET= -DSCALE='*0.3048' -DALLOW_TWELVES

~/bin/pound2kg:
	cc x2y.c -o ~/bin/pound2kg -O3 -DOFFSET= -DSCALE='*0.45359237'

~/bin/camel2snake:
	cc camel2snake.c -o ~/bin/camel2snake -O3

~/bin/get_yt_ids:
	cc get_yt_ids.c -o ~/bin/get_yt_ids -O3

~/bin/capitalise:
	cc capitalise.c -o ~/bin/capitalise -O3

~/bin/dblqt:
	cc dblqt.c -o ~/bin/dblqt -O3

~/bin/duration-of:
	cc duration-of.c -O3 -o ~/bin/duration-of -lavcodec -lavformat

~/bin/extract-audio:
	cc extract-audio.c -O3 -o ~/bin/extract-audio -lavformat -lavcodec -lavutil

~/bin/echo-if-contains:
	cc echo-if-contains.c -O3 -o ~/bin/echo-if-contains

~/bin/ip2i:
	cc ip2i.c -O3 -o ~/bin/ip2i
