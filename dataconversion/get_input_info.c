#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <qsort.h>
#include <unistd.h>

int is_regular_file(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

void listFilesRecursively(char *basePath, char ** fpaths, int *index)
{
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	// Unable to open directory stream
	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			// Construct new path from our base path
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);
			if(is_regular_file(path) == 1){
				char * tmp = strdup(path);
				if(*index >= 1248 ){
					printf("error\n");
					exit(1);
				}
				fpaths[*index] = tmp;
				*index += 1;
			}

			listFilesRecursively(path, fpaths, index);

		}
	}

	closedir(dir);
}

void list_files_and_variables(char * Path, FILE * fptr){
	char path[1000];
	struct dirent *dp;
	DIR *dir = opendir(Path);
	while ((dp = readdir(dir)) != NULL){
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
			fprintf(fptr, "Main: %s\n", dp->d_name);
			strcpy(path, Path);
			strcat(path, "/");
			strcat(path, dp->d_name);

			DIR * dir2 = opendir(path);
			fprintf(fptr,"total_variables: \n");
			while ((dp = readdir(dir2)) != NULL){
				if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
					fprintf(fptr,"%s\n", dp->d_name);
				}
			}
			fprintf(fptr,"end_of_total_variables\n");
			closedir(dir2);
			dir2 = opendir(path);
			while ((dp = readdir(dir2)) != NULL){
				char path2[1000];
				if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
					strcpy(path2, path);
					strcat(path2, "/");
					strcat(path2, dp->d_name);
					char * fpaths[1248];
					int * index = malloc(sizeof(int));
					*index = 0;
					listFilesRecursively(path2, fpaths, index);
					if(*index < 1248){
						printf("not enough data for var %s in path %s \n", dp->d_name, path2);
						continue;
					}
					fprintf(fptr,"var: %s\n", dp->d_name);
					_qsort(fpaths, sizeof(char*), 0, (*index)-1, (int (*)(void*, void*))(cmpstr));
					for(int i = 0; i < *index; i++){
						fprintf(fptr, "%s\n",fpaths[i]);
						free(fpaths[i]);    
					}
					free(index);

				}
			}
			closedir(dir2);
			fprintf(fptr,"end_of_main\n");
		}
	}
	closedir(dir);

}




int main(int argc, char**argv){
	FILE *fptr;
	fptr = fopen("required_info.txt", "w+");
	if(argc != 2){
		printf("Please specity file path");
		exit(1);
	}
	char * file_path = strdup(argv[1]);
	list_files_and_variables(file_path, fptr);
	fclose(fptr);
	free(file_path);
	return 0;
}
