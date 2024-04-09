#include <dirent.h> // for opendir, readdir
#include <string.h> // for strlen, memcpy
#include <stdlib.h> // for malloc, realloc
#include <unistd.h> // for write
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <cstdint>
#include <cstdio>

constexpr uint32_t audio_ext_multiplier = 2152159432;
constexpr uint32_t audio_ext_shiftby = 26;
constexpr uint32_t audio_ext_pathids[] = {863005998, 1447122734, 1986098990, 1128349998, 1667326254, 863005998, 860900654, 863005998, 863005998, 1128352838, 1195855662, 1734831918, 1667329126, 1398100047, 863005998, 1630825774, 1937076335, 1195724877, 863005998, 863005998, 1093946670, 1734701165};
constexpr uint32_t audio__n_exts = sizeof(audio_ext_pathids) / sizeof(audio_ext_pathids[0]);

constexpr uint32_t png_ext_multiplier = 152106113;
constexpr uint32_t png_ext_shiftby = 29;
constexpr uint32_t png_ext_pathids[] = {1735290926, 1196314670};
constexpr uint32_t png__n_exts = sizeof(png_ext_pathids) / sizeof(png_ext_pathids[0]);

constexpr uint32_t media_ext_multiplier = 3349455577;
constexpr uint32_t media_ext_shiftby = 25;
constexpr uint32_t media_ext_pathids[] = {1447775534, 1734701162, 1346520407, 1195855662, 1885496695, 1734701162, 1734701162, 1986751790, 1734701162, 1734701162, 1734701162, 1734831918, 1734701162, 1734701162, 1347240494, 1196314670, 1734701162, 1734701162, 1734701162, 1734701162, 1734701162, 1734701162, 1886216750, 1735290926, 1734701162, 1734701162, 1734701162, 1734701162, 1734701162, 1195724874, 1734701162, 1734701162, 1734701162, 1230389550, 1630825774, 1734701162, 1734701162, 879783214, 1734701162, 1179207470, 1734701162, 1769365806, 1734701162, 1734701162, 1734701162, 1734701162, 877677870, 1718183726, 1734701162, 1734701162, 1734701162, 1329809710, 1734701162, 1734701162, 1734701162, 1734701162, 1734701162, 863005998, 1734701162, 1868785966, 1296188759, 1398100047, 1835165047, 1937076335, 1734701162, 860900654, 1734701162, 1734701162, 1196444206, 1734701162, 1734701162, 1734701162, 1093946670, 1734701162, 1734701162, 1734701162, 1735420462, 1128352838, 1734701162, 1667329126}; // {{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'w','e','b','p'},{'j','p','e','g'},{'j','p','e','g'},{'w','e','b','m'},{'f','l','a','c'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'.','M','4','A'},{'j','p','e','g'},{'j','p','e','g'},{'.','p','n','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'.','g','i','f'},{'.','P','N','G'},{'.','m','k','v'},{'W','E','B','P'},{'J','P','E','G'},{'j','p','e','g'},{'W','E','B','M'},{'F','L','A','C'},{'j','p','e','g'},{'j','p','e','g'},{'.','G','I','F'},{'o','p','u','s'},{'.','M','K','V'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'.','o','g','g'},{'j','p','e','g'},{'j','p','e','g'},{'j','p','e','g'},{'.','M','P','4'},{'.','M','P','3'},{'.','b','m','p'},{'.','m','4','a'},{'j','p','e','g'},{'.','i','c','o'},{'.','O','G','G'},{'j','p','e','g'},{'.','j','p','g'},{'j','p','e','g'},{'j','p','e','g'},{'O','P','U','S'},{'.','B','M','P'},{'.','a','v','i'},{'j','p','e','g'},{'.','I','C','O'},{'j','p','e','g'},{'j','p','e','g'},{'.','J','P','G'},{'j','p','e','g'},{'j','p','e','g'},{'.','m','p','4'},{'.','m','p','3'},{'.','A','V','I'}}; */
constexpr uint32_t media__n_exts = sizeof(media_ext_pathids) / sizeof(media_ext_pathids[0]);

