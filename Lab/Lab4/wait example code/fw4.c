#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

main()
{
  int i, j, status;

  i = fork();

  if (i > 0) {
    sleep(4);
    system("ps aux | grep fw");
    sleep(1);
    printf("\n");
    j = wait(&status);
    printf("Parent: Child done.\n");
    printf("  Return value: %d\n", j);
    printf("  Status:       %d\n", status);
    printf("  WIFSTOPPED:   %d\n", WIFSTOPPED(status));
    printf("  WIFSIGNALED:  %d\n", WIFSIGNALED(status));
    printf("  WIFEXITED:    %d\n", WIFEXITED(status));
    printf("  WEXITSTATUS:  %d\n", WEXITSTATUS(status));
    printf("  WTERMSIG:     %d\n", WTERMSIG(status));
    printf("  WSTOPSIG:     %d\n", WSTOPSIG(status));
  } else {
    printf("Child (%d) calling exit(4)\n", getpid());
    exit(4);
  }
}

