#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int main(int argc, char **argv)
{

	FILE *f;
	size_t len;
	unsigned char buffer[BUFSIZ];

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	f = fopen(argv[1], "r");

	if (!f) {
		fprintf(stderr, "couldn't open %s\n", argv[1]);
		return 1;
	}

	ERR_load_crypto_strings();	

	EVP_MD_CTX hashctx;
	//EVP_MD *hashptr = EVP_get_digestbyname("SHA256");
	const EVP_MD *hashptr = EVP_get_digestbyname("SHA1");

	EVP_MD_CTX_init(&hashctx);
	EVP_DigestInit_ex(&hashctx, hashptr, NULL);
	do {
		len = fread(buffer, 1, BUFSIZ, f);
		EVP_DigestUpdate(&hashctx, buffer, len);
	} while (len == BUFSIZ);

	unsigned int outlen;
	EVP_DigestFinal_ex(&hashctx, buffer, &outlen);
	EVP_MD_CTX_cleanup(&hashctx);

	fclose(f);

	int i;
	for (i = 0; i < outlen; ++i)
		printf("%02x", buffer[i]);

	return 0;
}
