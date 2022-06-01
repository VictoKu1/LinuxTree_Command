#define _GNU_SOURCE
#define _XOPEN_SOURCE 600
#include <ftw.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <dirent.h>

int arr[INT_MAX];
int numberOfFiles = 0;
int numberOfDirectories = 0;

int checkifdir(const char *path){
    struct stat statbuf;
    if(stat(path, &statbuf) == -1)
    {
        return 0;
    }
    if(S_ISDIR(statbuf.st_mode)){
        return 1;
    }
    return 0;
}

void helper(const char *path, int level)
{
    DIR *dir;
    dir = opendir(path);
    if (dir == NULL)
    {
        printf("\n");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            arr[level]++;
        }
    }
    closedir(dir);
}

static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if (strstr(pathname, "/.") != NULL)
    {
        return FTW_SKIP_SUBTREE;
    }
    if (ftwb->level == 0)
    {
        printf("%s\n", pathname);
        helper(&pathname[ftwb->base], ftwb->level + 1);
    }
    else
    {
        if (type == FTW_NS)
        {
            printf("?");
        }
        else
        {
            if (type == FTW_F)
            {
                numberOfFiles++;
            }
            if (type == FTW_D)
            {
                helper(&pathname[ftwb->base], ftwb->level + 1);
                numberOfDirectories++;
            }
            int level = 0;
            int index = 1;
            level = ftwb->level;
            while (level > 1)
            {
                if (arr[index] > 0)
                {
                    printf("│");
                }
                else
                {
                    printf(" ");
                }
                index++;
                printf("%*s", 4, " ");
                level--;
            }
            if (arr[ftwb->level] == 1)
            {
                printf("└──");
            }
            else
            {
                printf("├──");
            }
            printf(" [");
            arr[ftwb->level]--;
            switch (sbuf->st_mode & S_IFMT)
            {
            case S_IFREG:
                printf("-");
                break;
            case S_IFDIR:
                printf("d");
                break;
            case S_IFCHR:
                printf("c");
                break;
            case S_IFBLK:
                printf("b");
                break;
            case S_IFLNK:
                printf("l");
                break;
            case S_IFIFO:
                printf("p");
                break;
            case S_IFSOCK:
                printf("s");
                break;
            default:
                printf("?");
                break;
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
        printf("%s ", pw->pw_name);
        printf("%s ", gr->gr_name);
        printf("%11ld", sbuf->st_size);
        printf("]  %s\n", &pathname[ftwb->base]);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int flags = FTW_CHDIR | FTW_ACTIONRETVAL;
    if (argc == 1)
    {
        if (nftw(".", dirTree, 10, flags) == -1)
        {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if(!checkifdir(argv[i])){
                printf("%s [error opening dir]\n", argv[i]);
            }
            else if (nftw(argv[i], dirTree, 10, flags) == -1)
            {
                perror("nftw");
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("\n%d directories, %d files\n", numberOfDirectories, numberOfFiles);
    exit(EXIT_SUCCESS);
}



