#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

int dir_size(char *path){
    int count = 0;
    DIR * d = opendir(path);
    struct dirent * f = readdir(d);
    while(f){
        char *path_cpy = malloc(strlen(path));
        strcat(strcpy(path_cpy, path), f->d_name);
        struct stat *s = malloc(sizeof(struct stat));
        stat(path_cpy, s);

        struct stat *b = malloc(sizeof(struct stat));
        stat(f->d_name,b);
        count += b->st_size;
        f = readdir(d);
    }
    return count;
}

void list_dir(char *path){
    DIR *d = opendir(path);
    struct dirent *f = readdir(d);
    while(f){
        char *path_cpy = malloc(100);
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

        printf("%c%s \t %-60s \t\t %5lld B\n", f->d_type == DT_DIR ? 'd' : '-', perms, path_cpy, s->st_size);
        free(perms);
        if(f->d_type == DT_DIR && strcmp(".", f->d_name) && strcmp("..", f->d_name)){
            list_dir(strcat(path_cpy, "/"));
        }
        free(path_cpy);
        free(s);
        f = readdir(d);
    }
    closedir(d);
}

int main(){
  // printf("statistics for directory: . \n");
  printf("total directory size: %d \n", dir_size("./"));
  list_dir("./");
  return 0;
}
