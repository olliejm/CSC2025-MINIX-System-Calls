#include <lib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: %s <pid> <nchildren>\n", argv[0]);
		return -1;
	}

	pid_t pid = atoi(argv[1]);
	int nchildren = atoi(argv[2]);
	pid_t childpids[nchildren];

	int found = getnchildren(pid);
	printf("GETNCHILDREN returned: %d\n", found);

	found = getchildpids(pid, nchildren, childpids);
	printf("GETCHILDPIDS returned: %d\n", found);

	printf("GETCHILDPIDS filled array print:\n");
	for (int i = 0; i < found; i++) {
		printf("PID %d: %d\n", i+1, childpids[i]);
	}

	printf("PRINTCHILDPIDS (check vm console)\n");
	printchildpids(pid);
}