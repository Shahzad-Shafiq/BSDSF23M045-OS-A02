/*
* Programming Assignment 02: ls-v1.1.0
* Feature: Add -l (long listing format)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>        // NEW (for getpwuid)
#include <grp.h>        // NEW (for getgrgid)
#include <time.h>       // NEW (for ctime)
#include <sys/ioctl.h>
#include <termios.h>     // 

#define MODE_DEFAULT 0
#define MODE_LONG 1
#define MODE_HORIZONTAL 2

extern int errno;

void do_ls(const char *dir, int display_mode);
void do_ls_long(const char *dir);
void display_horizontal(char **names, int count, int maxlen);
void display_vertical(char **names, int count, int maxlen);
int compare_names(const void *a, const void *b);

int main(int argc, char *argv[])
{
    int opt;
    int display_mode = MODE_DEFAULT;

    while ((opt = getopt(argc, argv, "lx")) != -1)
    {
        switch (opt)
        {
        case 'l':
            display_mode = MODE_LONG;
            break;
        case 'x':
            display_mode = MODE_HORIZONTAL;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l | -x] [directory...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind == argc)
    {
        if (display_mode == MODE_LONG)
            do_ls_long(".");
        else
            do_ls(".", display_mode);
    }
    else
    {
        for (int i = optind; i < argc; i++)
        {
            printf("Directory listing of %s:\n", argv[i]);
            if (display_mode == MODE_LONG)
                do_ls_long(argv[i]);
            else
                do_ls(argv[i], display_mode);
            puts("");
        }
    }

    return 0;
}



void mode_to_letters(mode_t mode, char str[])
{
    strcpy(str, "----------");

    // File type
    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISSOCK(mode)) str[0] = 's';
    else if (S_ISFIFO(mode)) str[0] = 'p';

    // User permissions
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    // Group permissions
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    // Others permissions
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';

    str[10] = '\0';
}

void print_long_format(const char *path, const char *filename)
{
    struct stat info;
    if (lstat(path, &info) == -1)
    {
        perror("lstat");
        return;
    }

    char mode_str[11];
    mode_to_letters(info.st_mode, mode_str);

    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);

    char *time_str = ctime(&info.st_mtime);
    time_str[strlen(time_str) - 1] = '\0'; 

    printf("%s %3lu %-8s %-8s %8ld %s %s\n",
           mode_str,
           info.st_nlink,
           pw ? pw->pw_name : "unknown",
           gr ? gr->gr_name : "unknown",
           info.st_size,
           time_str + 4, 
           filename);
}

void do_ls(const char *dir, int display_mode)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    char **names = NULL;
    int count = 0;
    int capacity = 100;
    names = malloc(capacity * sizeof(char *));
    if (names == NULL)
    {
        perror("malloc");
        closedir(dp);
        return;
    }

    int maxlen = 0;
    errno = 0;

    // --- Read all filenames into memory ---
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // skip hidden files

        if (count >= capacity)
        {
            capacity *= 2;
            names = realloc(names, capacity * sizeof(char *));
            if (names == NULL)
            {
                perror("realloc");
                closedir(dp);
                return;
            }
        }

        names[count] = strdup(entry->d_name);
        if (names[count] == NULL)
        {
            perror("strdup");
            closedir(dp);
            return;
        }

        int len = strlen(entry->d_name);
        if (len > maxlen)
            maxlen = len;

        count++;
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);

    if (count == 0)
    {
        free(names);
        return;
    }

    // --- FEATURE 5: Alphabetical Sorting using qsort ---
    qsort(names, count, sizeof(char *), compare_names);

    // --- Display mode selection ---
    if (display_mode == MODE_LONG)
        do_ls_long(dir);
    else if (display_mode == MODE_HORIZONTAL)
        display_horizontal(names, count, maxlen);
    else
        display_vertical(names, count, maxlen);

    // --- Free dynamically allocated memory ---
    for (int i = 0; i < count; i++)
        free(names[i]);
    free(names);
}

int compare_names(const void *a, const void *b)
{
    const char *nameA = *(const char **)a;
    const char *nameB = *(const char **)b;
    return strcmp(nameA, nameB);
}

void display_horizontal(char **names, int count, int maxlen)
{
    struct winsize ws;
    int term_width = 80; // fallback
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        term_width = ws.ws_col;

    int spacing = 2;
    int col_width = maxlen + spacing;
    int current_width = 0;

    for (int i = 0; i < count; i++)
    {
        if (current_width + col_width > term_width)
        {
            printf("\n");
            current_width = 0;
        }

        printf("%-*s", col_width, names[i]);
        current_width += col_width;
    }
    printf("\n");
}

void do_ls_long(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);

    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        print_long_format(path, entry->d_name);
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}

void display_vertical(char **names, int count, int maxlen)
{
    struct winsize ws;

    // --- Get terminal width ---
    int term_width = 80; // default fallback width
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        term_width = ws.ws_col;

    int spacing = 2;
    int col_width = maxlen + spacing;
    int cols = term_width / col_width;
    if (cols < 1) cols = 1;

    int rows = (count + cols - 1) / cols; // ceiling division

    // --- Print "down then across" ---
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int index = c * rows + r;
            if (index < count)
                printf("%-*s", col_width, names[index]);
        }
        printf("\n");
    }
}

