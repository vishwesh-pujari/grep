#include <unistd.h>
#include <stdio.h>
#include <glob.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "code/matching.h"

void dirWalk(char*, char *, int);
int currentDir(char*, int);

int main(int argc, char **argv)
{
    /*currentDir();
    int ret = 0;
    ret = chdir("code");
    currentDir();
    // For Printing The path according to current directory.
    // The directory in which the program is running, find strlen() of the directory path
    // from strlen() + 1 print the next directory path
    if (ret == 0)
        dirWalk("*");
    else
        printf("NOOOOO\n");*/

    // Condition for entered filename is a directory should be placed here


    int i, lenArgv = strlen(argv[1]);
    for (i = lenArgv - 1; i >= 0; i--)
        if (argv[1][i] == '/')
            break;
    
    char str[1024];

    if (i == -1)
        getcwd(str, sizeof(str));
    else {
        strcpy(str, argv[1]);
        str[i] = '\0'; // str now contains the directory name
    }
    chdir(str);
    printf("argv[1] + i + 1 = %s, str = %s\n", argv[1] + i + 1, str);
    dirWalk(argv[1] + i + 1, str, i);
    //dirWalk("code/*"); // IMPPPPPPPPPPPPPPPPPPPPP
    
    return 0;
}

/*void dirWalk(char *filePattern) {
    printf("Pattern = %s\n", filePattern);
    glob_t paths; // this structure will hold all information about matched filenames
    int retval, ret;
    
    paths.gl_pathc = 0; // Count of paths matched by the pattern.
    paths.gl_pathv = NULL; // List of matched pathnames.
    paths.gl_offs = 0; // Slots to reserve in `gl_pathv'. 

    retval = glob(filePattern, GLOB_NOSORT, NULL, &paths);
    if (retval == 0) {
        int i;
        for(i = 0; i < paths.gl_pathc; i++) {
            printf("[%d]: %s\n", i, paths.gl_pathv[i]);
            ret = chdir(paths.gl_pathv[i]);
            if (ret == 0) { // means paths.gl_pathv[i] is a directory
                currentDir();
                dirWalk(filePattern); // This now works according to the changed directory
                chdir(".."); // Setting back the direectory to original
                currentDir();
            }
        }
        
        globfree( &paths );
    } else {
        puts( "glob() failed" );
    }
    return;
}*/

int currentDir(char *str, int j) {
   char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
        int *matches = substr(cwd, str, NO_IGNORE_CASE);
        int i = 0;
        while (matches[i] != -1)
            i++;
        int dirLength = matches[i - 1];
        free(matches);
        if (j == -1) {
            if (strcmp(str, cwd)) // if not equal
                printf("%s/", cwd + dirLength + strlen(str) + 1);
            // if equal then we don't print
        } else
            printf("%s/", cwd + dirLength);
   } else {
       perror("getcwd() error");
       return 1;
   }
   return 0;
}

void dirWalk(char *filePattern, char *str, int j) {
    glob_t paths; // this structure will hold all information about matched filenames
    int retval, ret;
    
    paths.gl_pathc = 0; // Count of paths matched by the pattern.
    paths.gl_pathv = NULL; // List of matched pathnames.
    paths.gl_offs = 0; // Slots to reserve in `gl_pathv'. 

    retval = glob(filePattern, GLOB_NOSORT, NULL, &paths);
    if (retval == 0) {
        int i;
        for(i = 0; i < paths.gl_pathc; i++) {
            //printf("[%d]: %s\n", i, paths.gl_pathv[i]);
            ret = chdir(paths.gl_pathv[i]);
            if (ret == 0) { // means paths.gl_pathv[i] is a directory
                //currentDir();
                dirWalk(filePattern, str, j); // This now works according to the changed directory
                chdir(".."); // Setting back the direectory to original
                //currentDir();
            } else {
                currentDir(str, j);
				printf("%s\n", paths.gl_pathv[i]);
			}
        }
        globfree( &paths );
    } else {
        puts( "glob() failed" );
    }
    return;
}
