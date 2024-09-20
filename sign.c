#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "tsu.h"

struct dynamic_digest {
	uint64_t secs;
	uint32_t pid;
	uint32_t uid;
};

static EVP_PKEY *pkey = NULL;
static EC_KEY *eckey = NULL;

int init_signer(const char *private_key_path)
{
    	OpenSSL_add_all_algorithms();
    	ERR_load_crypto_strings();

    	// Read the private key using EVP_PKEY
    	FILE *fp = fopen(private_key_path, "r");
    	if (!fp) {
		perror("Unable to open private key file.");
    	}

    	pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    	fclose(fp);

    	if (!pkey) {
		ERR_print_errors_fp(stderr);
		goto err_cleanup;
    	}

    	// Ensure the key is an EC key
    	if (EVP_PKEY_base_id(pkey) != EVP_PKEY_EC) {
		perror("The key is not an EC key.\n");
		goto err_free_pkey;
    	}

    	eckey = EVP_PKEY_get1_EC_KEY(pkey);
    	if (!eckey) {
		ERR_print_errors_fp(stderr);
		goto err_free_eckey;
    	}

	return 0;

err_free_eckey:
	EC_KEY_free(eckey);
err_free_pkey:
    	EVP_PKEY_free(pkey);
err_cleanup:
    	EVP_cleanup();
    	ERR_free_strings();
    	return 1;
}

void free_signer()
{
	EC_KEY_free(eckey);
    	EVP_PKEY_free(pkey);
    	EVP_cleanup();
    	ERR_free_strings();
}

struct sig_payload *gen_sig()
{
	struct timespec tp = {};
	clock_gettime(CLOCK_REALTIME, &tp);
	struct dynamic_digest digest = {
		.secs = tp.tv_sec - tp.tv_sec % 5,
		.pid = getpid(),
		.uid = getuid(),
	};
	struct sig_payload *payload = malloc(sizeof(struct sig_payload));
    	unsigned char *sig_buf = NULL; // DER encoding writes to this buffer
    	size_t sig_len = 0;
    	ECDSA_SIG *ecdsa_sig = ECDSA_do_sign((const unsigned char *)&digest, sizeof(digest), eckey);

    	if (!ecdsa_sig) {
		ERR_print_errors_fp(stderr);
		goto err_free_payload;
    	}

    	sig_len = i2d_ECDSA_SIG(ecdsa_sig, &sig_buf);
	if (sig_len == 0) {
		ERR_print_errors_fp(stderr);
		goto err_free_sig;
	}

	payload->signature = sig_buf;
	payload->signature_size = sig_len;
    	ECDSA_SIG_free(ecdsa_sig);
	return payload;

err_free_sig:
    	ECDSA_SIG_free(ecdsa_sig);
err_free_payload:
	free(payload);
	return NULL;
}

void free_sig(struct sig_payload *sig)
{
	OPENSSL_free(sig->signature);
	free(sig);
}
