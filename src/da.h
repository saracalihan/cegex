#ifndef DA_H
#define DA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DA_EXPAND(da, time) \
do { \
    if ((da).count >= (da).capacity) { \
        (da).capacity = (da).capacity == 0 ? 1 : (da).capacity*(time); \
        (da).items = realloc((da).items, (da).capacity * sizeof(*(da).items)); \
        if (!(da).items) { \
            printf("Memory allocation failed\n"); \
            exit(EXIT_FAILURE); \
        } \
    } \
} while(0);

#define DA_PUSH(da, data) \
do{\
  if ((da).count >= (da).capacity) { \
      DA_EXPAND((da),2) \
      if (!(da).items) { \
          printf("Memory allocation failed\n"); \
          exit(EXIT_FAILURE); \
      } \
  } \
  (da).items[(da).count++] = data; \
} while(0);

#define DA_POP(da) ((da).items[--(da).count])

#define DA_SHIFT(da) \
    ((da).count > 0 ? memmove((da).items, (da).items + 1, --(da).count * sizeof(*(da).items)), (da).items[0])

#define DA_UNSHIFT(da, item) \
do { \
    DA_EXPAND(da,2); \
    memmove((da).items + 1, (da).items, (da).count * sizeof(*(da).items)); \
    (da).items[0] = (item); \
    (da).count++; \
} while(0)

#define DA_REMOVE(da, index) \
do { \
    if ((index) < (da).count) { \
        memmove((da).items + (index), (da).items + (index) + 1, ((da).count - (index) - 1) * sizeof(*(da).items)); \
        (da).count--; \
    } \
} while(0)

#define DA_CONCAT(dest, src) \
do { \
    if((src).count >0){ \
      /* if dest smaller then src, realloc dest */ \
      if((dest).count + (dest).capacity < (src).count){ \
        (dest).capacity = (dest).count + (src).count; \
        (dest).items = realloc((dest).items, sizeof(*((dest).items)) * (dest).capacity); \
      } \
      memcpy( \
        (dest).items + ((dest).count == 0 ? 0 : (dest).count), \
        (src).items, \
        sizeof(*((dest).items))*(src).count \
      ); \
      (dest).count+= (src).count; \
    } \
} while(0)
#endif

#define DA_GET_LAST(da) ((da).items[(da).count -1])