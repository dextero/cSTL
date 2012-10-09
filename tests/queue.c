#define Type int
#include "../include/templates/queue.h"

#include "test.h"

int main(void)
{
    queue_t(int)* queue = METHOD_TEMPLATE(queue, int, create)();
    int i;

    TEST_EQUAL(queue->front, NULL);
    TEST_EQUAL(queue->back, NULL);
    TEST_EQUAL(queue->size, 0);
    TEST_EQUAL(queue->objectCopyConstructor, NULL);
    TEST_EQUAL(queue->objectDestructor, NULL);

    for (i = 0; i < 10; ++i)
    {
        METHOD_TEMPLATE(queue, int, push)(queue, i);

        TEST_NOT_EQUAL(queue->front, NULL);
        TEST_EQUAL(*queue->back, i);
        TEST_EQUAL(queue->size, i + 1);
    }

    for (i = 9; i >= 0; --i)
    {
        int n;

        TEST_NOT_EQUAL(queue->front, NULL);
        TEST_NOT_EQUAL(queue->back, NULL);

        n = *queue->front;

        METHOD_TEMPLATE(queue, int, pop)(queue);
        
        TEST_EQUAL(queue->size, i);
        TEST_EQUAL(n, 9 - i);
    }

    TEST_EQUAL(queue->front, NULL);
    TEST_EQUAL(queue->back, NULL);
    TEST_EQUAL(queue->size, NULL);

    for (i = 0; i < 5; ++i)
        METHOD_TEMPLATE(queue, int, push)(queue, i);

    METHOD_TEMPLATE(queue, int, clear)(queue);

    TEST_EQUAL(queue->front, NULL);
    TEST_EQUAL(queue->back, NULL);
    TEST_EQUAL(queue->size, NULL);

    METHOD_TEMPLATE(queue, int, release)(queue);

    printf("all tests passed\n");
    return 0;
}
