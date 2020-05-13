//
// Created by dunka on 27.09.2019.
//

#ifndef MIPT_DED_32_MYSTACK_H
#define MIPT_DED_32_MYSTACK_H

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG (code) code
#else
#define ON_DEBUG(code)
#endif

#include "string.h"

#define __INFORMATION__ __FILE__, __LINE__, __PRETTY_FUNCTION__

enum StackError_t { STK_PTR_NULL = 10, STK_NEG_COUNTER = 11, STK_OVERFLOW = 12, STK_CORRUPTED = 13, STK_MEMORY_OUT = 14 };

typedef int Stack_elem_t;

const int Poison             = -228;
const int StackBeginningSize = 10;
const int Delta              = 3;
const int SizeDif            = 2;
#define __CANARY__           -331  // MUST be #define for everybody to ask about it


#define ASSERT_OK if (Stack_OK (stk) != 0)                                     \
                  {                                                                \
                      Dump (stk, __INFORMATION__, "ERROR");                       \
                      ASSERT (0);                                                \
                  }

/*!
 * Contains all needed parameters for stack
 */
struct Stack_t {
    ON_DEBUG (int ShieldFromLeft;)

    Stack_elem_t* data;                     ///<The pointer on the array
    int StackSize;                          ///<Size of Stack
    int count;                              ///<Number of the first empty element

    mutable int ErrorCode;                  ///<Error code


    const char* name;                       ///<Name of structure

    ON_DEBUG(unsigned long int HashSum);
    ON_DEBUG(int ShieldFromRight;)
};


/*!
 * Full inscription of stack
 * @param[in] FileName Name of File
 * @param[in] Line Number of line
 * @param[in] Function Name of Function
 * @param[in] WTF What a Terrible Failure? -- Reason to use this Function
 * @param[in] stk Stack
 */
void Dump (Stack_t * stk, const char* FileName, const int Line, const char* Function, const char* WTF);

/*!
 * Explains ErrorCode
 * @param[in] ErrorCode Number of ERROR
 * @return "Stack is Okay" If Error = 0
 * @return "The pointer on the beginning of text is nullptr" If Error = 1
 * @return "Count is less than zero" If Error = 2
 * @return "Stack is overflowing" If Error = 3
 */
const char* ExplainResult (int ErrorCode);  // strerror

/*!
 * Analyze Stack
 * @param stk Stack
 * @return ErrorCode Number of ERROR
 */
int Stack_OK (const Stack_t * stk);

/*!
 * Initialization of Stack
 * @param stk Stack
 * @return true Everything is Okay
 * @return false Something has gone wrong (ASSERT_OK do this)
 */
bool StackInit (Stack_t * stk);

/*!
 * Put element on top of the Stack and ++ count
 * @param stk Stack
 * @param value Element, that we put in stack
 * @return true Everything is Okay
 * @return false Something has gone wrong (ASSERT_OK do this)
 */
bool StackPush (Stack_t * stk, Stack_elem_t value);

/*!
 * Take element from top of the Stack
 * @param stk Stack
 * @return Element from top of the stack
 */
Stack_elem_t StackPop (Stack_t * stk);

/*!
 * Destruct Stack
 * @param stk Stack
 * @return true If Stack is destructed
 * @return false If stack is not destructed
 */
bool StackDestruct (Stack_t * stk);

/*!
 * Makes HASH
 * @param[in] beginning pointer on the beginning of masiive
 * @param[in] length - length of massive to has
 * @return HASH of massive
 */
unsigned long int Hash (void* beginning, int length);


void Dump (Stack_t * stk, const char* FileName, const int Line, const char* Function, const char* ForWhat)
{
    printf ("\n%s %s (Line %d) %s\nStack_t %s [%p]", ForWhat, FileName, Line, Function, stk->name, &stk);
    printf ("\n{\nsize = %d\ndata [%d] = [%p]", stk->count-1, stk->StackSize, &stk->data);

    const char* Result = nullptr;
    printf("\t(%s)\n\t{\n", stk->ErrorCode == 0 ? "OK" : "ERROR");

    for (int i = 0; i < stk->StackSize; i++)//убрать -1 +1
    {
        printf("       %s[%d] = %d\t%s              %p\n", i+1 < stk->count ? "*" : "", i+1, stk->data[i+1],
               stk->data[i+1] == Poison ? "[Poison]" : "", &stk->data[i+1]);//меняя тип нужно менять % после равно
    }

    printf ("\t}\nErrorCode = %d\t(%s)\n}\n\n", stk->ErrorCode, ExplainResult (stk->ErrorCode));
    stk->ErrorCode = 0;
}


unsigned long int Hash (void* beginning, int length)
{
    //printf("Length is: %d\n",length);
    unsigned long int hash = 0;
    char* place = (char*) beginning;
    for (int i = 0; i < length; i++)
    {
        hash += ~*place + 5 ^ * place;
        place++;
    }

    return hash;
}

