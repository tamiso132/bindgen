#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define T_ERROR -1

#if !defined(NDEBUG)

#define VEC_DEBUG_FIELD bool initialized;
#define VEC_DEBUG_INIT(vec) vec.initialized = true
#define VEC_INITIALIZED_CHECK(vec)                                             \
  if (!vec.initialized) {                                                      \
    log_error("NOT INITIALIZED A VECTOR");                                     \
    exit(EXIT_FAILURE);                                                        \
  }

#define VEC_DEBUG_CHECK_VALID_INDEX(vec, index)                                \
  if (index >= vec.len || index < 0) {                                         \
    log_error("TRYING TO ACCESS INDEX(%d) WITH LEN(%d)\n", index, vec.len);    \
  }

#else
#define VEC_DEBUG_FIELD
#define VEC_INITIALIZED_CHECK(vec)
#define VEC_DEBUG_INIT(vec)
#endif

#define VEC_IMPLEMENT(type)                                                    \
  typedef struct {                                                             \
    type *data;                                                                \
    u32 capacity;                                                              \
    u32 len;                                                                   \
    u32 element_len;                                                           \
    VEC_DEBUG_FIELD                                                            \
  } Vec##type;

#define VEC_IMPLEMENT_WITH_NAME(type, name)                                    \
  typedef struct {                                                             \
    type *data;                                                                \
    u32 capacity;                                                              \
    u32 len;                                                                   \
    u32 element_len;                                                           \
    VEC_DEBUG_FIELD                                                            \
  } Vec##name;

#define VEC_INIT(vec, element_size, init_capacity)                             \
  vec.capacity = init_capacity;                                                \
  vec.data = calloc(init_capacity, element_size);                              \
  vec.element_len = element_size;                                              \
  vec.len = 0;                                                                 \
  VEC_DEBUG_INIT(vec)

#define VEC_ADD(vec, add)                                                      \
  VEC_INITIALIZED_CHECK(vec)                                                   \
  if ((vec).len >= (vec).capacity) {                                           \
    u32 macro_new_capacity = 10 + (vec).capacity * 2;                          \
    void *macro_buffer =                                                       \
        (void *)malloc((vec).element_len * macro_new_capacity);                \
    memcpy(macro_buffer, (vec).data, ((vec).len) * (vec).element_len);         \
    free(vec.data);                                                            \
    (vec).data = macro_buffer;                                                 \
    (vec).capacity = macro_new_capacity;                                       \
  }                                                                            \
                                                                               \
  memcpy(&VEC_GET(vec, vec.len), &add, (vec).element_len);                     \
  (vec).len += 1;

#define VEC_REMOVE(vec, index)                                                 \
  {                                                                            \
    tassert(vec.len > 0, "Len should be bigger then 0 when Remove is used");   \
    u32 macro_len = (vec).len;                                                 \
    u32 macro_start_src = macro_len - index;                                   \
    u32 macro_diff = (vec).len - 1 - index;                                    \
                                                                               \
    memmove(&(vec).data[index], &(vec).data[macro_start_src],                  \
            (vec).element_len *macro_diff);                                    \
    (vec).len--;                                                               \
  }

#define VEC_EXTEND_BY(macro_vec, macro_p_data, macro_len)                      \
  {                                                                            \
    if ((macro_vec).len + (macro_len) >= (macro_vec).capacity) {               \
      u32 macro_new_capacity = (macro_vec.len + (macro_len)) * 2;              \
      void *macro_buffer =                                                     \
          (void *)calloc((macro_vec).element_len, macro_new_capacity);         \
      memcpy(macro_buffer, (macro_vec).data,                                   \
             ((macro_vec).len) * (macro_vec).element_len);                     \
      free(macro_vec.data);                                                    \
      (macro_vec).data = macro_buffer;                                         \
      (macro_vec).capacity = macro_new_capacity;                               \
    }                                                                          \
  }                                                                            \
                                                                               \
  memcpy(&VEC_GET(macro_vec, macro_vec.len), macro_p_data,                     \
         macro_vec.element_len *macro_len);                                    \
  (macro_vec).len += macro_len;

#define VEC_ADD_RING(vec, add)                                                 \
  VEC_INITIALIZED_CHECK(vec)                                                   \
  if ((vec).len >= (vec).capacity) {                                           \
    (vec).len = 0;                                                             \
  }                                                                            \
                                                                               \
  memcpy(&vec.data[(vec).len], &add, vec.element_len);                         \
  (vec).len += 1;

#define VEC_GET(vec, index) ((char *)(vec.data))[index * vec.element_len]

#define VEC_SET_ELEMENT(vec, add, index)                                       \
  memcpy(&vec.data[index], &add, vec.element_len);

VEC_IMPLEMENT(u64);

#define VEC_EXTEND_TO_INDEX(vec, index)                                        \
  if ((vec).len <= index) {                                                    \
    if ((vec).capacity <= index) {                                             \
      u32 macro_new_capacity = 10 + (vec).capacity * 2;                        \
      void *macro_buffer =                                                     \
          (void *)malloc((vec).element_len * macro_new_capacity);              \
      memcpy(macro_buffer, (vec).data, (vec).len *(vec).element_len);          \
      free((vec).data);                                                        \
      (vec).data = macro_buffer;                                               \
      (vec).capacity = macro_new_capacity;                                     \
    }                                                                          \
    vec.len = index + 1;                                                       \
  }

typedef struct Bitflag {
  Vecu64 vec_flags;
} TBitflag;

typedef struct {
  u32 min;
  u32 max;
} Range;

VEC_IMPLEMENT(Range);

typedef struct {
  VecRange ranges;
  void *data;
  u32 len;
  u32 element_len;
  u32 capacity;
} FreeList;

typedef struct {
  u32 frame_index;
  bool is_swap;
} TFrameIndex;
