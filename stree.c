// Linux/Unix headers - ignore include errors on Windows
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

/* Constants */
#define MAX_DEPTH 64
#define BUFFER_SIZE 1024
#define MAX_PATH_LEN 4096

/* Tree statistics structure */
typedef struct
{
    int files;
    int directories;
    int errors;
} tree_stats_t;

/* Tree context structure */
typedef struct
{
    tree_stats_t *stats;
    int max_depth;
    int *arr; /* Array to track remaining entries at each level */
} tree_context_t;

/**
 * Check if a path is a directory
 * @param path Path to check
 * @return 1 if directory, 0 otherwise
 */
static int is_directory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1)
    {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}

/**
 * Get file type character for display
 * @param mode File mode
 * @return Character representing file type
 */
static char get_file_type_char(mode_t mode)
{
    switch (mode & S_IFMT)
    {
    case S_IFREG:
        return '-';
    case S_IFDIR:
        return 'd';
    case S_IFCHR:
        return 'c';
    case S_IFBLK:
        return 'b';
    case S_IFLNK:
        return 'l';
    case S_IFIFO:
        return 'p';
    case S_IFSOCK:
        return 's';
    default:
        return '?';
    }
}

/**
 * Print file permissions in rwx format
 * @param mode File mode
 */
static void print_permissions(mode_t mode)
{
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

/**
 * Print user and group information safely
 * @param uid User ID
 * @param gid Group ID
 */
static void print_owner_info(uid_t uid, gid_t gid)
{
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);

    printf("%-8s ", pw ? pw->pw_name : "unknown");
    printf("%-8s ", gr ? gr->gr_name : "unknown");
}

/**
 * Count visible entries in a directory (original helper function logic)
 * @param path Directory path
 * @param level Current level
 * @param arr Array to store counts
 */
static void count_visible_entries(const char *path, int level, int *arr)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
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

/**
 * File tree walk callback function
 * @param pathname Path to current file/directory
 * @param sbuf File status information
 * @param type Type of file
 * @param ftwb File tree walk buffer
 * @return 0 on success, non-zero on error
 */
static int dir_tree_callback(const char *pathname, const struct stat *sbuf,
                             int type, struct FTW *ftwb)
{
    tree_context_t *context = (tree_context_t *)ftwb->data;

    /* Skip hidden files and directories */
    if (strstr(pathname, "/.") != NULL)
    {
        return FTW_SKIP_SUBTREE;
    }

    /* Handle root directory */
    if (ftwb->level == 0)
    {
        printf("%s\n", pathname);
        count_visible_entries(&pathname[ftwb->base], ftwb->level + 1, context->arr);
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
                context->stats->files++;
            }
            if (type == FTW_D)
            {
                count_visible_entries(&pathname[ftwb->base], ftwb->level + 1, context->arr);
                context->stats->directories++;
            }

            /* Print tree prefix exactly like original */
            int level = ftwb->level;
            int index = 1;
            while (level > 1)
            {
                if (context->arr[index] > 0)
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

            if (context->arr[ftwb->level] == 1)
            {
                printf("└──");
            }
            else
            {
                printf("├──");
            }

            printf(" [");
            context->arr[ftwb->level]--;

            /* Print file type */
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

            /* Print permissions */
            printf("%s", (sbuf->st_mode & S_IRUSR) ? "r" : "-");
            printf("%s", (sbuf->st_mode & S_IWUSR) ? "w" : "-");
            printf("%s", (sbuf->st_mode & S_IXUSR) ? "x" : "-");
            printf("%s", (sbuf->st_mode & S_IRGRP) ? "r" : "-");
            printf("%s", (sbuf->st_mode & S_IWGRP) ? "w" : "-");
            printf("%s", (sbuf->st_mode & S_IXGRP) ? "x" : "-");
            printf("%s", (sbuf->st_mode & S_IROTH) ? "r" : "-");
            printf("%s", (sbuf->st_mode & S_IWOTH) ? "w" : "-");
            printf("%s ", (sbuf->st_mode & S_IXOTH) ? "x" : "-");

            /* Print owner info */
            struct passwd *pw = getpwuid(sbuf->st_uid);
            struct group *gr = getgrgid(sbuf->st_gid);
            printf("%s ", pw ? pw->pw_name : "unknown");
            printf("%s ", gr ? gr->gr_name : "unknown");
            printf("%11ld", (long)sbuf->st_size);
            printf("]  %s\n", &pathname[ftwb->base]);
        }
    }
    return 0;
}

/**
 * Print usage information
 * @param program_name Name of the program
 */
static void print_usage(const char *program_name)
{
    printf("Usage: %s [OPTIONS] [PATH...]\n", program_name);
    printf("Display directory tree structure with file information\n\n");
    printf("Options:\n");
    printf("  -h, --help     Display this help message\n");
    printf("  -d, --depth N  Limit tree depth to N levels (default: 10)\n");
    printf("\nIf no PATH is specified, current directory is used.\n");
}

/**
 * Main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit status
 */
int main(int argc, char *argv[])
{
    tree_stats_t stats = {0, 0, 0};
    int arr[MAX_DEPTH] = {0}; /* Local array instead of global */
    tree_context_t context = {
        .stats = &stats,
        .max_depth = 10,
        .arr = arr};

    int flags = FTW_CHDIR | FTW_ACTIONRETVAL;

    if (argc == 1)
    {
        if (nftw(".", dir_tree_callback, 10, flags) == -1)
        {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (!is_directory(argv[i]))
            {
                printf("%s [error opening dir]\n", argv[i]);
            }
            else if (nftw(argv[i], dir_tree_callback, 10, flags) == -1)
            {
                perror("nftw");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("\n%d directories, %d files\n", stats.directories, stats.files);
    exit(EXIT_SUCCESS);
}
