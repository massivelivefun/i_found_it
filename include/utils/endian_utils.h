#ifndef ENDIAN_UTILS_H
#define ENDIAN_UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define IS_HOST_BIG_ENDIAN 1
    #define IS_HOST_LITTLE_ENDIAN 0
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define IS_HOST_BIG_ENDIAN 0
    #define IS_HOST_LITTLE_ENDIAN 1
#else
    #if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || \
            defined(_M_X64) || defined(__ARMEL__) || defined(__THUMBEL__) || \
            defined(__AARCH64EL__) || defined(_M_ARM)
        #define IS_HOST_BIG_ENDIAN 0
        #define IS_HOST_LITTLE_ENDIAN 1
    #elif defined(__ARMEB__) || defined(__THUMBEB__) || \
            defined(__AARCH64EB__) || defined(__sparc) || defined(__sparc__)
        #define IS_HOST_BIG_ENDIAN 1
        #define IS_HOST_LITTLE_ENDIAN 0
    #else
        #define IS_HOST_BIG_ENDIAN (is_host_big_endian())
        #define IS_HOST_LITTLE_ENDIAN (is_host_little_endian())
    #endif
#endif

// FLOAT & DOUBLE BIT CONVERSIONS

static inline float bits_to_float(uint32_t u) {
    float f;
    memcpy(&f, &u, sizeof(float));
    return f;
}

static inline uint32_t float_to_bits(float f) {
    uint32_t u;
    memcpy(&u, &f, sizeof(uint32_t));
    return u;
}

static inline double bits_to_double(uint64_t u) {
    double d;
    memcpy(&d, &u, sizeof(double));
    return d;
}

static inline uint64_t double_to_bits(double d) {
    uint64_t u;
    memcpy(&u, &d, sizeof(uint64_t));
    return u;
}

//
// LITTLE ENDIAN (WAD, BMP, WAV)
//

// 16 BIT - SIGNED & UNSIGNED LITTLE ENDIAN

static inline int read_u16le_from_file(FILE * f, uint16_t * result) {
    uint8_t bytes[2];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
    return 0;
}

// HOW DO I HANDLE FOR ALIGNMENT OF THE SYSTEM?
static inline int read_u16le_from_ptr(const uint8_t * ptr, uint32_t * result) {
    if (ptr == NULL || result == NULL) {
        return 1;
    }
    *result = (uint16_t)ptr[0] | ((uint16_t)ptr[1] << 8);
    return 0;
}

// DO NOT PASS IN NULL INTO THESE FUNCTIONS
static inline uint16_t unsafe_read_u16le_from_ptr(const uint8_t * ptr) {
    return (uint16_t)ptr[0] | ((uint16_t)ptr[1] << 8);
}

static inline int read_i16le_from_file(FILE * f, int16_t * result) {
    uint16_t temp;
    if (read_u16le_from_file(f, &temp) != 0) { return 1; }
    *result = (int16_t)temp;
    return 0;
}

