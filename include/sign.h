#ifndef __TSU_H_SIGN
#define __TSU_H_SIGN

struct sig_payload {
	uint8_t *signature;
	uint32_t signature_size;
};

int init_signer(const char *private_key_path);
void free_signer();
struct sig_payload *gen_sig();
void free_sig(struct sig_payload* sig);

#endif
