/**
 * This is a simple debugger written in C.
 * 
 * Author: Satvik Sethia
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include "linenoise/linenoise.h"

void handleCommand(char *command) {

}

void debug(int pid) {
  int status;
  waitpid(pid, &status, 0);

  /* Load history from file. The history file is just a plain text file
    * where entries are separated by newlines. */
  linenoiseHistoryLoad("history.txt"); // Load the history at startup
  
  char* command;
  while ((command = linenoise("dbg> ")) != NULL) {
      handleCommand(command);
      linenoiseHistoryAdd(command);
      linenoiseHistorySave("history.txt"); /* Save the history on disk. */
      free(command);
  }
}

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
}