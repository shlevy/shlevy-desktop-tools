#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

typedef enum {
  exit_success = 0,
  exit_mem = 1,
  exit_open_archives = 2,
  exit_rename = 3
} exit_code;

int main(int argc, char ** argv) {
  int archive_fd = open("/home-persistent/shlevy/mail/maildir/Archives/cur", O_PATH | O_DIRECTORY | O_CLOEXEC);
  if (archive_fd == -1) {
    perror("opening archives dir");
    return exit_open_archives;
  }
  for (int i = 1; i < argc; ++i) {
    char *path = strdup(argv[i]);
    if (!path) {
      perror("copying argument");
      return exit_mem;
    }
    char * dir = dirname(path);
    if (strcmp(dir, "/home-persistent/shlevy/mail/maildir/INBOX/cur") != 0 && strcmp(dir, "/home-persistent/shlevy/mail/maildir/INBOX/new") != 0)
      continue;
    strcpy(path, argv[i]);
    char * base = basename(path);
    char * u_begin = NULL;
    size_t base_sz = strlen(base) + 1;
    for (size_t j = 0; j < base_sz; ++j) {
      if (u_begin) {
	if (base[j] < '0' || base[j] > '9') {
	  memmove(u_begin, base + j, base_sz - j);
	  break;
	}
      } else if (base[j] == ',' && base[j + 1] == 'U' && base[j + 2] == '=') {
	u_begin = base + j;
	j += 3;
      }
    }
    int res = renameat(AT_FDCWD, argv[i], archive_fd, base);
    if (res == -1) {
      fprintf(stderr, "renaming %s to /home-persistent/shlevy/mail/maildir/Archives/cur/%s: %s\n", argv[i], base, strerror(errno));
      return exit_rename;
    }
    free(path);
  }

  return exit_success;
}
