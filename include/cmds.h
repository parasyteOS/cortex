#ifndef __TSU_H_CMDS
#define __TSU_H_CMDS

#include "sign.h"

int dump(struct sig_payload* sig, int argc, char *argv[]);
int load(struct sig_payload* sig, int argc, char *argv[]);
int su(struct sig_payload* sig, int argc, char *argv[]);

#endif
