#include "kernel/types.h"
#include "user/user.h"

void primes(int parent_fd);

int
main(int argc, char *argv[])
{
  int fds[2];
  pipe(fds);
  if (fork() == 0)
  {
    close(fds[1]);
    primes(fds[0]);
  } else {
    close(fds[0]);
    for (int i = 2; i < 36; i++)
    {
      write(fds[1], &i, sizeof(int));
    }
    close(fds[1]);
    int status;
    wait(&status);
  }
  exit(0);
}

void primes(int parent_fd) {
  int base;
  if (read(parent_fd, &base, sizeof(int)) == 0)
  {
    close(parent_fd);
    exit(0);
  }
  
  int child_fds[2];
  pipe(child_fds);
  if (fork() == 0) {
    close(child_fds[1]);
    primes(child_fds[0]);
  } else {
    close(child_fds[0]);
    printf("prime %d\n", base);
    int key;
    while (read(parent_fd, &key, sizeof(int)) != 0)
    {
        if (key % base != 0) {
            write(child_fds[1], &key, sizeof(int));
        }
    }
    close(parent_fd);
    close(child_fds[1]);
    int status;
    wait(&status);
  }  
}
