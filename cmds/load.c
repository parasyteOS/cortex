#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "tsu.h"
#include "cmds.h"

int load(int tsu_fd, int argc, char *argv[])
{
	int selinux_fd = -1;

	size_t cap = 4096;
	char *buf = malloc(cap * sizeof(char));
	size_t len = 0;
	char *ptr = buf;
	size_t step = cap;
	size_t steplen = 0;
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
	
	while ((steplen = read(STDIN_FILENO, ptr, step)) > 0) {
		len += steplen;
		while (len + step >= cap) {
			cap *= 2;
			buf = realloc(buf, cap * sizeof(char));
		}
		ptr = buf + len;
	}

	write(selinux_fd, buf, len);

	close(selinux_fd);
	return 0;
}
