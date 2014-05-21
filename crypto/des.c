
#include <inttypes.h>
#include <stdio.h>

// Swap multiple bit pairs
#define DELTA_SWAP(a, b, d, m)          \
    (b) = ((a) ^ ((a) >> (d))) & (m);   \
    (a) ^= (b) ^ ((b) << (d))

// 1-bit subkey rotation
#define ROT1(k) \
    ((((k) << 1) & UINT64_C(0xFFFFFFEFFFFFFE00)) | (((k) >> 27) & UINT64_C(0x0000001000000100)))

// 2-bit subkey rotation
#define ROT2(k) \
    ((((k) << 2) & UINT64_C(0xFFFFFFCFFFFFFC00)) | (((k) >> 26) & UINT64_C(0x0000003000000300)))
   

uint64_t ip(uint64_t blk)
{
    uint64_t tmp;

    DELTA_SWAP(blk, tmp,  9, UINT64_C(0x0055005500550055));
    DELTA_SWAP(blk, tmp, 18, UINT64_C(0x0000333300003333));
    DELTA_SWAP(blk, tmp, 36, UINT64_C(0x000000000F0F0F0F));
    DELTA_SWAP(blk, tmp, 24, UINT64_C(0x00000000FF00FF00));
    DELTA_SWAP(blk, tmp, 24, UINT64_C(0x000000FF000000FF));

    return blk;
}

uint64_t fp(uint64_t blk)
{
    uint64_t tmp;

    DELTA_SWAP(blk, tmp, 24, UINT64_C(0x000000FF000000FF));
    DELTA_SWAP(blk, tmp, 24, UINT64_C(0x00000000FF00FF00));
    DELTA_SWAP(blk, tmp, 36, UINT64_C(0x000000000F0F0F0F));
    DELTA_SWAP(blk, tmp, 18, UINT64_C(0x0000333300003333));
    DELTA_SWAP(blk, tmp,  9, UINT64_C(0x0055005500550055));

    return blk;
}

void des_key_schedule(uint64_t key, uint64_t subkey[16])
{
    uint64_t tmp;

    // Permuted Choice 1
    DELTA_SWAP(key, tmp,  9, UINT64_C(0x0055005500550055));
    DELTA_SWAP(key, tmp, 18, UINT64_C(0x0000333300003333));
    DELTA_SWAP(key, tmp, 36, UINT64_C(0x000000000F0F0F0F));
    DELTA_SWAP(key, tmp, 24, UINT64_C(0x00000000FFFFFF00));
    DELTA_SWAP(key, tmp, 16, UINT64_C(0x000000FF00000000));
    DELTA_SWAP(key, tmp, 56, UINT64_C(0x00000000000000FF));
    
    tmp = (key >> 24) & UINT64_C(0x0000000000000F00);
    tmp ^= (key << 4) & UINT64_C(0x0000000FFFFFF000);
    tmp ^= key;
    tmp &= UINT64_C(0x0000000FFFFFFFFF);
    key ^= tmp;

    // Precompute subkeys for each round
    for (int n = 0; n < 16; n++)
    {
        key = (n < 2 || n == 8 || n == 15) ? ROT1(key) : ROT2(key);
        subkey[n] = key;
        
        // Permuted Choice 2
        DELTA_SWAP(subkey[n], tmp,  1, UINT64_C(0x0511010550050400));
        DELTA_SWAP(subkey[n], tmp,  2, UINT64_C(0x1220100133102000));
        DELTA_SWAP(subkey[n], tmp, 16, UINT64_C(0x000076ba00000000));
        DELTA_SWAP(subkey[n], tmp, 32, UINT64_C(0x0000000020801100));
        DELTA_SWAP(subkey[n], tmp,  8, UINT64_C(0x0034005a00e60047));
        DELTA_SWAP(subkey[n], tmp,  4, UINT64_C(0x0f01020e060a0304));
        DELTA_SWAP(subkey[n], tmp, 32, UINT64_C(0x0000000000270103));
        DELTA_SWAP(subkey[n], tmp, 16, UINT64_C(0x000096ad00008345));
        DELTA_SWAP(subkey[n], tmp,  2, UINT64_C(0x0033021302200000));
        DELTA_SWAP(subkey[n], tmp,  1, UINT64_C(0x5044401414050000));

        subkey[n] >>= 16;
    }
}


int main(void)
{
    //uint64_t blk = UINT64_C(0x675A69675E5A6B5A);
    uint64_t key = UINT64_C(0x133457799bbcdff1);
    uint64_t subkey[16];

    des_key_schedule(key, subkey);

    printf("\nSchedule for key %016" PRIX64 ":\n\n", key);

    for (int n = 0; n < 16; n++)
    {
        printf("K%d: %012" PRIX64 "\n", n + 1, subkey[n]);
    }

    return 0;
}

