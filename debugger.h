typedef struct {
	char *name;
  int pid;
} dbgr;

dbgr * debugger(char *, int);
void debug(dbgr *);
void continueExecution(dbgr *);
void handleCommand(dbgr *, char *);

char ** split(char *, char);