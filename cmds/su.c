#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>

#include "tsu.h"
#include "cmds.h"

int su(struct sig_payload* sig, int argc, char *argv[])
{
	int rc = 0;
	struct tsu_string se_context = {
		.ptr = "u:r:terminator:s0",
	};
	se_context.len = strlen(se_context.ptr) + 1;

	if (argc < 2) {
		printf("Usage: su <commands>\n");
		return 1;
	}

	prctl(TERMINAL_SU_OPTION, sig, CMD_TRANSFORM, &se_context, &rc);

	if (rc) {
		perror("prctl failed");
		return 1;
	}

	rc = execvp(argv[1], argv + 1);
	if (rc)
		perror("execvp");
	return rc;
}
