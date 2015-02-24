#ifndef HAAR_H
#define HAAR_H 1

#include <stdint.h>

void
haar_pass(int8_t* in, int8_t* ws, size_t len);

void
haar8(int8_t* in, int8_t* ws);

void
haar16(int8_t* in, int8_t* ws);

void
haar32(int8_t* in, int8_t* ws);

#endif /* #ifndef HAAR_H */
