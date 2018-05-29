#include <lib.h>
#include <unistd.h>
#include <errno.h>

int printchildpids(pid_t pid) {
	if (pid < 1) {
		errno = EINVAL;
		return -1;
	}

	message m;
	m.m1_i1 = pid;

	return _syscall(PM_PROC_NR, PRINTCHILDPIDS, &m);
}

int getnchildren(pid_t pid) {
	if (pid < 1) {
		errno = EINVAL;
		return -1;
	}

	message m;
	m.m1_i1 = pid;

	return _syscall(PM_PROC_NR, GETNCHILDREN, &m);
}
        
int getchildpids(pid_t pid, int nchildren, pid_t *childpids) {
	if (pid < 1 || nchildren < 1) {
		errno = EINVAL;
		return -1;
	}

	message m;
	m.m1_i1 = pid;
	m.m1_i2 = nchildren;
	m.m1_i3 = (vir_bytes) childpids;

	return _syscall(PM_PROC_NR, GETCHILDPIDS, &m);
}