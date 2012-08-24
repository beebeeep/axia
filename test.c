#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) 
{
	DIR *dir = opendir(argv[1]);
	struct dirent *entry;
	while((entry = readdir(dir)) != NULL) {
		printf("\n%s ", entry->d_name);
		int i = 0;
		while(entry->d_name[i]) {
			unsigned int a = (unsigned char) entry->d_name[i++];
			printf("0x%x(%c) ", a, entry->d_name[i-1]);
		}
	}
	return 0;
}
