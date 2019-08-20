/**
 * This is a simple debugger written in C.
 *
 * Author: Satvik Sethia
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include "linenoise/linenoise.h"
#include "breakpoint.h"

int main (int argc, char *argv[]) {
  // Check if name of program to debug specified
	if (argc < 2) {
    printf("Enter a program name to debug!");
    exit(1);
  }

  char *name = argv[1];
  pid_t pid = fork();

  if (!pid) {
    // In the child process
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execl(name, name, NULL);
  } else {
    debug(pid);
  }

  return 0;
}