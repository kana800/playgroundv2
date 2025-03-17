#pragma once

#include <stdint.h>

// this implementation matches pcg_engines::cm_oneseq_dxsm_128_64

// NOTE: you should use this on 64-bit architectures only. While it will work on 32-bit and will
// give exact same numbers, it will be slower. You should use this when you need to have 128-bit
// state on 32-bit architecture. If all you need is 64-bit random, use pcg32_next64 from pcg32.h

#if defined(__SIZEOF_INT128__) && !defined(__wasm__)
    typedef unsigned __int128 pcg64_uint128;
    #define PCG64_INIT(hi, lo)  (((pcg64_uint128)(hi)) << 64) + (lo)
    #define PCG64_IS_ZERO(r)    ((r) == (pcg64_uint128)0)
    #define PCG64_ZERO(r)       (r) = (pcg64_uint128)0
    #define PCG64_COPY(r, x)    (r) = (x)
    #define PCG64_LO(r)         (uint64_t)(r)
    #define PCG64_HI(r)         (uint64_t)((r) >> 64)
    #define PCG64_SHR(r, c)     (r) >>= (c)
    #define PCG64_ADD(r, x)     (r) += (x)
    #define PCG64_MUL(r, x)     (r) *= (x)
#else
    typedef uint64_t pcg64_uint128[2];
    #define PCG64_INIT(hi, lo) { (lo), (hi) }
    #define PCG64_IS_ZERO(r)   (((r)[0] | (r)[1]) == 0)
    #define PCG64_ZERO(r)      (r)[0] = (r)[1] = 0
    #define PCG64_LO(r)        (r)[0]
    #define PCG64_HI(r)        (r)[1]
    #if defined(_MSC_VER) && defined(_M_AMD64)
        #include <intrin.h>
        // __assume(x) is here to prevent MSVC using SSE registers instead of just a pair of GPR's
        // see the codegen differences between functions here: https://godbolt.org/z/rT8MscbPh
        #define PCG64_COPY(r, x) (r)[0] = (x)[0], __assume(x), (r)[1] = (x)[1]
        #define PCG64_SHR(r, c)  (r)[0] = __shiftright128((r)[0], (r)[1], (c)), (r)[1] >>= (c)
        #define PCG64_ADD(r, x)  _addcarry_u64(_addcarry_u64(0, (r)[0], (x)[0], &(r)[0]), (r)[1], (x)[1], &(r)[1])
        #define PCG64_MUL(r, x)                                 \
        {                                                       \
            uint64_t _temp = (r)[0] * (x)[1] + (r)[1] * (x)[0]; \
            (r)[0] = _umul128((r)[0], (x)[0], &(r)[1]);         \
            (r)[1] += _temp;                                    \
        }
    #elif defined(_MSC_VER) && defined(_M_ARM64)
        #include <intrin.h>
        #define PCG64_COPY(r, x) (r)[0] = (x)[0], (r)[1] = (x)[1]
        #define PCG64_SHR(r, c)  (r)[0] = (((r)[0] >> (c)) | ((r)[1] << (64 - (c)))), (r)[1] >>= (c)
        #define PCG64_ADD(r, x)  (r)[0] += (x)[0], (r)[1] += (x)[1] + ((r)[0] < (x)[0])
        #define PCG64_MUL(r, x)                                                   \
        {                                                                         \
            (r)[1] = __umulh((r)[0], (x)[0]) + (r)[0] * (x)[1] + (r)[1] * (x)[0]; \
            (r)[0] *= (x)[0];                                                     \
        }
    #else
        #define PCG64_COPY(r, x) (r)[0] = (x)[0], (r)[1] = (x)[1]
        #define PCG64_SHR(r, c)  (r)[0] = (((r)[0] >> (c)) | ((r)[1] << (64 - (c)))), (r)[1] >>= (c)
        #define PCG64_ADD(r, x)  (r)[0] += (x)[0], (r)[1] += (x)[1] + ((r)[0] < (x)[0])
        #define PCG64_MUL(r, x)                                     \
        {                                                           \
            uint64_t _a = (r)[0];                                   \
            uint64_t _b = (x)[0];                                   \
            uint64_t _ll = (_a & 0xFFFFFFFF) * (_b & 0xFFFFFFFF);   \
            uint64_t _hl = (_a >> 32)        * (_b & 0xFFFFFFFF);   \
            uint64_t _lh = (_a & 0xFFFFFFFF) * (_b >> 32);          \
            uint64_t _hh = (_a >> 32)        * (_b >> 32);          \
            uint64_t _mid = (_ll >> 32) + (_hl & 0xFFFFFFFF) + _lh; \
            uint64_t _top = (_hl >> 32) + (_mid >> 32)       + _hh; \
            (r)[1] = _a * (x)[1] + (r)[1] * _b + _top;              \
            (r)[0] = (_mid << 32) | (_ll & 0xFFFFFFFF);             \
        }
    #endif
