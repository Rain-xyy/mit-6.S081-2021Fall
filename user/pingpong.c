#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int fds[2];
  if (pipe(fds) < 0) {
      fprintf(2, "pipe creation failed");
      exit(1);
  }

  int pid = fork();
  if (pid == 0){
    pid = getpid();
    char c;
    read(fds[0], &c, 1);
    printf("%d: received ping\n", pid);
    write(fds[1], "x", 1);
    close(fds[0]);
    close(fds[1]);
  } else {
    pid = getpid();
    char c;
    write(fds[1], "x", 1);
    read(fds[0], &c, 1);
    printf("%d: received pong\n", pid);
    close(fds[0]);
    close(fds[1]);
  }
  exit(0);
}
