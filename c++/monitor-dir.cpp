#include <unistd.h>
#include <sys/inotify.h>
#include <string_view> // for std::string_view for asciify only
#include <compsky/asciify/asciify.hpp>
#include <compsky/utils/ptrdiff.hpp>


/*
NOTE: You cannot get the process ID of the process which caused the event - use auditd for that
auditd example:
	sudo auditctl -w "$dirpath" -p war -k mylogtitle
whose logs can be searched with my custom script audits

struct inotify_event {
  int wd;         /∗ The watch descriptor ∗/
  uint32_t mask;     /∗ Watch mask ∗/
  uint32_t cookie;    /∗ A cookie to tie two events together ∗/
  uint32_t len;        /∗ The length of the filename found in the name field ∗/
  char name __flexarr;    /∗ The name of the file, padding to the end with NULs ∗/    
}
*/


constexpr std::size_t EVENT_SIZE = sizeof(struct inotify_event);
constexpr std::size_t BUF_LEN  = 1024 * (EVENT_SIZE + 16);


int main(int argc,  const char* const* const argv){
	char buffer[BUF_LEN];
	char write_buf[1024] = "USAGE: monitor-dir [DIRPATH]\nERROR: Cannot initialise inotify\nERROR: Cannot read\n";
	
	if (argc == 1){
		write(2, write_buf, 29);
		return 1;
	}
	
	
	const int fd = inotify_init();
	if (fd < 0){
		write(2, write_buf+29, 33);
		return 1;
	}
	
	const char* const dirpath = argv[1];
	
	const int wd = inotify_add_watch(
		fd, dirpath, 
		IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_ACCESS | IN_ATTRIB | IN_DELETE_SELF | IN_OPEN
	);
	
	while(true){ // TODO: Use proper thread handling instead of this
		const int length = read(fd, buffer, BUF_LEN);  
		
		if (length < 0){
			write(2, write_buf+62, 19);
			return 1;
		}  
		
		int i = 0;
		while(i < length){
			struct inotify_event* event = (struct inotify_event*)&buffer[i];
			if (event->len != 0){
				constexpr const char* file_typs[] = {"file","dir"};
				const char* const file_typ = file_typs[(event->mask & IN_ISDIR) / IN_ISDIR];
				
				constexpr const char* actions[] = {"unknown","created","deleted","modified","moved from","moved to","access","attrib changed","root dir deleted","opened"};
				const char* const action = actions[
					// 0 -> unknown
					1*(event->mask & IN_CREATE)/IN_CREATE +
					2*(event->mask & IN_DELETE)/IN_DELETE +
					3*(event->mask & IN_MODIFY)/IN_MODIFY +
					4*(event->mask & IN_MOVED_FROM)/IN_MOVED_FROM +
					5*(event->mask & IN_MOVED_TO)/IN_MOVED_TO +
					6*(event->mask & IN_ACCESS)/IN_ACCESS +
					7*(event->mask & IN_ATTRIB)/IN_ATTRIB +
					8*(event->mask & IN_DELETE_SELF)/IN_DELETE_SELF +
					9*(event->mask & IN_OPEN)/IN_OPEN
				];
				
				char* write_itr = write_buf;
				compsky::asciify::asciify(write_itr, file_typ, ' ', event->name, ':', ' ', action, '\n');
				write(1, write_buf, compsky::utils::ptrdiff(write_itr, write_buf));
			}
			i += EVENT_SIZE + event->len;
		}
	}
	inotify_rm_watch(fd, wd);
	close(fd);
	return 0;
}
