#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "tsu.h"
#include "sign.h"
#include "cmds.h"

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

	struct sig_payload *sig = gen_sig();
	if (!sig) {
		rc = 1;
		goto cleanup;
	}

	cmd = argv[2];
	cmd_argc = argc - 2;
	cmd_argv = argv + 2;

	if (eq(cmd, "dump")) {
		rc = dump(sig, cmd_argc, cmd_argv);
	}
	else if (eq(cmd, "load")) {
		rc = load(sig, cmd_argc, cmd_argv);
	}
	else if (eq(cmd, "su")) {
		rc = su(sig, cmd_argc, cmd_argv);
	}

	free_sig(sig);
cleanup:
	free_signer();
	return rc;
}
