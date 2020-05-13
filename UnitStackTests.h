//
// Created by dunka on 27.09.2019.
//

#ifndef MIPT_DED_32_UNITSTACKTESTS_H
#define MIPT_DED_32_UNITSTACKTESTS_H


#include "MyStack.h"
#include "Tests.h"
#include "stdio.h"

void UNIT_TEST ();
void TestStackInit (bool (*StackInit) (Stack_t * stk));
void TestStackPush (bool (*StackPush) (Stack_t * stk, Stack_elem_t value));
void TestStackPop  (Stack_elem_t (*StackPop ) (Stack_t * stk));

void UNIT_TEST ()
{
    TestStackInit (StackInit);
    TestStackPush (StackPush);
    TestStackPop  (StackPop);
}

void TestStackInit (bool (*StackInit) (Stack_t * stk))
{
    printf ("Test of function StackInit\n");
    int counter = 1;

    Stack_t test  = {};
    __TEST_BOOL__ (StackInit (&test), 1);
    StackDestruct (&test);

    Stack_t test1  = {};
    __TEST_BOOL__ (StackInit (&test1), 1);
    StackDestruct (&test1);

    Stack_t test2  = {};
    __TEST_BOOL__ (StackInit (&test2), 1);
    StackDestruct (&test2);

    Stack_t test3  = {};
    __TEST_BOOL__ (StackInit (&test3), 1);  // if (value op ref)
    StackDestruct (&test3);
}

void TestStackPush (bool (*StackPush) (Stack_t * stk, Stack_elem_t value))
{
    printf ("Test of function StackPush\n");
    int counter = 1;

    Stack_t test  = {};
    test.count = -1;
    __TEST_BOOL__ (StackPush (&test, 10), 0);
    StackDestruct (&test);

    Stack_t test1  = {};
    test1.count = StackBeginningSize + 1;
    __TEST_BOOL__ (StackPush (&test1, 1), 0);
    StackDestruct (&test1);
}

void TestStackPop (Stack_elem_t (*StackPop) (Stack_t * stk))
{
    printf ("Test of function StackPop\n");
    int counter = 1;

    Stack_t test  = {};
    test.count = -1;
    __TEST_BOOL__ (StackPop (&test), 0);
    StackDestruct (&test);

    Stack_t test1  = {};
    test1.count = StackBeginningSize + 1;
    __TEST_BOOL__ (StackPop (&test1), 0);
    StackDestruct (&test1);

    Stack_t test2  = {};

    StackInit (&test2);
    StackPush (&test2, 10);

    Stack_elem_t check = {};
    if ((check = StackPop (&test2)) == 10)
    {
        printf ("Test %d is true\n", counter);
    }
    else
    {
        printf ("Test %d is false\n", counter);
    }
    StackDestruct (&test2);
}

#endif //MIPT_DED_32_UNITSTACKTESTS_H
