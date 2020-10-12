#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <stdlib.h>

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid;
  int status;
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort"};
  pipe(pipefd);
  pipe(pipefd2);
  pid = fork();
  if (pid == 0)
    {
      dup2(pipefd[0], STDIN_FILENO);
      dup2(pipefd2[1], STDOUT_FILENO);
      close(pipefd[1]);
      close(pipefd2[0]);
      execvp("grep", grep_args);
      exit(0);
    }
  else
    {
      pid = fork(); 
      if(pid == 0)
      {
        dup2(pipefd2[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        close(pipefd2[1]);
        execvp("sort", sort_args);
        exit(0);
      } else {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execvp("cat", cat_args);
        wait(&status);
        wait(&status);
        exit(0);
        }
    }
  exit(0);
}