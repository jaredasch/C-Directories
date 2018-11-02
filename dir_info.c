#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

int dir_size(char *path, char RECURSIVE){
    int size = 0;
    DIR *d = opendir(path);
    struct dirent *f = readdir(d);
    while(f){
        char *path_cpy = malloc(strlen(path) + strlen(f->d_name) + 10);
        strcat(strcpy(path_cpy, path), f->d_name);
        struct stat *s = malloc(sizeof(struct stat));
        stat(path_cpy, s);

        if(RECURSIVE && f->d_type == DT_DIR && strcmp(".", f->d_name) && strcmp("..", f->d_name)){
            size += dir_size(strcat(path_cpy, "/"), RECURSIVE);
        } else {
            size += s->st_size;
        }
        f = readdir(d);
        free(path_cpy);
        free(s);
    }
    closedir(d);
    return size;
}

void list_dir(char *path, int depth){
    DIR *d = opendir(path);
    struct dirent *f = readdir(d);
    while(f){
        char *path_cpy = malloc(strlen(path) + strlen(f->d_name) + 10);
        strcat(strcpy(path_cpy, path), f->d_name);
        struct stat *s = malloc(sizeof(struct stat));
        stat(path_cpy, s);

        char *perms = malloc(10);
        perms[9] = 0;
        for(int i = 0; i < 9; i += 3){
            perms[6 - i] = (s->st_mode >> (2 + i)) % 2 ? 'r' : '-';
            perms[7 - i] = (s->st_mode >> (1 + i)) % 2 ? 'w' : '-';
            perms[8 - i] = (s->st_mode >> (0 + i)) % 2 ? 'x' : '-';
        }

        printf("%c%s %10lld B %.*s %-60s\n", f->d_type == DT_DIR ? 'd' : '-', perms,  s->st_size, depth, "\t\t\t\t\t\t\t\t\t", path_cpy);
        free(perms);
        if(f->d_type == DT_DIR && strcmp(".", f->d_name) && strcmp("..", f->d_name)){
            list_dir(strcat(path_cpy, "/"), depth+1);
        }
        free(path_cpy);
        free(s);
        f = readdir(d);
    }
    closedir(d);
}

int main(){
  // printf("statistics for directory: . \n");
  printf("Total directory size (non-recursive): %d \n", dir_size("./", 0));
  printf("Total directory size (recursive): %d \n", dir_size("./", 1));
  list_dir("./", 0);
  return 0;
}
