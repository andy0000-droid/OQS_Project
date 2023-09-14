// SPDX-License-Identifier: MIT

// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:
//     cat PQCkemKAT_whatever.rsp | head -n 8 | tail -n 6

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

#include "system_info.c"

/* Displays hexadecimal strings */
static void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len)
{
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++)
	{
		printf("%02X", str[i]);
	}
	printf("\n");
}

static void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L)
{
	size_t i;
	fprintf(fp, "%s", S);
	if (fp == stdout)
	{
		for (i = 0; i < L; i++)
		{
			fprintf(fp, "%02X", A[i]);
		}
	}
	else
	{
		for (i = 0; i < L; i++)
		{
			fprintf(fp, "%c", A[i]);
		}
	}

	if (L == 0)
	{
		fprintf(fp, "00");
	}
	if (fp == stdout) {
		fprintf(fp, "\n");
	}
}

static void freadBstr(FILE *fp, uint8_t *A, size_t L)
{
	void *m = NULL;
	m = malloc(L);
	memset(m, 0, L);
	size_t size = fread(m, sizeof(uint8_t), L, fp);
	if (size <= L)
	{
		memcpy(A, m, L);
		// fprintBstr(stdout, "file: ", A, L);
	}
	else
	{
		fprintf(stderr, "Read file error\n");
	}
	free(m);

	/*for (size_t i = 0; i < L; i++)
	{
		fscanf(fp, "%02X", (unsigned int) &A[i]);
	}*/
}

static OQS_STATUS kem_kat(const char *method_name, FILE *fm, const int device, FILE *fp)
{
	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	uint8_t *m = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	if (device == -1)
	{
		fprintf(stderr, "Input kind of device you use");
	}

	kem = OQS_KEM_new(method_name);
	if (kem == NULL)
	{
		printf("[kem_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (kem->claimed_nist_level != 5){
		fprintf(stderr, "Only use NIST level 5\n");
		goto err;
	}

	for (uint8_t i = 0; i < 48; i++)
	{
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS)
	{
		goto err;
	}
	OQS_randombytes_nist_kat_init_256bit(entropy_input, NULL);

	fh = stdout;
	FILE *fpk = NULL;
	FILE *fsk = NULL;
	FILE *fct = NULL;
	FILE *fss = NULL;
	FILE *fpt = NULL;

	OQS_randombytes(seed, 48);
	OQS_randombytes_nist_kat_init_256bit(seed, NULL);

	// if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL))
	// {
	// 	fprintf(stderr, "[kat_kem] %s ERROR: malloc failed!\n", method_name);
	// 	goto err;
	// }
	if (fp == NULL)
	{
		fprintf(fh, "keypair need\n");
	}
	
	fprintf(fh, "starting KEM algirhtm\n");
	if (device == 2 && fp == NULL)
	{
		// fprintf(fh, "count = 0\n");
		fprintf(fh, "start keypair\n");
		fprintBstr(fh, "seed = ", seed, 48); // error occur
		public_key = malloc(kem->length_public_key);
		secret_key = malloc(kem->length_secret_key);
		memset(public_key, 0, kem->length_public_key);
		memset(secret_key, 0, kem->length_secret_key);
		fpk = fopen("public_key.txt", "wb");
		fsk = fopen("secret_key.txt", "wb");
		rc = OQS_KEM_keypair(kem, public_key, secret_key);
		if (rc != OQS_SUCCESS)
		{
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
			goto err;
		}
		fprintBstr(fpk, "", public_key, kem->length_public_key);
		fprintBstr(fsk, "", secret_key, kem->length_secret_key);
		//fprintBstr(fh, "pk = ", public_key, kem->length_public_key);
		// fprintBstr(fh, "sk=", secret_key, kem->length_secret_key);
		fclose(fpk);
		fclose(fsk);
		fpk = NULL;
		fsk = NULL;
	}
	if (device == 1 || device == 0)
	{
		if (fm == NULL)
		{
			fprintf(stderr, "No message file\n");
			return EXIT_FAILURE;
		}

		public_key = malloc(kem->length_public_key);
		memset(public_key, 0, kem->length_public_key);

		ciphertext = malloc(kem->length_ciphertext);
		memset(ciphertext, 0, kem->length_ciphertext);

		shared_secret_e = malloc(kem->length_shared_secret);
		memset(shared_secret_e, 0, kem->length_shared_secret);

		m = malloc(kem->length_plaintext);
		freadBstr(fm, m, kem->length_plaintext);
		//memcpy(m, message, strlen(message));
		//fprintBstr(fh, "initial message = ", m, kem->length_plaintext);
		fct = fopen("cipher_text.txt", "wb");
		fss = fopen("shared_secret.txt", "wb");

		freadBstr(fp, public_key, kem->length_public_key);
		//fprintBstr(fh, "pk = ", public_key, kem->length_public_key);
		rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key, m);
		if (rc != OQS_SUCCESS)
		{
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
			goto err;
		}
		//fprintBstr(fh, "ct = ", ciphertext, kem->length_ciphertext);
		//fprintBstr(fh, "ss = ", shared_secret_e, kem->length_shared_secret);
		fprintBstr(fct, "", ciphertext, kem->length_ciphertext);
		fprintBstr(fss, "", shared_secret_e, kem->length_shared_secret);
		fclose(fct);
		fclose(fss);
		fct = NULL;
		fss = NULL;
		free(m);
		m = NULL;
		//fclose(fsk);
		//fsk = NULL;
	}
	if (device == 2 && fp != NULL)
	{
		fprintf(stdout, "Start decapsulation\n");
		secret_key = malloc(kem->length_secret_key);
		memset(secret_key, 0, kem->length_secret_key);

		shared_secret_d = malloc(kem->length_shared_secret);
		memset(shared_secret_d, 0, kem->length_shared_secret);

		ciphertext = malloc(kem->length_ciphertext);
		memset(ciphertext, 0, kem->length_ciphertext);

		freadBstr(fm, ciphertext, kem->length_ciphertext);
		//fprintBstr(fh, "ct = ", ciphertext, kem->length_ciphertext);
		freadBstr(fp, secret_key, kem->length_secret_key);
		fpt = fopen("decryted.txt", "wb");
		fss = fopen("shared_decaps.txt", "wb");
		m = malloc(kem->length_plaintext);
		memset(m, 0, kem->length_plaintext);
		rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key, m);
		if (rc != OQS_SUCCESS)
		{
			fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
			goto err;
		}
		//fprintBstr(fh, "shared_sercret: ", shared_secret_d, kem->length_shared_secret);
		fprintBstr(fpt, "", m, kem->length_plaintext);
		fprintBstr(fss, "", shared_secret_d, kem->length_shared_secret);
		/*
		rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
		if (rv != 0)
		{
			fprintf(stderr, "[kat_kem] %s ERROR: shared secrets are not equal\n", method_name);
			OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
			OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
			goto err;
		}*/
		fclose(fpt);
		fpt = NULL;
		fclose(fss);
		fss = NULL;
		free(m);
		m = NULL;
	}
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL)
	{
		if (secret_key != NULL)
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		if (shared_secret_e != NULL)
			OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		if (shared_secret_d != NULL)
			OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	if (public_key != NULL)
		OQS_MEM_insecure_free(public_key);
	if (ciphertext != NULL)
		OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);
	return ret;
}

