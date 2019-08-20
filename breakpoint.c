#include <stddef.h>
#include <sys/ptrace.h>
#include "breakpoint.h"


breakp * breakpoint(pid_t pid, __intptr_t address) {
	breakp *this = malloc(sizeof(breakp));

	this->pid = pid;
	this->addr = address;
	this->enabled = 0;
	this->data = 0;

	return this;
}

void enable(breakp* this) {
  long data = ptrace(PTRACE_PEEKDATA, this->pid, this->addr, NULL);
  this->data = (u_int8_t) (data & 0xff);
  long int3 = 0xcc;
  long data_int3 = ((data & ~0xff) | int3);
  ptrace(PTRACE_POKEDATA, this->pid, this->addr, data_int3);
  this->enabled = 1;
}

void disable(breakp* this) {
  long data = ptrace(PTRACE_PEEKDATA, this->pid, this->addr, NULL);
  long old_data = ((data & ~0xff) | this->data);
  ptrace(PTRACE_POKEDATA, this->pid, this->addr, old_data);
  this->enabled = 0;
}
