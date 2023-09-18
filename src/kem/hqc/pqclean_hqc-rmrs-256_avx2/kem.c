#include "api.h"
#include "fips202.h"
#include "hqc.h"
#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "sha2.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
/**
 * @file kem.c
 * @brief Implementation of api.h
 */

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

/**
 * @brief Keygen of the HQC_KEM IND_CAA2 scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 * @returns 0 if keygen is successful
 */
int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {

    PQCLEAN_HQCRMRS256_AVX2_hqc_pke_keygen(pk, sk);
    return 0;
}



/**
 * @brief Encapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ct String containing the ciphertext
 * @param[out] ss String containing the shared secret
 * @param[in] pk String containing the public key
 * @returns 0 if encapsulation is successful
 */
int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk, const unsigned char *message) {

    uint8_t theta[SHA512_BYTES] = {0};
    uint8_t m[VEC_K_SIZE_BYTES];
    static uint64_t u[VEC_N_256_SIZE_64] = {0};
    uint64_t v[VEC_N1N2_256_SIZE_64] = {0};
    unsigned char d[SHA512_BYTES] = {0};
    unsigned char mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};

    // Computing m
    // randombytes(m, VEC_K_SIZE_BYTES);
    memset(m, 0, VEC_K_SIZE_BYTES);
    memcpy(m, message, VEC_K_SIZE_BYTES);

    fprintBstr(stdout, "Input messaege: ", m, VEC_K_SIZE_BYTES);
    fprintBstr(stdout, "Input messaege: ", message, VEC_K_SIZE_BYTES);

    // Computing theta
    sha3_512(theta, m, VEC_K_SIZE_BYTES);

    // Encrypting m
    PQCLEAN_HQCRMRS256_AVX2_hqc_pke_encrypt(u, v, m, theta, pk);
    // fprintBstr(stdout, "u: ", u, VEC_N_256_SIZE_64);
    // fprintBstr(stdout, "v: ", v, VEC_N_256_SIZE_64);

    // Computing d
    sha512(d, m, VEC_K_SIZE_BYTES);

    // Computing shared secret
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    PQCLEAN_HQCRMRS256_AVX2_store8_arr(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
    PQCLEAN_HQCRMRS256_AVX2_store8_arr(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    sha512(ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES);

    // Computing ciphertext
    PQCLEAN_HQCRMRS256_AVX2_hqc_ciphertext_to_string(ct, u, v, d);


    return 0;
}



/**
 * @brief Decapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ss String containing the shared secret
 * @param[in] ct String containing the cipĥertext
 * @param[in] sk String containing the secret key
 * @returns 0 if decapsulation is successful, -1 otherwise
 */
int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk, unsigned char *message) {

    uint8_t result;
    uint64_t u[VEC_N_256_SIZE_64] = {0};
    uint64_t v[VEC_N1N2_256_SIZE_64] = {0};
    unsigned char d[SHA512_BYTES] = {0};
    unsigned char pk[PUBLIC_KEY_BYTES] = {0};
    uint8_t m[VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[SHA512_BYTES] = {0};
    uint64_t u2[VEC_N_256_SIZE_64] = {0};
    uint64_t v2[VEC_N1N2_256_SIZE_64] = {0};
    unsigned char d2[SHA512_BYTES] = {0};
    unsigned char mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};

    //fprintf(stdout, "Decryption start\n");

    // Retrieving u, v and d from ciphertext
    PQCLEAN_HQCRMRS256_AVX2_hqc_ciphertext_from_string(u, v, d, ct);

    // Retrieving pk from sk
    memcpy(pk, sk + SEED_BYTES, PUBLIC_KEY_BYTES);

    // Decryting
    PQCLEAN_HQCRMRS256_AVX2_hqc_pke_decrypt(m, u, v, sk);
    memcpy(message, m, 32);

    // Computing theta
    sha3_512(theta, m, VEC_K_SIZE_BYTES);

    // Encrypting m'
    PQCLEAN_HQCRMRS256_AVX2_hqc_pke_encrypt(u2, v2, m, theta, pk);

    // Computing d'
    sha512(d2, m, VEC_K_SIZE_BYTES);

    // Computing shared secret
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    PQCLEAN_HQCRMRS256_AVX2_store8_arr(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_256_SIZE_64);
    PQCLEAN_HQCRMRS256_AVX2_store8_arr(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
    sha512(ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES);

    // Abort if c != c' or d != d'
    result = PQCLEAN_HQCRMRS256_AVX2_vect_compare((uint8_t *)u, (uint8_t *)u2, VEC_N_SIZE_BYTES);
    result |= PQCLEAN_HQCRMRS256_AVX2_vect_compare((uint8_t *)v, (uint8_t *)v2, VEC_N1N2_SIZE_BYTES);
    result |= PQCLEAN_HQCRMRS256_AVX2_vect_compare(d, d2, SHA512_BYTES);
    result = (uint8_t) (-((int16_t) result) >> 15);
    for (size_t i = 0; i < SHARED_SECRET_BYTES; i++) {
        ss[i] &= ~result;
    }


    return -(result & 1);
}
