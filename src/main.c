#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_usage(const char *prog_name) {
  printf("Usage: %s [OPTION]... [FILE]...\n", prog_name);
  printf("List directory contents.\n\n");
  printf("Options:\n");
  printf("  -l    use long listing format\n");
  printf("  -a    show hidden files (starting with .)\n");
  printf("  -h    display this help and exit\n");
}

void print_permissions(mode_t mode) {
  printf("%c", S_ISDIR(mode) ? 'd' : '-');
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

void print_long_format(const char *path, const char *filename) {
  struct stat file_stat;
  char full_path[1024];

  if (path && strcmp(path, ".") != 0) {
    snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
  } else {
    strncpy(full_path, filename, sizeof(full_path) - 1);
    full_path[sizeof(full_path) - 1] = '\0';
  }

  if (stat(full_path, &file_stat) == -1) {
    perror("stat");
    return;
  }

  print_permissions(file_stat.st_mode);

  printf(" %2ld", file_stat.st_nlink);

  struct passwd *pwd = getpwuid(file_stat.st_uid);
  struct group *grp = getgrgid(file_stat.st_gid);
  printf(" %s %s", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");

  printf(" %8ld", file_stat.st_size);

  char time_str[20];
  struct tm *tm_info = localtime(&file_stat.st_mtime);
  strftime(time_str, sizeof(time_str), "%b %d %H:%M", tm_info);
  printf(" %s", time_str);

  printf(" %s\n", filename);
}

int should_show_file(const char *filename, int show_hidden) {
  if (!show_hidden && filename[0] == '.') {
    return 0;
  }
  return 1;
}

void list_directory(const char *path, int long_format, int show_hidden) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(path ? path : ".");
  if (dir == NULL) {
    perror("opendir");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (!should_show_file(entry->d_name, show_hidden)) {
      continue;
    }
    
    if (long_format) {
      print_long_format(path, entry->d_name);
    } else {
      printf("%s\n", entry->d_name);
    }
  }

  closedir(dir);
}

int main(int argc, char *argv[]) {
  int long_format = 0;
  int show_hidden = 0;
  int opt;

  while ((opt = getopt(argc, argv, "lah")) != -1) {
    switch (opt) {
      case 'l':
        long_format = 1;
        break;
      case 'a':
        show_hidden = 1;
        break;
      case 'h':
        print_usage(argv[0]);
        return 0;
      default:
        print_usage(argv[0]);
        return 1;
    }
  }

  if (optind >= argc) {
    list_directory(".", long_format, show_hidden);
  } else {
    for (int i = optind; i < argc; i++) {
      struct stat path_stat;

      if (stat(argv[i], &path_stat) == -1) {
        perror(argv[i]);
        continue;
      }

      if (S_ISDIR(path_stat.st_mode)) {
        if (argc - optind > 1) {
          printf("%s:\n", argv[i]);
        }
        list_directory(argv[i], long_format, show_hidden);
        if (i < argc - 1) {
          printf("\n");
        }
      } else {
        if (long_format) {
          print_long_format(".", argv[i]);
        } else {
          printf("%s\n", argv[i]);
        }
      }
    }
  }

  return 0;
}
