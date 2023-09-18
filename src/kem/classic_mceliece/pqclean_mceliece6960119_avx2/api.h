#ifndef PQCLEAN_MCELIECE6960119_AVX2_API_H
#define PQCLEAN_MCELIECE6960119_AVX2_API_H

#include <stdint.h>

#define PQCLEAN_MCELIECE6960119_AVX2_CRYPTO_ALGNAME "Classic McEliece 6960119"
#define PQCLEAN_MCELIECE6960119_AVX2_CRYPTO_PUBLICKEYBYTES 1047319
#define PQCLEAN_MCELIECE6960119_AVX2_CRYPTO_SECRETKEYBYTES 13948
#define PQCLEAN_MCELIECE6960119_AVX2_CRYPTO_CIPHERTEXTBYTES 194
#define PQCLEAN_MCELIECE6960119_AVX2_CRYPTO_BYTES 32

int PQCLEAN_MCELIECE6960119_AVX2_crypto_kem_enc(
    uint8_t *c,
    uint8_t *key,
    const uint8_t *pk,
    const uint8_t *m);

int PQCLEAN_MCELIECE6960119_AVX2_crypto_kem_dec(
    uint8_t *key,
    const uint8_t *c,
    const uint8_t *sk,
    uint8_t *m);

int PQCLEAN_MCELIECE6960119_AVX2_crypto_kem_keypair(
    uint8_t *pk,
    uint8_t *sk);

#endif
