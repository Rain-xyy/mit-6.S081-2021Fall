#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[20];
  char* args[32];
  args[0] = malloc(strlen(argv[0]) + 1);
  int arg_count;
  for (int i = 1; i < argc; i++) {
    args[i - 1] = malloc(strlen(argv[i]) + 1);
    strcpy(args[i - 1], argv[i]);
  }

  while (1)
  {
    arg_count = argc - 1;
    char input;
    if(read(0, &input, 1) == 0) {
        break;
    }

    int i = 0;
    while (input != '\n')
    {
        if (input == ' ')
        {
            buf[i] = 0;
            args[arg_count] = malloc(strlen(buf) + 1);
            strcpy(args[arg_count], buf);
            arg_count++;
            i = 0;
        } else {
            buf[i] = input;
            i++;
        }
        read(0, &input, 1);
    }
    buf[i] = 0;
    args[arg_count] = malloc(strlen(buf) + 1);
    strcpy(args[arg_count], buf);
    
    if (fork() == 0)
    {
        // printf("%s\n", args[0]);
        exec(args[0], args);
    } else {
        int status;
        wait(&status);
        // printf("666\n");
        for (int i = argc - 1; i <= arg_count; i++) {
          // printf("args: %s\n", args[i]);
          free(args[i]);
        }
    }
  }
  for (int i = 1; i < argc; i++) {
    free(args[i - 1]);
  }
  
  exit(0);
}
