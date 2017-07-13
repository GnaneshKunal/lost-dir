#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    DIR *dp;
    struct dirent *dirp;
    char name[PATH_MAX];
    char directory[PATH_MAX];
    char *slash;
    char cmd[PATH_MAX];
    char details[PATH_MAX];
    char new_name[PATH_MAX];

    if (argc != 2) {
        printf("usage %s <directory name>\n", argv[0]);
        exit(1);
    }

    getcwd(directory, PATH_MAX);
    strcat(directory, "/");
    if ((dp = opendir(argv[1])) == NULL) {
        printf("can't open %s\n", argv[1]);
        exit(1);
    }

    strcat(strcat(directory, argv[1]), "/");
    slash = strrchr(directory, '/') + 1;
    while ((dirp = readdir(dp)) != NULL) {
        strcpy(name, dirp->d_name);
        if ((strcmp(name, ".") == 0) || (strcmp(name, "..") == 0))
            continue;
        strcpy(slash, name);
        FILE *fp = NULL;
        sprintf(cmd, "file '%s'", directory);
        fp = popen(cmd, "r");
        if (fp == NULL) {
            perror("popen");
            exit(1);
        } else {
            fgets(details, PATH_MAX, fp);
            pclose(fp);
            strcpy(new_name, directory);
            if (strstr(details, ".MOV") != NULL)
                strcat(new_name, ".mov");
            else if (strstr(details, "JPEG") != NULL)
                strcat(new_name, ".jpg");
            else if (strstr(details, "MP4") != NULL)
                strcat(new_name, ".mp4");
            else if (strstr(details, "MPEG") != NULL)
                strcat(new_name, ".mpeg");
            if (rename(directory, new_name) == -1) {
                perror("Error");
                exit(1);
            }
        }
    }
    puts("Done!");
    closedir(dp);
    exit(0);
}
