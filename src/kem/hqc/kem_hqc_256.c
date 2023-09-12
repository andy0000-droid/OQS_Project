// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_hqc.h>

#if defined(OQS_ENABLE_KEM_hqc_256)

OQS_KEM *OQS_KEM_hqc_256_new(void) {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_hqc_256;
	kem->alg_version = "hqc-submission_2020-10-01 via https://github.com/jschanck/package-pqclean/tree/29f79e72/hqc";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;
	
	//kem->length_plaintext = OQS_KEM_hqc_256_length_plaintext;
	kem->length_public_key = OQS_KEM_hqc_256_length_public_key;
	kem->length_secret_key = OQS_KEM_hqc_256_length_secret_key;
	kem->length_ciphertext = OQS_KEM_hqc_256_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_hqc_256_length_shared_secret;

	kem->keypair = OQS_KEM_hqc_256_keypair;
	kem->encaps = OQS_KEM_hqc_256_encaps;
	kem->decaps = OQS_KEM_hqc_256_decaps;

	return kem;
}

extern int PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_hqc_256_avx2)
extern int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *message);
extern int PQCLEAN_HQCRMRS256_AVX2_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk, uint8_t *message);
#endif

OQS_API OQS_STATUS OQS_KEM_hqc_256_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_hqc_256_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_AVX2_crypto_kem_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_256_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key, const uint8_t *message) {
#if defined(OQS_ENABLE_KEM_hqc_256_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_AVX2_crypto_kem_enc(ciphertext, shared_secret, public_key, message);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_hqc_256_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key, uint8_t *message) {
#if defined(OQS_ENABLE_KEM_hqc_256_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI1) && OQS_CPU_has_extension(OQS_CPU_EXT_PCLMULQDQ)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_AVX2_crypto_kem_dec(shared_secret, ciphertext, secret_key, message);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_HQCRMRS256_CLEAN_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
