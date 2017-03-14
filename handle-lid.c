#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main (int argc, char ** argv) {
  DIR * evdir = opendir("/dev/input");
  if (!evdir) {
    perror("Opening input dir");
    return 1;
  }
  int evdir_fd = dirfd(evdir);
  int found = 0;
  int ev_fd;
  struct dirent * ent;
  while ((ent = readdir(evdir))) {
    if (strncmp("event", ent->d_name, strlen("event")) == 0) {
      ev_fd = openat(evdir_fd, ent->d_name, O_RDONLY);
      if (ev_fd != -1) {
        char name[256] = "";
        if (ioctl(ev_fd, EVIOCGNAME(256), name) != -1) {
          if (strcmp(name, "Lid Switch") == 0) {
            found = 1;
            break;
          }
        }
        close(ev_fd);
      }
    }
  }
  if (!found) {
    fprintf(stderr, "No lid switch\n");
    return 2;
  }

  closedir(evdir);

  struct input_event ev;
  ssize_t read_res;
  while ((read_res = read(ev_fd, &ev, sizeof ev)) != 0) {
    if (read_res == -1) {
      perror("reading from switch event");
      continue;
    }
    if (ev.type == EV_SW && ev.code == SW_LID && ev.value == 1) {
      pid_t child = fork();
      if (child == -1) {
        perror("forking");
      } else if (child == 0) {
        execlp("dm-tool", "dm-tool", "lock", NULL);
        perror("executing dm-tool");
        _exit(1);
      } else {
        while (waitpid(child, NULL, 0) == -1) {
          perror("waiting for dm-tool");
          if (errno == ECHILD) {
            break;
          }
        }
      }
    }
  }
  fprintf(stderr, "Unexpected EOF reading from switch event\n");
  return 3;
}