constexpr uint32_t vids_ext_multiplier = 3204948216;
constexpr uint32_t vids_ext_shiftby = 26;
constexpr uint32_t vids_ext_pathids[] = {1835165047, 1835165047, 1835165047, 1230389550, 1296188759, 1447775534, 1195855662, 1835165047, 1769365806, 1937076335, 1986751790, 1734831918, 1398100047, 1835165047, 1667329126, 1630825774, 1835165047, 1128352838, 1093946670, 877677870, 1835165047, 860900654, 1835165047, 879783214, 1835165047, 863005998};
constexpr uint32_t vids__n_exts = sizeof(vids_ext_pathids) / sizeof(vids_ext_pathids[0]);
// python3 ~/repos/compsky/static-and-chat-server/modulohash_utils.py --inputs jpeg --inputs .jpg --inputs .png --inputs .gif --inputs .bmp --inputs .ico --inputs webp --inputs JPEG --inputs .JPG --inputs .PNG --inputs .GIF --inputs .BMP --inputs .ICO --inputs WEBP --inputs webm --inputs .mkv --inputs .mp4 --inputs .avi --inputs .ogg --inputs .mp3 --inputs opus --inputs flac --inputs .m4a --inputs WEBM --inputs .MKV --inputs .MP4 --inputs .AVI --inputs .OGG --inputs .MP3 --inputs OPUS --inputs FLAC --inputs .M4A

uint32_t using_ext_multiplier;
uint32_t using_ext_shiftby;
const uint32_t* using_ext_pathids;
uint32_t using__n_exts;

