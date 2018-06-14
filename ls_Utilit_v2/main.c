#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#define STR_EQ(s, v) (strcmp((s), (v)) == 0)

bool flag_input = false;
bool flag_column = false;

struct dirent *readdir(DIR *dirp);

char dir[256];

int mkdir(const char *pathname, mode_t mode);
int rmdir(const char *pathname);
int closedir(DIR *dirp);

void print_help();

int main(int argc, char** argv) {

    setlocale(LC_ALL, "");

    for (int i = 1; i < argc; ++i) {
        if (!flag_input) {
            if (STR_EQ(argv[i], "--help")) {
                print_help();
            } else if ((strlen(argv[i]) > 1) && (argv[i][0] == '-') && (argv[i][1] != '-')) {

                size_t keys = strlen(argv[i]);

                for (size_t j = 1; j < keys; ++j) {
                    switch (argv[i][j]) {
                        case 'C':
                            flag_column = true;
                            break;
                        default:
                            fprintf(stderr, "Invalid input '%c'\n Try \"%s --help for details.\n",
                                    argv[i][j], argv[0]);
                            return 0;
                    }
                }
            }

            else {
                flag_input = true;

                DIR *dp = NULL;

                struct dirent *dptr = NULL;
                char buff[128];

                memset(buff, 0, sizeof(buff));
                strcpy(buff, argv[1]);

                if (NULL == (dp = opendir(argv[1]))) {

                    printf("\n Can't open Input directory [%s]\n", argv[1]);
                    exit(1);
                }
                else {

                    if(buff[strlen(buff)-1]=='/') {

                        strncpy(buff+strlen(buff),"newDir/",7);
                    }
                    else {

                        strncpy(buff+strlen(buff),"/newDir/",8);
                    }

                    printf("\n Creating a new directory [%s]\n",buff);

                    mkdir(buff,S_IRWXU|S_IRWXG|S_IRWXO);
                    printf("\n The contents of directory [%s] are as follows: \n",argv[1]);

                    while(NULL != (dptr = readdir(dp)) )
                    {
                        printf(" [%s] ",dptr->d_name);
                    }

                    closedir(dp);

                    rmdir(buff);
                    printf("\n");
                }
            }
        }
    }

    if (flag_column) {
        //UNDO: add -C option
    }

    return 0;
}

void print_help() {

    printf("\nUsage: ls [OPTION]... [DIR]...\n\n"
           "Options: \n"
           "-C     *print dirs in column format*\n"
           "--help *print help list*\n\n");
}

