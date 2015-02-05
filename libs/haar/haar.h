#ifndef HAAR_H
#define HAAR_H 1

#include <stdint.h>

void
haar_pass(int8_t* in, int8_t* ws, size_t len);

void
haar32(int8_t* in, int8_t* ws);

#endif /* #ifndef HAAR_H */
