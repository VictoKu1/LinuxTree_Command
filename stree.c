// Implementation for linux tree command . 




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
#define MAX_BUF_LEN 1048576
#define MAX_ARG_LEN 1048576








// function receives a path as input 
// and prints all the files in the directory and all of its subdirectories what so ever
void tree (char *path)
{

    // open the directory
    DIR *dir = opendir(path);

    // if the directory is not opened
    if (dir == NULL)
    {
        printf("Error opening directory\n");
        return;
    }
    
    // get the directory entry
    struct dirent *entry;

    // while there are entries in the directory
    while ((entry = readdir(dir)) != NULL)
    {
        // if the entry is a directory 
        if (entry->d_type == DT_DIR)
        {
            // if the entry is . or ..
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            else{
            // print the directory name
            printf("%s", entry->d_name);

            // call the function recursively
            tree(entry->d_name);
            }
        }
         
        // if the entry is a file
        else
        {
            //print the file permissions
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
            // print the file owner and group
            struct passwd *pw = getpwuid(st.st_uid);
            struct group *gr = getgrgid(st.st_gid);
            printf("%s ", pw->pw_name);
            printf("%s ", gr->gr_name);
            // print the file size in bytes
            printf("%ld] ", st.st_size);
             // print the file name
            printf("%s\n", entry->d_name);
        }
    }

    // close the directory
    closedir(dir);
}


int main(int argc, char const *argv[])
{
    // reading from argv the path to the directory , if there is no path , ./ is used as default path
    char path[MAX_PATH_LEN];
    if (argc == 1)
    {
        getcwd(path, MAX_PATH_LEN);
    }
    else
    {
        strcpy(path, argv[1]);
    }
    // inputing the path to the function tree 
    tree(path);
    return 0;
}







