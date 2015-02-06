#include <Arduino.h>
#include <CuTest.h>
#include <haar.h>

void
test_all_ones(CuTest* tc)
{
    int8_t in[8];
    for (int idx=0; idx<8; idx++)
    {
        in[idx] = 1;
    }

    int8_t ws[8];
    haar8(in, ws);

    int8_t out[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    for (int idx=0; idx<8; idx++)
    {
        CuAssertIntEquals(tc, out[idx], in[idx]);
    }
}

void
setup(void)
{
    CuString* output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_all_ones);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    Serial.begin(9600);
    Serial.println(output->buffer);
}

void
loop(void)
{
    return;
}