#endif

#define PCG64_CHEAP_MULTIPLIER 15750249268501108917ULL
#define PCG64_DEFAULT_INCREMENT_HI 6364136223846793005ULL
#define PCG64_DEFAULT_INCREMENT_LO 1442695040888963407ULL

typedef struct {
    pcg64_uint128 state;
} pcg64;

static inline uint64_t pcg64_next(pcg64* rng)
{
    pcg64_uint128 state_mul = PCG64_INIT( 0, PCG64_CHEAP_MULTIPLIER );
    pcg64_uint128 state_add = PCG64_INIT( PCG64_DEFAULT_INCREMENT_HI, PCG64_DEFAULT_INCREMENT_LO );

    pcg64_uint128 state;
    PCG64_COPY(state, rng->state);

    uint64_t hi = PCG64_HI(state);
    uint64_t lo = PCG64_LO(state);

    PCG64_MUL(state, state_mul);
    PCG64_ADD(state, state_add);
    PCG64_COPY(rng->state, state);

    // DXSM
    hi ^= hi >> 32;
    hi *= PCG64_CHEAP_MULTIPLIER;
    hi ^= hi >> 48;
    hi *= lo | 1;
    return hi;
}

// returns value in [low,high) interval

static inline uint64_t pcg64_range(pcg64* rng, uint64_t low, uint64_t high)
{
    uint64_t bound = high - low;

#if 1

    // An optimal algorithm for bounded random integers: https://github.com/apple/swift/pull/39143

    pcg64_uint128 r1 = PCG64_INIT( 0, pcg64_next(rng) );
    pcg64_uint128 r2 = PCG64_INIT( 0, pcg64_next(rng) );
    pcg64_uint128 b = PCG64_INIT( 0, bound );
    PCG64_MUL(r1, b);
    PCG64_MUL(r2, b);

    pcg64_uint128 t1 = PCG64_INIT( 0, PCG64_LO(r1) );
    pcg64_uint128 t2 = PCG64_INIT( 0, PCG64_HI(r2) );
    PCG64_ADD(t1, t2);

    return low + PCG64_HI(r1) + PCG64_HI(t1);

#else

    // Fast Random Integer Generation in an Interval: https://arxiv.org/abs/1805.10941

    pcg64_uint128 m;
    PCG64_MUL2(m, pcg64_next(rng), bound);

    uint64_t l = PCG64_LO(m);

    if (l < bound)
    {
        uint64_t t = -(int64_t)bound % bound;
        while (l < t)
        {
            PCG64_MUL2(m, pcg64_next(rng), bound);
            l = PCG64_LO(m);
        }
    }

    return low + PCG64_HI(m);

#endif
}

// returns float & double in [0,1) interval

static inline float pcg64_nextf(pcg64* rng)
{
    uint64_t x = pcg64_next(rng);
    return (float)(int32_t)(x >> 40) * 0x1.0p-24f;
}

static inline double pcg64_nextd(pcg64* rng)
{
    uint64_t x = pcg64_next(rng);
    return (double)(int64_t)(x >> 11) * 0x1.0p-53;
}

static inline void pcg64_seed(pcg64* rng, uint64_t seed_high, uint64_t seed_low)
{
    pcg64_uint128 seed = PCG64_INIT( seed_high, seed_low );

    PCG64_ZERO(rng->state);
    pcg64_next(rng);
    PCG64_ADD(rng->state, seed);
    pcg64_next(rng);
}

static inline void pcg64_advance(pcg64* rng, uint64_t delta_high, uint64_t delta_low)
{
    pcg64_uint128 cur_mult = PCG64_INIT(                          0, PCG64_CHEAP_MULTIPLIER     );
    pcg64_uint128 cur_plus = PCG64_INIT( PCG64_DEFAULT_INCREMENT_HI, PCG64_DEFAULT_INCREMENT_LO );

    pcg64_uint128 acc_mult = PCG64_INIT( 0ULL, 1ULL );
    pcg64_uint128 acc_plus = PCG64_INIT( 0ULL, 0ULL );

    pcg64_uint128 delta = PCG64_INIT( delta_high, delta_low );

    while (!PCG64_IS_ZERO(delta))
    {
        if (PCG64_LO(delta) & 1)
        {
            PCG64_MUL(acc_mult, cur_mult);
            PCG64_MUL(acc_plus, cur_mult);
            PCG64_ADD(acc_plus, cur_plus);
        }
        pcg64_uint128 temp;
        PCG64_COPY(temp, cur_plus);
        PCG64_MUL(cur_plus, cur_mult);
        PCG64_ADD(cur_plus, temp);
        PCG64_MUL(cur_mult, cur_mult);
        PCG64_SHR(delta, 1);
    }

    PCG64_MUL(rng->state, acc_mult);
    PCG64_ADD(rng->state, acc_plus);
}
