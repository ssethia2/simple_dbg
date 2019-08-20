#include <sys/types.h>


typedef struct {
	pid_t pid;
	__intptr_t addr;
	int enabled;
	u_int8_t data;
} breakp;

breakp * breakpoint(pid_t pid, __intptr_t address);
void enable(breakp *);
void disable(breakp *);