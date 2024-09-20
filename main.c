#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>

#include "tsu.h"
#include "sign.h"
#include "cmds.h"

static int tsu_getfd()
{
	int fd, rc;	
	struct sig_payload *sig = gen_sig();
	if (!sig)
		return -1;

	rc = prctl(TERMINAL_SU_OPTION, sig, CMD_GETFD, 0, &fd);
	free_sig(sig);

	return fd;	
}

static int eq(const char *str1, const char *str2)
{
	return strcmp(str1, str2) == 0;
}

int main(int argc, char *argv[])
{
	int tsu_fd = -1;
	int rc = 0;
	const char* cmd;
	int cmd_argc;
	char **cmd_argv;

	if (argc < 3)
		return 1;

	rc = init_signer(argv[1]);
	if (rc)
		return rc;

	tsu_fd = tsu_getfd();
	fprintf(stderr, "tsu_fd: %d\n", tsu_fd);
	if (tsu_fd < 0) {
		rc = tsu_fd;
		goto cleanup;
	}

	cmd = argv[2];
	cmd_argc = argc - 2;
	cmd_argv = argv + 2;

	if (eq(cmd, "dump")) {
		rc = dump(tsu_fd, cmd_argc, cmd_argv);
	}
	else if (eq(cmd, "load")) {
		rc = load(tsu_fd, cmd_argc, cmd_argv);
	}
	else if (eq(cmd, "su")) {
		rc = su(tsu_fd, cmd_argc, cmd_argv);
	}

	close(tsu_fd);
cleanup:
	free_signer();
	return rc;
}