int main(int argc, char **argv)
{

	char *alg_name = NULL;
	uint8_t *message = NULL;

	int device = -1;
	FILE *fp = NULL;
	FILE *fm = NULL;

	OQS_init();

	if (argc == 1)
	{
		fprintf(stderr, "%d\n", argc);
		fprintf(stderr, "Usage: kat_kem algname message device\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++)
		{
			if (i > 0)
			{
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}
	else
	{
		alg_name = argv[1];
		// size_t len = strlen(argv[2]);
		// message = malloc(32);
		// memset(message, 0, 32);
		// memcpy(message, argv[2], len);
		// fprintf(stdout, "argv[2] len: %ld\n",strlen(argv[2]));
		// fprintBstr(stdout, "message = ", message, len);
		if (!(strcmp(argv[2], "server")))
		{
			device = 0;
			fprintf(stdout, "set device to server\nset device ID to %d\n", device);
			fp = fopen(argv[4], "rb");
			fm = fopen(argv[3], "rb");
		}
		else if (!(strcmp(argv[2], "client_a")))
		{
			device = 1;
			fprintf(stdout, "set device to client alice\nset device ID to %d\n", device);
			fp = fopen(argv[4], "rb");
			fm = fopen(argv[3], "rb");
		}
		else if (!(strcmp(argv[2], "client_b")))
		{
			device = 2;
			fprintf(stdout, "set device to client bob\nset device ID to %d\n", device);
			if (argc > 3) {
				fp = fopen(argv[4], "rb");
				fm = fopen(argv[3], "rb");
			}
		}
		else if (!(strcmp(argv[2], "help")))
		{
			fprintf(stderr, "%d\n", argc);
			fprintf(stderr, "Usage: kat_kem algname message device\n");
			fprintf(stderr, "  algname: ");
			for (size_t i = 0; i < OQS_KEM_algs_length; i++)
			{
				if (i > 0)
				{
					fprintf(stderr, ", ");
				}
				fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
			}
			fprintf(stderr, "\n");
			printf("\n");
			print_system_info();
			OQS_destroy();
			return EXIT_FAILURE;
		}
		else
		{
			fprintf(stderr, "Input proper option\n");
			return EXIT_FAILURE;
		}
	}
	if (fp == NULL && device != 2)
	{
		fprintf(stderr, "Input file name with extension\n");
		return EXIT_FAILURE;
	}

	OQS_STATUS rc = kem_kat(alg_name, fm, device, fp);
	if (rc != OQS_SUCCESS)
	{
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	fclose(fp);
	fclose(fm);
	free(message);
	return EXIT_SUCCESS;
}
