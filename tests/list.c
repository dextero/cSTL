#define Type int
#include "../include/templates/list.h"

#include "test.h"

int main(void)
{
    /* create */
    list_t(int)* list = METHOD_TEMPLATE(list, int, create)();
    int i;

    TEST_NOT_EQUAL(list, NULL);
    TEST_EQUAL(list->first, NULL);
    TEST_EQUAL(list->last, NULL);
    TEST_EQUAL(list->objectCopyConstructor, NULL);
    TEST_EQUAL(list->objectDestructor, NULL);

    /* pushBack */
    const list_item_t(int)* prev = list->last;

    for (i = 0; i < 10; ++i)
    {
        METHOD_TEMPLATE(list, int, pushBack)(list, i);

        TEST_EQUAL(list->size, i + 1);
        TEST_NOT_EQUAL(list->first, NULL);
        TEST_NOT_EQUAL(list->last, NULL);
        
        TEST_EQUAL(list->first->prev, NULL);
        TEST_EQUAL(list->last->next, NULL);
        TEST_EQUAL(list->last->value, i);

        TEST_EQUAL(list->last->prev, prev);
        if (prev)
            TEST_EQUAL(prev->next, list->last);
        prev = list->last;
    }
    /* [ 0, 1, ..., 9 ] */

    /* pushFront */
    prev = list->first;

    for (i = 10; i < 20; ++i)
    {
        METHOD_TEMPLATE(list, int, pushFront)(list, i);

        TEST_EQUAL(list->size, i + 1);
        TEST_NOT_EQUAL(list->first, NULL);
        TEST_NOT_EQUAL(list->last, NULL);

        TEST_EQUAL(list->first->prev, NULL);
        TEST_EQUAL(list->last->next, NULL);
        TEST_EQUAL(list->first->value, i);

        TEST_EQUAL(list->first->next, prev);
        TEST_EQUAL(prev->prev, list->first);

        prev = list->first;
    }

    /* insertAfter - end */
    prev = list->last;

    METHOD_TEMPLATE(list, int, insertAfter)(list, list->last, 10);

    TEST_EQUAL(list->size, 21);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);
    TEST_EQUAL(list->last->value, 10);
    TEST_EQUAL(list->last->prev, prev);
    TEST_EQUAL(prev->next, list->last);
    TEST_EQUAL(list->last->next, NULL);
    /* [ 19, 18, ..., 10, 0, 1, 2, ..., 9, 10 ] */

    /* insertAfter - middle */
    prev = list->first;

    METHOD_TEMPLATE(list, int, insertAfter)(list, list->first, 20);

    TEST_EQUAL(list->size, 22);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);

    TEST_NOT_EQUAL(list->first->next, NULL);
    TEST_EQUAL(list->first->next->value, 20);
    TEST_EQUAL(list->first, list->first->next->prev);
    TEST_NOT_EQUAL(list->first->next->next, NULL);
    TEST_EQUAL(list->first->next->next->value, 18);
    TEST_EQUAL(list->first->next, list->first->next->next->prev);
    /* [ 19, 20, 18, ..., 10, 0, 1, 2, ..., 9, 10 ] */

    /* insertBefore - begin */
    prev = list->first;

    METHOD_TEMPLATE(list, int, insertBefore)(list, list->first, 21);

    TEST_EQUAL(list->size, 23);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);
    TEST_EQUAL(list->first->value, 21);
    TEST_EQUAL(list->first->next, prev);
    TEST_EQUAL(prev->prev, list->first);
    TEST_EQUAL(list->first->prev, NULL);
    /* [ 21, 19, 20, 18, ..., 10, 0, 1, 2, ..., 9, 10 ] */

    /* insertBefore - middle */
    prev = list->last;

    METHOD_TEMPLATE(list, int, insertBefore)(list, list->last, 11);

    TEST_EQUAL(list->size, 24);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);

    TEST_NOT_EQUAL(list->last->prev, NULL);
    TEST_EQUAL(list->last->prev->value, 11);
    TEST_EQUAL(list->last, list->last->prev->next);
    TEST_NOT_EQUAL(list->last->prev->prev, NULL);
    TEST_EQUAL(list->last->prev->value, 11);
    TEST_EQUAL(list->last->prev, list->last->prev->prev->next);
    /* [ 21, 19, 20, 18, ..., 10, 0, 1, 2, ..., 9, 11, 10 ] */

    /* popBack */
    prev = list->last->prev;

    METHOD_TEMPLATE(list, int, popBack)(list);
    
    TEST_EQUAL(list->size, 23);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_EQUAL(list->last, prev);

    TEST_EQUAL(list->last->value, 11);
    TEST_EQUAL(list->last->next, NULL);
    /* [ 21, 19, 20, 18, ..., 10, 0, 1, 2, ..., 9, 11 ] */

    /* popFront */
    prev = list->first->next;

    METHOD_TEMPLATE(list, int, popFront)(list);
    
    TEST_EQUAL(list->size, 22);
    TEST_NOT_EQUAL(list->last, NULL);
    TEST_EQUAL(list->first, prev);

    TEST_EQUAL(list->first->value, 19);
    TEST_EQUAL(list->first->prev, NULL);
    /* [ 19, 20, 18, ..., 10, 0, 1, 2, ..., 9, 11 ] */

    /* erase - middle */
    METHOD_TEMPLATE(list, int, erase)(list, list->first->next);

    TEST_EQUAL(list->size, 21);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);

    TEST_NOT_EQUAL(list->first->next, NULL);
    TEST_EQUAL(list->first->next->value, 18);
    TEST_EQUAL(list->first, list->first->next->prev);
    /* [ 19, 18, 17, ..., 10, 0, 1, 2, ..., 9, 11 ] */

    /* erase - front */
    METHOD_TEMPLATE(list, int, erase)(list, list->first);

    TEST_EQUAL(list->size, 20);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);

    TEST_EQUAL(list->first->prev, NULL);
    TEST_NOT_EQUAL(list->first->next, NULL);
    TEST_EQUAL(list->first->value, 18);
    TEST_EQUAL(list->first, list->first->next->prev);
    /* [ 18, 17, ..., 10, 0, 1, 2, ..., 9, 11 ] */

    /* erase - back */
    METHOD_TEMPLATE(list, int, erase)(list, list->last);

    TEST_EQUAL(list->size, 19);
    TEST_NOT_EQUAL(list->first, NULL);
    TEST_NOT_EQUAL(list->last, NULL);

    TEST_EQUAL(list->last->next, NULL);
    TEST_NOT_EQUAL(list->last->prev, NULL);
    TEST_EQUAL(list->last->prev->value, 8);
    TEST_EQUAL(list->last, list->last->prev->next);
    /* [ 18, 17, ..., 10, 0, 1, 2, ..., 9 ] */

    METHOD_TEMPLATE(list, int, clear)(list);
    TEST_EQUAL(list->size, 0);
    TEST_EQUAL(list->first, NULL);
    TEST_EQUAL(list->last, NULL);

    METHOD_TEMPLATE(list, int, release)(list);

    printf("all tests passed\n");
    return 0;
}