int Stack_OK (const Stack_t * stk)
{
    if (!stk->data)                                                                                       return stk->ErrorCode = STK_PTR_NULL;
    if (stk->count < 1)                                                                                   return stk->ErrorCode = STK_NEG_COUNTER;
    if (stk->count - 1> stk->StackSize)                                                                   return stk->ErrorCode = STK_OVERFLOW;
    ON_DEBUG(if ((stk->ShieldFromRight != __CANARY__) || (stk->ShieldFromLeft != __CANARY__))             return stk->ErrorCode = STK_CORRUPTED;
    if ((stk->data[0] != __CANARY__) || (stk->data[stk->StackSize +  1] != __CANARY__))                   return stk->ErrorCode = STK_CORRUPTED;
    if ((stk->count - 1 >= 0) && ( stk->HashSum != Hash (stk->data, (stk->count)* sizeof(Stack_elem_t)))) return stk->ErrorCode = STK_CORRUPTED;)

    return stk->ErrorCode = 0;
}

#define EXPLAIN_ERROR_(code, text)      \
    case (code) :                       \
    {                                   \
    return #code ": " text;             \
    }

const char* ExplainResult (int ErrorCode)
{
    switch (ErrorCode)
    {
        EXPLAIN_ERROR_ (0,               "No Error")
        EXPLAIN_ERROR_ (STK_PTR_NULL,    "The pointer on the beginning of text is nullptr");
        EXPLAIN_ERROR_ (STK_NEG_COUNTER, "Count is less than zero");
        EXPLAIN_ERROR_ (STK_OVERFLOW,    "Stack is overflowing");
        EXPLAIN_ERROR_ (STK_CORRUPTED,   "You're f*cking man, what are you doing in my structure");
        EXPLAIN_ERROR_ (STK_MEMORY_OUT,  "Memory is out((((")

        default:
        {
            return "How have you taken such a result?";
        }
    }
}

#undef EXPLAIN_ERROR_

bool StackInit (Stack_t * stk)
{
    ASSERT (stk);

    ON_DEBUG(stk->ShieldFromLeft  = __CANARY__;
    stk->ShieldFromRight = __CANARY__;)
    stk->ErrorCode = 0;
    stk->count = 1;
    stk->StackSize = StackBeginningSize;
    stk->name = "stk1";

    stk->data = (Stack_elem_t*) calloc (stk->StackSize+2, sizeof (Stack_elem_t));
    ON_DEBUG(stk->data[0] = __CANARY__;
    stk->data[stk->StackSize + 1] = __CANARY__;
    stk->HashSum = Hash(stk->data, sizeof(int));)



    memset (&stk->data[1], 0, (stk->StackSize) * sizeof (Stack_elem_t));

    ASSERT_OK

    return true;
}



bool StackPush (Stack_t * stk, Stack_elem_t value)
{

    ASSERT_OK
    if (stk->count - 1 >= stk->StackSize)
    {
        Stack_elem_t* data = (Stack_elem_t*) realloc (stk->data, 2 * sizeof (int) + sizeof (Stack_elem_t) * stk->StackSize * SizeDif);

        if (!data)
        {
            stk->ErrorCode = STK_MEMORY_OUT;
            Dump (stk,__INFORMATION__, "Memory out");
        }


        stk->data = data;
        stk->StackSize *= 2;
        stk->data[stk->StackSize/SizeDif + 1] = Poison;
        ON_DEBUG(stk->data[stk->StackSize + 1] = __CANARY__;)
    }

    ON_DEBUG (printf("\n\n\nMy hash before function: %lu\n", stk->HashSum);)
    stk->data[stk->count++] = value;
    ON_DEBUG(stk->HashSum += Hash((void *) &stk->data[stk->count - 1], sizeof(Stack_elem_t));)
    ON_DEBUG (printf("\n\n\nMy hash after function: %lu\n", stk->HashSum);)

    ON_DEBUG (Dump (stk, __INFORMATION__, "Just for looking");)
    ASSERT_OK

    return true;
}


Stack_elem_t StackPop (Stack_t* stk)
{
    ASSERT_OK

    if ((stk->StackSize > StackBeginningSize) && (stk->count + Delta  < stk->StackSize/2))
    {
        stk->data = (Stack_elem_t*) realloc (stk->data, sizeof (Stack_elem_t) * stk->StackSize / SizeDif - 1);
        stk->StackSize /= 2;
        ON_DEBUG(stk->data[stk->StackSize + 1] = __CANARY__;)
    }



    Stack_elem_t haveTakenFromStack= stk->data[stk->count - 1];

    ON_DEBUG(stk->HashSum -= Hash ((void*)&stk->data[stk->count - 1], sizeof(Stack_elem_t));)

    stk->data[--stk->count] = Poison;

    ASSERT_OK

    return haveTakenFromStack;
}

bool StackDestruct (Stack_t* stk)
{
    ASSERT_OK

    ON_DEBUG(stk->ShieldFromLeft  = Poison;
    stk->ShieldFromRight = Poison;
    stk->ErrorCode = Poison;
    stk->count = Poison;
    stk->StackSize = Poison;
    stk->name = "Poison";
    free (stk->data);)

    return true;
}


#endif //MIPT_DED_32_MYSTACK_H
