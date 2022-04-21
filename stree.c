/* nftw_dir_tree.c
   Demonstrate the use of nftw(3). Walk though the directory tree specified
   on the command line (or the current working directory if no directory
   is specified on the command line), displaying an indented hierarchy
   of files in the tree. For each file, display:
      * a letter indicating the file type (using the same letters as "ls -l")
      * a string indicating the file type, as supplied by nftw()
      * the file's i-node number.
*/
#define _GNU_SOURCE
#define _XOPEN_SOURCE 600 /* Get nftw() */
#include <ftw.h>
#include <sys/types.h>    /* Type definitions used by many programs */
#include <stdio.h>        /* Standard I/O functions */
#include <stdlib.h>       /* Prototypes of commonly used library functions,
                             plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>       /* Prototypes for many system calls */
#include <errno.h>        /* Declares errno and defines error constants */
#include <string.h>       /* Commonly used string-handling functions */

#include <pwd.h>
#include <grp.h>
#include <limits.h>

int stringlength(char* str){
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

static int              /* Callback function called by ftw() */
dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    // If hidden directory skip sub tree.
    if(strstr(pathname, "/.") != NULL){
        return FTW_SKIP_SUBTREE;
    }
    // If current dir continue.
    else if(!strcmp(pathname,".")){
        return FTW_CONTINUE;
    }
     printf(" %*s", 4 * ftwb->level, " ");
    if (type == FTW_NS) {                  /* Could not stat() file */
        printf("?");
    } else {
        printf("[");
        switch (sbuf->st_mode & S_IFMT) {  /* Print file type */
        case S_IFREG:  printf("-"); break;
        case S_IFDIR:  printf("d"); break;
        case S_IFCHR:  printf("c"); break;
        case S_IFBLK:  printf("b"); break;
        case S_IFLNK:  printf("l"); break;
        case S_IFIFO:  printf("p"); break;
        case S_IFSOCK: printf("s"); break;
        default:       printf("?"); break; /* Should never happen (on Linux) */
        }
    }
    printf("%s", (sbuf->st_mode & S_IRUSR) ? "r" : "-");
    printf("%s", (sbuf->st_mode & S_IWUSR) ? "w" : "-");
    printf("%s", (sbuf->st_mode & S_IXUSR) ? "x" : "-");
    printf("%s", (sbuf->st_mode & S_IRGRP) ? "r" : "-");
    printf("%s", (sbuf->st_mode & S_IWGRP) ? "w" : "-");
    printf("%s", (sbuf->st_mode & S_IXGRP) ? "x" : "-");
    printf("%s", (sbuf->st_mode & S_IROTH) ? "r" : "-");
    printf("%s", (sbuf->st_mode & S_IWOTH) ? "w" : "-");
    printf("%s ", (sbuf->st_mode & S_IXOTH) ? "x" : "-");
    struct passwd *pw = getpwuid(sbuf->st_uid);
    struct group *gr = getgrgid(sbuf->st_gid);
    printf("%s   ", pw->pw_name);
    printf("%s ", gr->gr_name);
    printf("%15ld" ,sbuf->st_size);
    printf("] %s\n",  &pathname[ftwb->base]);     /* Print basename */
    return 0;                                   /* Tell nftw() to continue */
}

int
main(int argc, char *argv[])
{
    int flags = FTW_ACTIONRETVAL;
    if(argc == 1){
        if (nftw(".", dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
        }
    }
    else{
        if (nftw(argv[1], dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
