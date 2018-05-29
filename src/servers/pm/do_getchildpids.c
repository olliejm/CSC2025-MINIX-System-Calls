#include <stdio.h>
#include "pm.h"             // for glo.h:  mp, call_nr, who_p etc.
#include "mproc.h"          // for proc table

// helper method to determine if process at given index is a child of pid
int _idx_is_nonzero_child_of(pid_t pid, int i) {
	// if the PID of the parent of process at current index equals given pid
	// and the pid of the process at current index is not 0
	if (mproc[mproc[i].mp_parent].mp_pid == pid &&
		mproc[i].mp_pid != 0) {
		return 1;
	} return 0;
}

// system call for library function printchildpids
int do_printchildpids() {
	// receive PID from message
	pid_t pid = m_in.m1_i1;

	printf("Searching for children of process: %d\n", pid);

	// iterate through mproc, if process at index is child of pid, print
	for (int i = 0; i < NR_PROCS; i++) {
		if (_idx_is_nonzero_child_of(pid, i)) {
			printf("%d\n", mproc[i].mp_pid);
		}
	}
	
	// success
	return 0;
}

// system call for library function getnchildren
int do_getnchildren() {
	// receive PID from message and initialise child count
	pid_t pid = m_in.m1_i1;
	int nchildren = 0;

	// iterate through mproc, increase count if process at i is child of pid
	for (int i = 0; i < NR_PROCS; i++) {
		if (_idx_is_nonzero_child_of(pid, i)) {
			nchildren++;
		}
	}
	
	// return count
	return nchildren;
}

// system call for library function getchildpids
int do_getchildpids() {
	// receive PID and max no of children to find from message
	pid_t pid = m_in.m1_i1;
	int nchildren = m_in.m1_i2;

	// if the requested number is more than number of processes, set equal
	if (nchildren > NR_PROCS) {
		nchildren = NR_PROCS;
	}

	// initialise array to store child pids
	pid_t childpids[nchildren];

	// initialise counter for number found
	int nfound = 0;

	// while there are still more to be found, and end of mproc not reached
	for (int i = 0; nchildren && i < NR_PROCS; i++) {
		// add to array if idx is a child, decrease to find, increase found
		if (_idx_is_nonzero_child_of(pid, i)) {
			childpids[nfound] = mproc[i].mp_pid;
			nchildren--;
			nfound++;
		}
	}

	// if there was children found, and copying fails, return -1
	if (nfound &&
		sys_vircopy(SELF,
			(vir_bytes) childpids,
			who_e,
			m_in.m1_i3,
			sizeof(pid_t) * nfound) != OK) {
		return -1;
	}

	// return num found
	return nfound;
}