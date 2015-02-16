#include <Arduino.h>
#include <haar.h>

#define __ASSERT_USE_STDERR
#include <assert.h>

int
test_all_ones(void)
{
    int8_t in[8];
    for (int idx=0; idx<8; idx++)
        in[idx] = 1;

    int8_t ws[8];
    haar8(in, ws);

    int8_t out[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    for (int idx=0; idx<8; idx++)
        assert(out[idx] == in[idx]);
}

void
setup(void)
{
    Serial.begin(9600);
    test_all_ones();
    Serial.println("all tests ran successfully");
}

void
loop(void)
{
    return;
}

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link.
    Serial.println(__func);
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    abort();
}
