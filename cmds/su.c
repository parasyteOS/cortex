#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "tsu.h"
#include "cmds.h"

int su(int tsu_fd, int argc, char *argv[])
{
	int rc;
	struct tsu_string se_context = {
		.ptr = "u:r:terminator:s0",
	};
	se_context.len = strlen(se_context.ptr) + 1;

	if (argc < 2) {
		printf("Usage: su <commands>\n");
		return 1;
	}

	rc = ioctl(tsu_fd, TSU_IOCTL_TRANSFORM, &se_context);

	if (rc) {
		perror("ioctl");
		return 1;
	}

	rc = execvp(argv[1], argv + 1);
	if (rc)
		perror("execvp");
	return rc;
}