static inline int write_u16le_to_file(FILE * f, uint16_t val) {
    uint8_t bytes[2];
    bytes[0] = (uint8_t)(val & 0xFF);
    bytes[1] = (uint8_t)((val >> 8) & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i16le_to_file(FILE * f, int16_t val) {
    return write_u16le_to_file(f, (uint16_t)val);
}

// 32 BIT - SIGNED & UNSIGNED LITTLE ENDIAN

static inline int read_u32le_from_file(FILE * f, uint32_t * result) {
    uint8_t bytes[4];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = (uint32_t)bytes[0]        | ((uint32_t)bytes[1] << 8)  |
             ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
    return 0;
}

// HOW DO I HANDLE FOR ALIGNMENT OF THE SYSTEM?
static inline int read_u32le_from_ptr(const uint8_t * ptr, uint32_t * result) {
    if (ptr == NULL || result == NULL) {
        return 1;
    }
    *result = (uint32_t)ptr[0]        | 
             ((uint32_t)ptr[1] << 8)  | 
             ((uint32_t)ptr[2] << 16) | 
             ((uint32_t)ptr[3] << 24);
    return 0;
}

// DO NOT PASS IN NULL INTO THESE FUNCTIONS
static inline uint32_t unsafe_read_u32le_from_ptr(const uint8_t * ptr) {
    return (uint32_t)ptr[0]        | 
          ((uint32_t)ptr[1] << 8)  | 
          ((uint32_t)ptr[2] << 16) | 
          ((uint32_t)ptr[3] << 24);
}

static inline int read_i32le_from_file(FILE * f, int32_t * result) {
    uint32_t temp;
    if (read_u32le_from_file(f, &temp) != 0) { return 1; }
    *result = (int32_t)temp;
    return 0;
}

static inline int write_u32le_to_file(FILE * f, uint32_t val) {
    uint8_t bytes[4];
    bytes[0] = (uint8_t)(val & 0xFF);
    bytes[1] = (uint8_t)((val >> 8) & 0xFF);
    bytes[2] = (uint8_t)((val >> 16) & 0xFF);
    bytes[3] = (uint8_t)((val >> 24) & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i32le_to_file(FILE * f, int32_t val) {
    return write_u32le_to_file(f, (uint32_t)val);
}

// 64 BIT - SIGNED & UNSIGNED LITTLE ENDIAN

static inline int read_u64le_from_file(FILE * f, uint64_t * result) {
    uint8_t bytes[8];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = (uint64_t)bytes[0]        | ((uint64_t)bytes[1] << 8)  |
             ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24) |
             ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) |
             ((uint64_t)bytes[6] << 48) | ((uint64_t)bytes[7] << 56);
    return 0;
}

static inline int read_i64le_from_file(FILE * f, int64_t * result) {
    uint64_t temp;
    if (read_u64le_from_file(f, &temp) != 0) { return 1; }
    *result = (int64_t)temp;
    return 0;
}

static inline int write_u64le_to_file(FILE * f, uint64_t val) {
    uint8_t bytes[8];
    bytes[0] = (uint8_t)(val & 0xFF);
    bytes[1] = (uint8_t)((val >> 8) & 0xFF);
    bytes[2] = (uint8_t)((val >> 16) & 0xFF);
    bytes[3] = (uint8_t)((val >> 24) & 0xFF);
    bytes[4] = (uint8_t)((val >> 32) & 0xFF);
    bytes[5] = (uint8_t)((val >> 40) & 0xFF);
    bytes[6] = (uint8_t)((val >> 48) & 0xFF);
    bytes[7] = (uint8_t)((val >> 56) & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i64le_to_file(FILE * f, int64_t val) {
    return write_u64le_to_file(f, (uint64_t)val);
}

// 32 BIT FLOAT - LITTLE ENDIAN

static inline int read_f32le_from_file(FILE *f, float * result) {
    uint32_t temp;
    if (read_u32le_from_file(f, &temp) != 0) { return 1; }
    *result = bits_to_float(temp);
    return 0;
}

static inline int write_f32le_to_file(FILE *f, float val) {
    return write_u32le_to_file(f, float_to_bits(val));
}

// 64 BIT DOUBLE - LITTLE ENDIAN

static inline int read_f64le_from_file(FILE *f, double * result) {
    uint64_t temp;
    if (read_u64le_from_file(f, &temp) != 0) { return 1; }
    *result = bits_to_double(temp);
    return 0;
}

static inline int write_f64le_to_file(FILE *f, double val) {
    return write_u64le_to_file(f, double_to_bits(val));
}

//
// BIG ENDIAN (PNG, JPEG segments, Network packets)
//

// 16 BIT - SIGNED & UNSIGNED BIG ENDIAN

static inline int read_u16be_from_file(FILE * f, uint16_t * result) {
    uint8_t bytes[2];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = ((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1];
    return 0;
}

static inline int read_i16be_from_file(FILE * f, int16_t * result) {
    uint16_t temp;
    if (read_u16be_from_file(f, &temp) != 0) { return 1; }
    *result = (int16_t)temp;
    return 0;
}

static inline int write_u16be_to_file(FILE * f, uint16_t val) {
    uint8_t bytes[2];
    bytes[0] = (uint8_t)((val >> 8) & 0xFF);
    bytes[1] = (uint8_t)(val & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i16be_to_file(FILE * f, int16_t val) {
    return write_u16be_to_file(f, (uint16_t)val);
}

// 32 BIT - SIGNED & UNSIGNED BIG ENDIAN

static inline int read_u32be_from_file(FILE * f, uint32_t * result) {
    uint8_t bytes[4];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
              ((uint32_t)bytes[2] << 8)  |  (uint32_t)bytes[3];
    return 0;

}

static inline int read_i32be_from_file(FILE * f, int32_t * result) {
    uint32_t temp;
    if (read_u32be_from_file(f, &temp) != 0) { return 1; }
    *result = (int32_t)temp;
    return 0;
}

static inline int write_u32be_to_file(FILE * f, uint32_t val) {
    uint8_t bytes[4];
    bytes[0] = (uint8_t)((val >> 24) & 0xFF);
    bytes[1] = (uint8_t)((val >> 16) & 0xFF);
    bytes[2] = (uint8_t)((val >> 8) & 0xFF);
    bytes[3] = (uint8_t)(val & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i32be_to_file(FILE * f, int32_t val) {
    return write_u32be_to_file(f, (uint32_t)val);
}

// 64 BIT - SIGNED & UNSIGNED BIG ENDIAN

static inline int read_u64be_from_file(FILE * f, uint64_t * result) {
    uint8_t bytes[8];
    if (fread(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    *result = ((uint64_t)bytes[0] << 56) | ((uint64_t)bytes[1] << 48) |
              ((uint64_t)bytes[2] << 40) | ((uint64_t)bytes[3] << 32) |
              ((uint64_t)bytes[4] << 24) | ((uint64_t)bytes[5] << 16) |
              ((uint64_t)bytes[6] << 8)  |  (uint64_t)bytes[7];
    return 0;
}

static inline int read_i64be_from_file(FILE * f, int64_t * result) {
    uint64_t temp;
    if (read_u64be_from_file(f, &temp) != 0) { return 1; }
    *result = (int64_t)temp;
    return 0;
}

static inline int write_u64be_to_file(FILE * f, uint64_t val) {
    uint8_t bytes[8];
    bytes[0] = (uint8_t)((val >> 56) & 0xFF);
    bytes[1] = (uint8_t)((val >> 48) & 0xFF);
    bytes[2] = (uint8_t)((val >> 40) & 0xFF);
    bytes[3] = (uint8_t)((val >> 32) & 0xFF);
    bytes[4] = (uint8_t)((val >> 24) & 0xFF);
    bytes[5] = (uint8_t)((val >> 16) & 0xFF);
    bytes[6] = (uint8_t)((val >> 8) & 0xFF);
    bytes[7] = (uint8_t)(val & 0xFF);
    if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int write_i64be_to_file(FILE * f, int64_t val) {
    return write_u64be_to_file(f, (uint64_t)val);
}

// 32 BIT FLOAT - BIG ENDIAN

static inline int read_f32be_from_file(FILE *f, float * result) {
    uint32_t temp;
    if (read_u32be_from_file(f, &temp) != 0) { return 1; }
    *result = bits_to_float(temp);
    return 0;
}

static inline int write_f32be_to_file(FILE *f, float val) {
    return write_u32be_to_file(f, float_to_bits(val));
}

// 64 BIT DOUBLE - BIG ENDIAN

static inline int read_f64be_from_file(FILE *f, double * result) {
    uint64_t temp;
    if (read_u64be_from_file(f, &temp) != 0) { return 1; }
    *result = bits_to_double(temp);
    return 0;
}

static inline int write_f64be_to_file(FILE *f, double val) {
    return write_u64be_to_file(f, double_to_bits(val));
}

//
// SWAPS
//

// Converts a 16-bit integer from Host Order to Big Endian (and vice versa)
static inline uint16_t swap_u16(uint16_t val) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(val);
#elif defined(_MSC_VER)
    return _byteswap_uint16(val);
#else
    return ((val >> 8)  & 0x00FF) | ((val << 8)  & 0xFF00);
#endif
}

static inline int16_t swap_i16(int16_t val) {
    return (int16_t)swap_u16((uint16_t)val);
}

// Converts a 32-bit integer from Host Order to Big Endian (and vice versa)
static inline uint32_t swap_u32(uint32_t val) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(val);
#elif defined(_MSC_VER)
    return _byteswap_uint32(val);
#else
    return ((val >> 24) & 0x000000FF) | ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) | ((val << 24) & 0xFF000000);
#endif
}

static inline int32_t swap_i32(int32_t val) {
    return (int32_t)swap_u32((uint32_t)val);
}

// Converts a 64-bit integer from Host Order to Big Endian (and vice versa)
static inline uint64_t swap_u64(uint64_t val) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(val);
#elif defined(_MSC_VER)
    return _byteswap_uint64(val);
#else
    return ((val >> 56) & 0x00000000000000FFULL) |
           ((val >> 40) & 0x000000000000FF00ULL) |
           ((val >> 24) & 0x0000000000FF0000ULL) |
           ((val >> 8)  & 0x00000000FF000000ULL) |
           ((val << 8)  & 0x000000FF00000000ULL) |
           ((val << 24) & 0x0000FF0000000000ULL) |
           ((val << 40) & 0x00FF000000000000ULL) |
           ((val << 56) & 0xFF00000000000000ULL);
#endif
}

static inline int64_t swap_i64(int64_t val) {
    return (int64_t)swap_u64((uint64_t)val);
}

//
// IS THE SYSTEM BIG OR LITTLE ENDIAN
//

// If the first byte is 0, it's Big Endian (00 00 00 01)
static inline int is_host_big_endian(void) {
    const int val = 1;
    return (*(const uint8_t *)&val) == 0;
}

// If the first byte is 1, it's Little Endian (01 00 00 00)
static inline int is_host_little_endian(void) {
    const int val = 1;
    return (*(const uint8_t *)&val) == 1;
}

//
// FROM HOST - TO LITTLE
//

// If host is big we gotta swap bytes to give le result
static inline uint16_t host_to_u16le(uint16_t val) {
    return IS_HOST_BIG_ENDIAN ? swap_u16(val) : val;
}

static inline int16_t host_to_i16le(int16_t val) {
    return (int16_t)host_to_u16le((uint16_t)val);
}

// If host is big we gotta swap bytes to give le result
static inline uint32_t host_to_u32le(uint32_t val) {
    return IS_HOST_BIG_ENDIAN ? swap_u32(val) : val;
}

static inline int32_t host_to_i32le(int32_t val) {
    return (int32_t)host_to_u32le((uint32_t)val);
}

// If host is big we gotta swap bytes to give le result
static inline uint64_t host_to_u64le(uint64_t val) {
    return IS_HOST_BIG_ENDIAN ? swap_u64(val) : val;
}

static inline int64_t host_to_i64le(int64_t val) {
    return (int64_t)host_to_u64le((uint64_t)val);
}

static inline float host_to_f32le(float val) {
    return bits_to_float(host_to_u32le(float_to_bits(val)));
}

static inline double host_to_f64le(double val) {
    return bits_to_double(host_to_u64le(double_to_bits(val)));
}

//
// FROM HOST - TO BIG
//

// If host is little we gotta swap bytes to give be result
static inline uint16_t host_to_u16be(uint16_t val) {
    return IS_HOST_LITTLE_ENDIAN ? swap_u16(val) : val;
}

static inline int16_t host_to_i16be(int16_t val) {
    return (int16_t)host_to_u16be((uint16_t)val);
}

// If host is little we gotta swap bytes to give be result
static inline uint32_t host_to_u32be(uint32_t val) {
    return IS_HOST_LITTLE_ENDIAN ? swap_u32(val) : val;
}

static inline int32_t host_to_i32be(int32_t val) {
    return (int32_t)host_to_u32be((uint32_t)val);
}

// If host is little we gotta swap bytes to give be result
static inline uint64_t host_to_u64be(uint64_t val) {
    return IS_HOST_LITTLE_ENDIAN ? swap_u64(val) : val;
}

static inline int64_t host_to_i64be(int64_t val) {
    return (int64_t)host_to_u64be((uint64_t)val);
}

static inline float host_to_f32be(float val) {
    return bits_to_float(host_to_u32be(float_to_bits(val)));
}

static inline double host_to_f64be(double val) {
    return bits_to_double(host_to_u64be(double_to_bits(val)));
}

//
// FROM LITTLE - TO HOST
//

static inline uint16_t u16le_to_host(uint16_t val) {
    return host_to_u16le(val);
}

static inline int16_t i16le_to_host(int16_t val) {
    return host_to_i16le(val);
}

static inline uint32_t u32le_to_host(uint32_t val) {
    return host_to_u32le(val);
}

static inline int32_t i32le_to_host(int32_t val) {
    return host_to_i32le(val);
}

static inline uint64_t u64le_to_host(uint64_t val) {
    return host_to_u64le(val);
}

static inline int64_t i64le_to_host(int64_t val) {
    return host_to_i64le(val);
}

static inline float f32le_to_host(float val) {
    return host_to_f32le(val);
}

static inline double f64le_to_host(double val) {
    return host_to_f64le(val);
}

//
// FROM BIG - TO HOST
//

static inline uint16_t u16be_to_host(uint16_t val) {
    return host_to_u16be(val);
}

static inline int16_t i16be_to_host(int16_t val) {
    return host_to_i16be(val);
}

static inline uint32_t u32be_to_host(uint32_t val) {
    return host_to_u32be(val);
}

static inline int32_t i32be_to_host(int32_t val) {
    return host_to_i32be(val);
}

static inline uint64_t u64be_to_host(uint64_t val) {
    return host_to_u64be(val);
}

static inline int64_t i64be_to_host(int64_t val) {
    return host_to_i64be(val);
}

static inline float f32be_to_host(float val) {
    return host_to_f32be(val);
}

static inline double f64be_to_host(double val) {
    return host_to_f64be(val);
}

#endif
