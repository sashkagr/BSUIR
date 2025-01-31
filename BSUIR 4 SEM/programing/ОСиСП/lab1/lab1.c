#define _POSIX_C_SOURCE 200809L
//#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <malloc.h>
// 10 - symlink
// 8 - file
// 4 - directory

void output_data(char from, char to, int need_files, int need_directory, int need_symlink,struct dirent *dir, char* current_dir, DIR *d){
	for(char i = from; i <= to; i++){
		d = opendir(current_dir);
		while ((dir = readdir(d)) != NULL) {
			int type = dir->d_type;
			if (((type == 8 && need_files == 1) || (type == 4 && need_directory == 1) || (type == 10 && need_symlink == 1)) && dir->d_name[0] == i)
				printf("%s\n", dir->d_name);
			
		}
		closedir(d);
	}
}

int main(int argc, char *argv[]){
	
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'h'){
		printf("usage: dirwalk [dir] [options]\n-l -- show links\n-d -- show directory\n-f -- show files\n-s -- sort\n");
		return 0;
	}
	
	int need_symlink = 0, need_files = 0, need_directory = 0, step = 1, sort = 0;
	char* current_dir;
     int c;
    while ((c = getopt(argc, argv, "ldfs")) != -1) {
        switch (c) {
            case 'l':
                need_symlink = 1;
                break;
            case 'd':
                need_directory = 1;
                break;
            case 'f':
                need_files = 1;
                break;
            case 's':
                sort = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s]\n", argv[0]);
                return 1;
        }
    }

	if (argc == 1){ // no arguments
		current_dir = (char*)malloc(2);
		current_dir[0] = '.';
		current_dir[1] = '\0';
		need_symlink = 1; need_files = 1; need_directory = 1;
		step = -1;
	} else if (argv[step][0] == '-'){ //arguments start with -
		current_dir = (char*)malloc(2);
		current_dir[0] = '.';
		current_dir[1] = '\0';
	} else {                        //normal arguments
		current_dir = argv[1];
		step++;
	}
	
	// if (step > 1 && step > argc-1){ //checker
	// 	need_directory = 1;
	// 	need_files = 1;
	// 	need_symlink = 1;
	// 	step = -1;
	// }
	
	// if ( step != -1)        // takings '-' arguments
	// 	for(int i = 0; argv[step][i] != '\0'; i++){
	// 		if (argv[step][i] == 'l')
	// 			need_symlink = 1;
	// 		else if (argv[step][i] == 'd')
	// 			need_directory = 1;
	// 		else if (argv[step][i] == 'f')
	// 			need_files = 1;
	// 		else if (argv[step][i] == 's')
	// 			sort = 1;
	// 	}
		
		if (sort == 1 && need_directory == 0 && need_files == 0 && need_symlink == 0){
			need_symlink = 1;
			need_files = 1;
			need_directory = 1;
		}
		
		//starting output
		DIR *d;
		struct dirent *dir;
		d = opendir(current_dir);
		if (d) {
			if (sort == 1){
				//else
				d = opendir(current_dir);
				while ((dir = readdir(d)) != NULL) {
					int type = dir->d_type;
					if (((type == 8 && need_files == 1) || (type == 4 && need_directory == 1) || 
                    (type == 10 && need_symlink == 1)) && (!(dir->d_name[0] >= '0' && dir->d_name[0] <='9') && 
                    !(dir->d_name[0] >= 'a' && dir->d_name[0] <= 'z') && !(dir->d_name[0] >= 'A' && dir->d_name[0] <= 'Z')))
						printf("%s\n", dir->d_name);
				}
				closedir(d);
				//for numbers
				output_data('0','9',need_files,need_directory,need_symlink,dir,current_dir,d);
				//for small letters
				output_data('a','z',need_files,need_directory,need_symlink,dir,current_dir,d);
				//for big letters
				output_data('A','Z',need_files,need_directory,need_symlink,dir,current_dir,d);
				return 0;
			}
			//output without sort
			while ((dir = readdir(d)) != NULL) {
				int type = dir->d_type;
				if ((type == 8 && need_files == 1) || (type == 4 && need_directory == 1) || (type == 10 && need_symlink == 1))
					printf("%s\n", dir->d_name);
				
			}
			closedir(d);
		}
		
		return 0;
}