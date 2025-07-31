#pragma once

// types ---
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
typedef long double f128;
// ---

// defer ---
#define __DEFER__(func_name, var_name) \
  auto void func_name(i32*); \
  __attribute__((__cleanup__(func_name))) int var_name; \
  void func_name(i32*) 
#define DEFER_ONE(N) __DEFER__(__DEFER__FUNC ## N, __DEFER__VAR ## N)
#define DEFER_TWO(N) DEFER_ONE(N)
#define defer DEFER_TWO(__COUNTER__)
// ---

