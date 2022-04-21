#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <sys/syscall.h>
#include <ftw.h>

#define MAX_PATH_LEN 1048576

void tree(char *path ,int level)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        printf("\n");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }
        for (int i = 0; i < level; i++)
        {
            printf("    ");
        }
        if (entry->d_type == DT_DIR)
        {
            printf("%s", entry->d_name);

            //* HERE USUALLY IT FUCKS UP .
            tree(strcat(strcat(path,"/"),(entry->d_name)) ,(level + 1));
        }
        else
        {
            struct stat st;
            stat(entry->d_name, &st);
            printf("[%s", (st.st_mode & S_IRUSR) ? "r" : "-");
            printf("%s", (st.st_mode & S_IWUSR) ? "w" : "-");
            printf("%s", (st.st_mode & S_IXUSR) ? "x" : "-");
            printf("%s", (st.st_mode & S_IRGRP) ? "r" : "-");
            printf("%s", (st.st_mode & S_IWGRP) ? "w" : "-");
            printf("%s", (st.st_mode & S_IXGRP) ? "x" : "-");
            printf("%s", (st.st_mode & S_IROTH) ? "r" : "-");
            printf("%s", (st.st_mode & S_IWOTH) ? "w" : "-");
            printf("%s ", (st.st_mode & S_IXOTH) ? "x" : "-");
            struct passwd *pw = getpwuid(st.st_uid);
            struct group *gr = getgrgid(st.st_gid);
            printf("%s ", pw->pw_name);
            printf("%s ", gr->gr_name);
            printf("%ld] ", st.st_size);
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char const *argv[])
{
    char path[MAX_PATH_LEN];
    if (argc == 1)
    {
        getcwd(path, MAX_PATH_LEN);
    }
    else
    {
        strcpy(path, argv[1]);
    }
    tree(path,1);
    return 0;
}