bool is_pathid_a_media_ext(const char* const last_4_chars){
	const uint32_t ext_pathid = reinterpret_cast<const uint32_t*>(last_4_chars)[0];
	
	const uint32_t indx = (ext_pathid*using_ext_multiplier) >> using_ext_shiftby;
	
	/*if ((last_4_chars[0] == '.') and (last_4_chars[1] == 'p'))
		printf("%.4s -> %u -> %u vs %u\n", last_4_chars, indx, (indx<using__n_exts)?using_ext_pathids[indx]:9999, ext_pathid);
	*/
	
	if (indx < using__n_exts){
		if (using_ext_pathids[indx] == ext_pathid){
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

unsigned mystrlen(const char* const ename){
	unsigned _name_len = 0;
	while(ename[_name_len] != 0)
		++_name_len;
	return _name_len;
}

struct ParentOfIgnoreEntity {
	const char* const fullpath;
	const unsigned fullpath_len;
	ParentOfIgnoreEntity(const char* const _fullpath,  const unsigned _len)
	: fullpath(_fullpath)
	, fullpath_len(_len)
	{}
	ParentOfIgnoreEntity(const char* const _fullpath)
	: fullpath(_fullpath)
	, fullpath_len(mystrlen(_fullpath))
	{}
};
struct IgnoreEntity {
	const char* const name;
	const unsigned name_len;
	const unsigned parent_indx;
	IgnoreEntity(const char* const _fullpath,  const unsigned _len,  const unsigned _parent_indx)
	: name(_fullpath)
	, name_len(_len)
	, parent_indx(_parent_indx)
	{}
	IgnoreEntity(const char* const _fullpath,  const unsigned _parent_indx)
	: name(_fullpath)
	, name_len(mystrlen(_fullpath))
	, parent_indx(_parent_indx)
	{}
};

struct QueuedName {
	char buf[64];
	char* value;
	const unsigned value_len;
	const unsigned parent_indx;
	const unsigned path_offset;
	QueuedName(const char* const _name,  const unsigned _name_len,  const unsigned _parent_indx,  const unsigned _path_offset)
	: value_len(_name_len)
	, parent_indx(_parent_indx)
	, path_offset(_path_offset)
	{
		if (_name_len > 64){
			[[unlikely]]
			this->value = reinterpret_cast<char*>(malloc(_name_len));
		} else {
			this->value = this->buf;
		}
		memcpy(this->value, _name, _name_len);
	}
	QueuedName(const QueuedName&& othr)
	: value_len(othr.value_len)
	, parent_indx(othr.parent_indx)
	, path_offset(othr.path_offset)
	{
		this->value = othr.value;
		memcpy(this->value, othr.value, othr.value_len);
	}
	~QueuedName(){
		if (this->value != this->buf){
			[[unlikely]]
			free(this->value);
		}
	}
};

template<bool using_extfiltering,  bool using_minsz,  bool print_total_sz,  bool print_filepaths>
void find(const char* const _path,  const unsigned _path_len,  const unsigned minsz){
	if constexpr ((not print_total_sz) and (not print_filepaths)){
		return;
	}
	
	std::vector<ParentOfIgnoreEntity> parents_of_ignoreentity;
	parents_of_ignoreentity.reserve(100);
	std::vector<IgnoreEntity> ignoreentities;
	ignoreentities.reserve(100);
	
	parents_of_ignoreentity.emplace_back("/home/vangelic/.local/lib");
	ignoreentities.emplace_back("python3.10",0);
	parents_of_ignoreentity.emplace_back("/home/vangelic/repos/compsky/tagem-secret/etc/R");
	ignoreentities.emplace_back("redpill",1);
	parents_of_ignoreentity.emplace_back("/home/vangelic/snap");
	ignoreentities.emplace_back("chromium",2);
	ignoreentities.emplace_back("firefox",2);
	ignoreentities.emplace_back("kdenlive",2);
	parents_of_ignoreentity.emplace_back("/home/vangelic/repos/ml");
	ignoreentities.emplace_back("whisper.cpp",3);
	ignoreentities.emplace_back("llama.cpp",3);
	ignoreentities.emplace_back("imaginaire",3);
	ignoreentities.emplace_back("webgl-convolution",3);
	parents_of_ignoreentity.emplace_back("/home/vangelic/repos/compsky");
	ignoreentities.emplace_back("static-and-chat-server",4);
	ignoreentities.emplace_back("fake-student-newspaper",4);
	ignoreentities.emplace_back("fake-student-newspaper__from-baththingy",4);
	parents_of_ignoreentity.emplace_back("/home/vangelic/Documents");
	ignoreentities.emplace_back("kdenlive-projects",5);
	parents_of_ignoreentity.emplace_back("/home/vangelic/.cache");
	ignoreentities.emplace_back("kdenlive",6);
	parents_of_ignoreentity.emplace_back("/media/vangelic/DATA");
	ignoreentities.emplace_back(".bob",7);
	ignoreentities.emplace_back("_tmp_emergency",7);
	ignoreentities.emplace_back("webcache",7);
	parents_of_ignoreentity.emplace_back("/media/vangelic/DATA/media/img");
	ignoreentities.emplace_back("by-source",8);
	parents_of_ignoreentity.emplace_back("/media/vangelic/DATA/media/vid");
	ignoreentities.emplace_back("by-source",9);
	parents_of_ignoreentity.emplace_back("/media/vangelic/DATA/media");
	ignoreentities.emplace_back("by-source",10);
	parents_of_ignoreentity.emplace_back("/home/vangelic");
	ignoreentities.emplace_back(".tldr",11);
	ignoreentities.emplace_back(".mozilla",11);
	parents_of_ignoreentity.emplace_back("/home/vangelic/repos/scrapers");
	ignoreentities.emplace_back("yt-dlc",12);
	parents_of_ignoreentity.emplace_back("/home/vangelic/Documents");
	ignoreentities.emplace_back(".notes",13);
	parents_of_ignoreentity.emplace_back("/home/vangelic/.cache");
	ignoreentities.emplace_back("plasmashell",14);
	ignoreentities.emplace_back("discover",14);
	ignoreentities.emplace_back("mesa_shader_cache",14);
	
	const unsigned parents_of_ignoreentity__sz = parents_of_ignoreentity.size();
	
	std::vector<QueuedName> queue;
	queue.reserve(1000000); // 1 million file names
	queue.emplace_back(_path, _path_len, 0, 0);
	
	constexpr unsigned space_for_errstr = 100;
	char full_path_buf[4096];
	char* const full_path = full_path_buf + space_for_errstr;
	
	uint64_t total_sz = 0;
	unsigned queue_indx = 0;
	while(queue_indx != queue.size()){
		const QueuedName& queue_item = queue[queue_indx];
		const char* const dirname = queue_item.value;
		unsigned full_path_len;
		{
			unsigned _parent_indx = queue_indx;
			full_path_len = queue_item.path_offset + queue_item.value_len;
			do {
				//printf("parent (%u) %.*s\n", _parent_indx, (int)queue[_parent_indx].value_len, queue[_parent_indx].value); fflush(stdout);
				// NOTE: Cache unfriendly (traversing backwards)
				memcpy(full_path + queue[_parent_indx].path_offset,  queue[_parent_indx].value,  queue[_parent_indx].value_len);
				full_path[queue[_parent_indx].path_offset + queue[_parent_indx].value_len] = '/';
				_parent_indx = queue[_parent_indx].parent_indx;
			} while(_parent_indx != 0);
			//printf("full_path[%u]: %.*s\n", full_path_len, (int)full_path_len, full_path); fflush(stdout);
			full_path[full_path_len] = '\0';
		}
		
		unsigned parent_of_ignoreentity__indx = 0;
		for (;  parent_of_ignoreentity__indx < parents_of_ignoreentity__sz;  ++parent_of_ignoreentity__indx){
			const ParentOfIgnoreEntity& parent_of_ignoreentity = parents_of_ignoreentity[parent_of_ignoreentity__indx];
			if (full_path_len == parent_of_ignoreentity.fullpath_len){
				bool matches = true;
				for (unsigned i = 0;  i < full_path_len;  ++i){
					matches &= (parent_of_ignoreentity.fullpath[i] == full_path[i]);
				}
				if (matches){
					// printf("Matches parent_of_ignoreentity %.*s\n", (int)full_path_len, full_path);
					break;
				}
			}
		}
		
		DIR* const dir = opendir(full_path);
		if (dir != nullptr){
			struct dirent* e;
			while ((e=readdir(dir)) != nullptr){
				const char* const ename = e->d_name;
				if (ename == nullptr){
					[[unlikely]]
					continue;
				}
				
				const unsigned name_len = mystrlen(ename);
				
				if (e->d_type == DT_DIR){
					if (name_len <= 2){
						if (ename[0] == '.'){
							if (ename[1] == 0)
								continue;
							if ((ename[1] == '.') and (ename[2] == 0))
								continue;
						}
					}
					
					if ((name_len == 4) and (ename[0] == '.') and (ename[1] == 'g') and (ename[2] == 'i') and (ename[3] == 't')){ // .git
						continue;
					}
					
					bool must_ignore = false;
					if (parent_of_ignoreentity__indx != parents_of_ignoreentity__sz){
						[[unlikely]]
						// printf("testing %.*s [%u]\n", (int)name_len, ename, name_len);
						for (const IgnoreEntity& ignoreentity : ignoreentities){
							if (ignoreentity.parent_indx == parent_of_ignoreentity__indx){
								// printf("vs %.*s [%u]\n", (int)ignoreentity.name_len, ignoreentity.name, ignoreentity.name_len);
								if (ignoreentity.name_len == name_len){
									must_ignore = true;
									for (unsigned i = 0;  i < ignoreentity.name_len;  ++i){
										must_ignore &= (ignoreentity.name[i] == ename[i]);
									}
									if (must_ignore){
										break;
									}
								}
								// printf("must_ignore %s\n", must_ignore?"TRUE":"FALSE");
							}
						}
					}
					if (not must_ignore){
						[[likely]]
						queue.emplace_back(ename, name_len, queue_indx, full_path_len+1);
					} else {
						// printf("IGNORING: %.*s%.*s\n", (int)full_path_len, full_path, (int)name_len, ename);
					}
				} else if (e->d_type == DT_REG) { // avoids links
					bool should_print = true;
					if constexpr (using_extfiltering){
						if (name_len < 4){
							should_print = false;
						} else {
							if (not is_pathid_a_media_ext(ename+name_len-4)){
								should_print = false;
							}
						}
					}
					
					full_path[full_path_len] = '/';
					memcpy(full_path + full_path_len + 1,  ename,  name_len);
					const unsigned full_path_len_inc_name = full_path_len + 1 + name_len;
					
					if constexpr (using_minsz or print_total_sz){
						if (should_print or print_total_sz){
							struct stat statbuf;
							full_path[full_path_len_inc_name] = '\0';
							if (stat(full_path, &statbuf) != 0){
								[[unlikely]]
								
								memcpy(full_path-12, "stat error: ", 12);
								full_path[full_path_len_inc_name] = '\n';
								write(2, full_path-12, full_path_len_inc_name+13);
								
								if constexpr (using_minsz){
									should_print = false;
								}
							} else {
								if constexpr (print_total_sz){
									total_sz += statbuf.st_size;
								}
								if constexpr (using_minsz){
									if (statbuf.st_size < minsz){
										should_print = false;
									}
								}
							}
						}
					}
					
					if constexpr (print_filepaths){
						if (should_print){
							full_path[full_path_len_inc_name] = '\n';
							write(1, full_path, full_path_len_inc_name+1);
						}
					}
				}
			}
			closedir(dir);
		} else {
			[[unlikely]]
			memcpy(full_path-17, "Cannot open dir: ", 17);
			full_path[full_path_len] = '\n';
			write(2, full_path-17, full_path_len+18);
		}
		
		if constexpr (print_total_sz){
			unsigned n_digits = 0;
			{
				uint64_t _total_sz = total_sz;
				do {
					_total_sz /= 10;
					++n_digits;
				} while(_total_sz != 0);
			}
			
			{
				uint64_t _total_sz = total_sz;
				char* itr = full_path_buf + n_digits - 1;
				do {
					*itr = '0' + (_total_sz % 10);
					_total_sz /= 10;
					--itr;
				} while(_total_sz != 0);
			}
			
			full_path_buf[n_digits] = ' ';
			full_path_buf[n_digits+1] = '\r';
			write(1, full_path_buf, n_digits+2);
		}
		
		full_path[full_path_len] = '/'; // Overwrite null byte
		++queue_indx;
	}
	
	if constexpr (print_total_sz){
		unsigned n_digits = 0;
		{
			uint64_t _total_sz = total_sz;
			do {
				_total_sz /= 10;
				++n_digits;
			} while(_total_sz != 0);
		}
		
		char* itr = full_path + n_digits - 1;
		do {
			*itr = '0' + (total_sz % 10);
			total_sz /= 10;
			--itr;
		} while(total_sz != 0);
		
		full_path[n_digits] = '\n';
		write(1, full_path, n_digits+1);
	}
}

constexpr
unsigned s2int(const char* s){
	unsigned n = 0;
	while(*s != 0){
		n *= 10;
		n += *s - '0';
		++s;
	}
	return n;
}

int main(const int argc,  const char* const* const argv){
	if (argc == 1){
		print_help_output:
		[[unlikely]]
		write(
			2,
			"USAGE: [[OPTIONS]] [[DIRECTORIES]]\n"
			"\n"
			"OPTIONS:\n"
			"	-s [MIN_SIZE]\n"
			"	-e [EXT: vid|media|png|audio]\n"
			"	-S print total size\n"
			"	-f print file paths\n"
			, 133
		);
		return 1;
	}
	
	bool print_total_sz = false;
	bool print_filepaths = false;
	unsigned minsz = 0;
	using_ext_multiplier = 0;
	for (int i = 1;  i < argc;  ++i){
		const char* const arg = argv[i];
		unsigned argv_len = mystrlen(arg);
		
		if ((argv_len == 2) and (arg[0] == '-')){
			switch(arg[1]){
				case 'f':
					print_filepaths = true;
					break;
				case 'S':
					print_total_sz = true;
					break;
				case 's':
					++i;
					if (i == argc){
						[[unlikely]]
						write(2, "-s requires argument\n", 21);
						return 1;
					}
					minsz = s2int(argv[i]);
					break;
				case 'e':
					++i;
					{
						const char* const _arg = argv[i];
						if ((_arg[0] == 'v') and (_arg[1] == 'i') and (_arg[2] == 'd')){
							using_ext_multiplier = vids_ext_multiplier;
							using_ext_shiftby = vids_ext_shiftby;
							using_ext_pathids = vids_ext_pathids;
							using__n_exts = vids__n_exts;
						} else if ((_arg[0] == 'a') and (_arg[1] == 'u') and (_arg[2] == 'd')){
							using_ext_multiplier = audio_ext_multiplier;
							using_ext_shiftby = audio_ext_shiftby;
							using_ext_pathids = audio_ext_pathids;
							using__n_exts = audio__n_exts;
						} else if ((_arg[0] == 'm') and (_arg[1] == 'e') and (_arg[2] == 'd')){
							using_ext_multiplier = media_ext_multiplier;
							using_ext_shiftby = media_ext_shiftby;
							using_ext_pathids = media_ext_pathids;
							using__n_exts = media__n_exts;
						} else if ((_arg[0] == 'p') and (_arg[1] == 'n') and (_arg[2] == 'g')){
							using_ext_multiplier = png_ext_multiplier;
							using_ext_shiftby = png_ext_shiftby;
							using_ext_pathids = png_ext_pathids;
							using__n_exts = png__n_exts;
						} else {
							goto print_help_output;
						}
					}
					break;
				default:
					goto print_help_output;
			}
			continue;
		}
		
		if (arg[argv_len-1] == '/'){
			argv_len -= 1;
		}
		
		if (print_filepaths){
			if (print_total_sz){
				if (using_ext_multiplier == 0){
					if (minsz == 0)
						find<false,false,true ,true>(arg, argv_len, minsz);
					else
						find<false,true ,true ,true>(arg, argv_len, minsz);
				} else {
					if (minsz == 0)
						find<true, false,true ,true>(arg, argv_len, minsz);
					else
						find<true, true ,true ,true>(arg, argv_len, minsz);
				}
			} else {
				if (using_ext_multiplier == 0){
					if (minsz == 0)
						find<false,false,false,true>(arg, argv_len, minsz);
					else
						find<false,true ,false,true>(arg, argv_len, minsz);
				} else {
					if (minsz == 0)
						find<true, false,false,true>(arg, argv_len, minsz);
					else
						find<true, true ,false,true>(arg, argv_len, minsz);
				}
			}
		} else {
			if (print_total_sz){
				if (using_ext_multiplier == 0){
					if (minsz == 0)
						find<false,false,true ,false>(arg, argv_len, minsz);
					else
						find<false,true ,true ,false>(arg, argv_len, minsz);
				} else {
					if (minsz == 0)
						find<true, false,true ,false>(arg, argv_len, minsz);
					else
						find<true, true ,true ,false>(arg, argv_len, minsz);
				}
			} else {
				if (using_ext_multiplier == 0){
					if (minsz == 0)
						find<false,false,false,false>(arg, argv_len, minsz);
					else
						find<false,true ,false,false>(arg, argv_len, minsz);
				} else {
					if (minsz == 0)
						find<true, false,false,false>(arg, argv_len, minsz);
					else
						find<true, true ,false,false>(arg, argv_len, minsz);
				}
			}
		}
	}
	return 0;
}
