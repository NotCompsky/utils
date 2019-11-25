#include <stdio.h>
#include <dirent.h>

int main(const int argc,  const char** argv){
	DIR *dir;
	struct dirent *ent;

	dir = opendir(argv[1]);

	while((ent = readdir(dir)))
		printf("%s\n", ent->d_name);

	closedir(dir);

	return 0;
}
