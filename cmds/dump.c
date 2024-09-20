#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "tsu.h"
#include "cmds.h"

int dump(int tsu_fd, int argc, char *argv[])
{
	int selinux_fd = -1;
	char buf[1024];
	size_t buflen;
	int rc;

	rc = ioctl(tsu_fd, TSU_IOCTL_SEPOL_GETFD, &selinux_fd);

	if (rc) {
		perror("ioctl failed");
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
