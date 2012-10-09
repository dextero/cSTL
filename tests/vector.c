#define Type int
#include "../include/templates/vector.h"

#include "test.h"

int main(void)
{
    vector_t(int)* vec = METHOD_TEMPLATE(vector, int, create)();
    int i;

    /* create */
    TEST_NOT_EQUAL(vec, NULL);
    TEST_EQUAL(vec->size, 0);
    TEST_GREATER(vec->capacity, 0);
    TEST_NOT_EQUAL(vec->array, NULL);
    TEST_EQUAL(vec->objectCopyConstructor, NULL);
    TEST_EQUAL(vec->objectDestructor, NULL);
    TEST_EQUAL(vec->objectCompareFunc, NULL);

    /* pushBack */
    for (i = 0; i < 300; ++i)
    {
        METHOD_TEMPLATE(vector, int, pushBack)(vec, i);

        TEST_NOT_EQUAL(vec->array, NULL);
        TEST_EQUAL(vec->array[i], i);
        TEST_EQUAL(vec->size, i + 1);
        TEST_GREATER_EQUAL(vec->capacity, vec->size);
    }

    /* eraseAt */
    METHOD_TEMPLATE(vector, int, eraseAt)(vec, 0);

    TEST_EQUAL(vec->size, 299);
    for (i = 0; i < 299; ++i)
        TEST_EQUAL(vec->array[i], i + 1);

    METHOD_TEMPLATE(vector, int, eraseAt)(vec, 1);
    TEST_EQUAL(vec->array[0], 1);
    TEST_EQUAL(vec->size, 298);
    for (i = 1; i < 298; ++i)
        TEST_EQUAL(vec->array[i], i + 2);

    /* eraseRange */
    METHOD_TEMPLATE(vector, int, eraseRange)(vec, 0, 8);

    TEST_EQUAL(vec->size, 290);
    for (i = 0; i < 290; ++i)
        TEST_EQUAL(vec->array[i], i + 10);

    /* insert */
    for (i = 0; i < 10; ++i)
    {
        METHOD_TEMPLATE(vector, int, insert)(vec, i, i);

        TEST_EQUAL(vec->size, 291 + i);
        TEST_EQUAL(vec->array[i], i);
    }

    for (i = 0; i < 300; ++i)
        TEST_EQUAL(vec->array[i], i);

    /* find */
    for (i = 0; i < 300; ++i)
        TEST_EQUAL(METHOD_TEMPLATE(vector, int, find)(vec, i), i);

    /* clear */
    METHOD_TEMPLATE(vector, int, clear)(vec);

    TEST_EQUAL(vec->size, 0);

    /* reserve */
    METHOD_TEMPLATE(vector, int, reserve)(vec, 1024);

    TEST_EQUAL(vec->size, 0);
    TEST_EQUAL(vec->capacity, 1024);
    TEST_NOT_EQUAL(vec->array, NULL);

    /* release */
    METHOD_TEMPLATE(vector, int, release)(vec);

    printf("all tests passed\n");
    return 0;
}
