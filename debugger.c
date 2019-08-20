#include <string.h>
#include <sys/ptrace.h>
#include "debugger.h"


dbgr * debugger(char *name, int pid) {
	dbgr *this = malloc(sizeof(dbgr));

	this->name = malloc(strlen(name) + 1);
  strcpy(this->name, name);
  this->pid = pid;

  return this;
}

void debug(dbgr *this) {
  int status;
  waitpid(this->pid, &status, 0);

  // Load history from file. The history file is just a plain text file
  // where entries are separated by newlines.
  linenoiseHistoryLoad("history.txt");

  char* command;
  while ((command = linenoise("dbg> ")) != NULL) {
      handleCommand(this->pid, command);
      linenoiseHistoryAdd(command);
      linenoiseHistorySave("history.txt"); // Save the history on disk.
      free(command);
  }
}

void continueExecution(int pid) {
  ptrace(PTRACE_CONT, pid, NULL, NULL);

  int status;
  waitpid(pid, &status, 0);
}

void handleCommand(int pid, char *command) {
  char **commands = split(command, ' ');
  if (commands[0][0] == 'c') {
    continueExecution(pid);
  } else {
    fprintf(stderr, "Unknown command.\n");
  }
}

char ** split(char *command, char delim) {
  int num_words = 1;
  char *iter = command;

  while (*iter) {
    iter++;
    if (*iter == delim) {
      if (iter[1] != '\0' || iter[1] != delim) {
        num_words++;
      }
    }
  }

  char **splitted = calloc(num_words + 1, sizeof(char *));
  char *copy = strtok(command, &delim);
  splitted[0] = calloc(strlen(copy) + 1, 1);
  strcpy(splitted[0], copy);

  for (int i = 1; i < num_words; i++) {
    copy = strtok(NULL, &delim);
    splitted[i] = calloc(strlen(copy) + 1, 1);
    strcpy(splitted[i], copy);
  }

  return splitted;
}
