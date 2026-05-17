#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "tsu.h"
#include "cmds.h"

int dump(struct sig_payload* sig, int argc, char *argv[])
{
	int selinux_fd = -1;
	char buf[1024];
	size_t buflen;
	int rc = 0;

	prctl(TERMINAL_SU_OPTION, sig, CMD_SEPOL_GETFD, &selinux_fd, &rc);

	if (rc) {
		fprintf(stderr, "prctl failed: %d\n", rc);
		return 1;
	}
	
	if (selinux_fd < 0) {
		fprintf(stderr, "selinux_fd: %d\n", selinux_fd);
		return 1;
	}

	while ((buflen = read(selinux_fd, buf, 1024)) > 0) {
		write(STDOUT_FILENO, buf, buflen);
	}

	close(selinux_fd);
	return 0;
}
