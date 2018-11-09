#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/file.h>
#include <zconf.h>
#include <memory.h>

// Example 1 - File
// ./main boring.txt
// File type:                regular file

// Example 2 - Directory
// ./main .
// File type:                directory

void printFileType(struct stat *fileStat);

void printFileSize(struct stat *fileStat);

int isReadable(struct stat *fileStat);

int isWritable(struct stat *fileStat);

int isExecutable(struct stat *fileStat);

int isDirectory(struct stat *fileStat);

void showDirContents(const char *pathName);

int main(int argc, char **argv) {
    if (argc != 2) {
        showDirContents(".");
        exit(EXIT_SUCCESS);
    }

    char *pathName = argv[1];
    // printf("<Debug> File name: %s\n", pathName);

    if (access(pathName, R_OK) != -1) {
        printf("Read permissions OK!\n");
    } else {
        printf("No read permissions!\n");
    }

    struct stat fileStat;
    if (stat(pathName, &fileStat) == -1) { // Call stat() and save results to fileStat
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printFileType(&fileStat);
    printFileSize(&fileStat);
    printf("Can the owner read?       %s\n", isReadable(&fileStat) ? "true" : "false");
    printf("Can the owner write?      %s\n", isWritable(&fileStat) ? "true" : "false");
    printf("Can the owner execute?    %s\n", isExecutable(&fileStat) ? "true" : "false");

    printf("Is directory?             %s\n", isDirectory(&fileStat) ? "true" : "false");

    if (isDirectory(&fileStat)) {
        showDirContents(pathName);

    }

    return 0;
}

void showDirContents(const char *pathName) {
    DIR *dirPtr;
    dirPtr = opendir(pathName);

    printf("Contents:\n");

    struct dirent *dirEntry;
    char *fileName;
    while ((dirEntry = readdir(dirPtr)) != NULL) {
            fileName = dirEntry->d_name;
            if (strncmp(fileName, ".", strlen(".")) != 0) { // Don't show if file starts with a dot
                printf("  %s\n", dirEntry->d_name);
            }
    }
    closedir(dirPtr);
}

int isReadable(struct stat *fileStat) {
    if ((*fileStat).st_mode & S_IRUSR) {
        return 1;
    } else {
        return 0;
    }
}

int isWritable(struct stat *fileStat) {
    if ((*fileStat).st_mode & S_IWUSR) {
        return 1;
    } else {
        return 0;
    }
}

int isExecutable(struct stat *fileStat) {
    if ((*fileStat).st_mode & S_IXUSR) {
        return 1;
    } else {
        return 0;
    }
}

int isDirectory(struct stat *fileStat) {
    if (((*fileStat).st_mode & S_IFMT) == S_IFDIR) {
        return 1;
    } else {
        return 0;
    }
}

void printFileSize(struct stat *fileStat) {
    printf("File size:                %lld bytes\n",
           (long long) (*fileStat).st_size);
}

void printFileType(struct stat *fileStat) {
    printf("File type:                ");

    switch ((*fileStat).st_mode & S_IFMT) { // Compare with type of file mask
        case S_IFBLK:
            printf("block device\n");
            break;
        case S_IFCHR:
            printf("character device\n");
            break;
        case S_IFDIR:
            printf("directory\n");
            break;
        case S_IFIFO:
            printf("FIFO/pipe\n");
            break;
        case S_IFLNK:
            printf("symlink\n");
            break;
        case S_IFREG:
            printf("regular file\n");
            break;
        case S_IFSOCK:
            printf("socket\n");
            break;
        default:
            printf("unknown?\n");
            break;
    }
}