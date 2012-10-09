#define Type int
#include "../include/templates/stack.h"

#include "test.h"

int main(void)
{
    stack_t(int)* stack = METHOD_TEMPLATE(stack, int, create)();
    int i;

    TEST_EQUAL(stack->size, 0);
    TEST_EQUAL(stack->top, NULL);
    TEST_EQUAL(stack->objectCopyConstructor, NULL);
    TEST_EQUAL(stack->objectDestructor, NULL);

    for (i = 0; i < 10; ++i)
    {
        METHOD_TEMPLATE(stack, int, push)(stack, i);

        TEST_EQUAL(stack->size, i + 1);
        TEST_EQUAL(*stack->top, i);
    }

    for (i = 9; i >= 0; --i)
    {
        TEST_EQUAL(*stack->top, i);

        METHOD_TEMPLATE(stack, int, pop)(stack);

        TEST_EQUAL(stack->size, i);
    }

    TEST_EQUAL(stack->top, NULL);
    TEST_EQUAL(stack->size, 0);

    for (i = 0; i < 5; ++i)
        METHOD_TEMPLATE(stack, int, push)(stack, i);

    METHOD_TEMPLATE(stack, int, clear)(stack);

    TEST_EQUAL(stack->top, NULL);
    TEST_EQUAL(stack->size, 0);

    METHOD_TEMPLATE(stack, int, release)(stack);

    printf("all tests passed\n");
    return 0;
}
