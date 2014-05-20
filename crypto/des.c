
#include <inttypes.h>
#include <stdio.h>

#define DELTA_SWAP(a, b, d, m)          \
    (b) = ((a) ^ ((a) >> (d))) & (m);   \
    (a) ^= (b) ^ ((b) << (d))

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

uint64_t pc1(uint64_t key)
{
    uint64_t tmp;
    
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

    return key;
}

uint64_t rot1(uint64_t key)
{
    return ((key << 1) & UINT64_C(0xFFFFFFEFFFFFFE00)) | ((key >> 27) & UINT64_C(0x0000001000000100));
}

uint64_t rot2(uint64_t key)
{
    return ((key << 2) & UINT64_C(0xFFFFFFCFFFFFFC00)) | ((key >> 26) & UINT64_C(0x0000003000000300));
}

uint64_t pc2(uint64_t key)
{
    uint64_t tmp;
    
    DELTA_SWAP(key, tmp,  1, UINT64_C(0x0511010550050400));
    DELTA_SWAP(key, tmp,  2, UINT64_C(0x1220100133102000));
    DELTA_SWAP(key, tmp, 16, UINT64_C(0x000076ba00000000));
    DELTA_SWAP(key, tmp, 32, UINT64_C(0x0000000020801100));
    DELTA_SWAP(key, tmp,  8, UINT64_C(0x0034005a00e60047));
    DELTA_SWAP(key, tmp,  4, UINT64_C(0x0f01020e060a0304));
    DELTA_SWAP(key, tmp, 32, UINT64_C(0x0000000000270103));
    DELTA_SWAP(key, tmp, 16, UINT64_C(0x000096ad00008345));
    DELTA_SWAP(key, tmp,  2, UINT64_C(0x0033021302200000));
    DELTA_SWAP(key, tmp,  1, UINT64_C(0x5044401414050000));
    
    return key;
}

int main(void)
{
    uint64_t blk = UINT64_C(0x675A69675E5A6B5A);
    //uint64_t key = UINT64_C(0x8000000000000000);
    uint64_t key = UINT64_C(0x133457799bbcdff1);
    printf("IP: %016" PRIX64 " -> %016" PRIX64 "\n", blk, ip(blk));
    printf("FP: %016" PRIX64 " -> %016" PRIX64 "\n", blk, fp(ip(blk)));
    printf("K1: %016" PRIX64 " -> %012" PRIX64 "\n", key, pc2(rot1(pc1(key))) >> 16);

    return 0;
}

