#!/usr/bin/env bash


dl_imgs_lib_min="$(cat dl_imgs_lib.js | sed -E "s~[ \t]//.*\$~~g" | sed 's/^[ \t]\+//g' | tr -d '\n' | sed 's/\\/\\\\/g' | sed 's/&/\\&/g')"
for f in $(rgrep -l '^// @require file:///home/vangelic/repos/compsky/utils__b/javascript/dl_imgs_lib.js'); do
	sed -i -E "s~^const dl_imgs_lib = .*\$~$dl_imgs_lib_min;~" "$f"
done
