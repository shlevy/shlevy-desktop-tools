#include <unistd.h>
#include <X11/Xlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char ** argv) {
  Display * dpy = XOpenDisplay(NULL);
  Window w = DefaultRootWindow(dpy);
  int fd = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY);
  while (1) {
    char buf[512];
    time_t tm = time(NULL);
    size_t off = strftime(buf, sizeof buf, "%r  |  %F  |  ", localtime(&tm));

    ssize_t res = pread(fd, buf + off, sizeof buf - off, 0);
    if (res == -1) {
      perror("reading battery capacity");
      XStoreName(dpy, w, "Error!");
      XFlush(dpy);
      return 1;
    }

    buf[off + res - 1] = '%';
    buf[off + res] = '\0';

    XStoreName(dpy, w, buf);
    XFlush(dpy);

    struct timespec req = { .tv_sec = 0, .tv_nsec = 100000000L };
    nanosleep(&req, NULL);
  }
}
