/*!
    @file
    @brief stack source code
*/

//==================ABOUT_DEFINES
/*
    -Dstack_multy_capacity
        +stack can resize-auto
        +stack init have not capacity

    -Dstack_init_checker
        +stack can check init or not

    -Dstack_init_checker_del_rubbish
        +stack init check if stack delete

    -Dstack_mem_birds
        +stack have canary

    -Dstack_hash_checker
        +stack check hash-sums of his elements

    -Dstack_poison
        +stack elements now is poison if not pushed

    -Dstack_dump
        +stack can dump                     
        
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>


#ifdef stack_multy_capacity

static const int stack_base_capacity       = 1000;
static const int multy_of_capacity         = 2;
static const int de_multy_of_capacity_mark = 4;
static const int de_multy_of_capacity      = 2;
static const int stack_maximum_capacity    = 100000000;
#endif

//type of stack element values----------------------------
typedef int stack_elem;

#ifdef stack_poison

static stack_elem poison_value             = -INT_MAX;
#endif

#ifdef stack_dump

#define STACK_ELEM_PRINTF_IDENT            "%d"
#endif

#ifdef stack_mem_birds
//------------------------------------------------------

/*!
    @brief this struct is mem_bird
    - cell_ptr it is a ptr to interesting byte
    - exp_ptr it is a expected value of interesting byte
*/
struct _mem_bird
{

    char *cell_ptr,
          exp_val;
};
typedef struct _mem_bird mem_bird;
#endif

/*!
    @brief this struct is stack
    - data it is a buffer, where stack keep his memory
    - size size of stack
    - capacity capacity of stack
    - st_l_byte mem_bird to first byte of struct
    - st_r_byte mem_bird to last byte of struct
    - buf_l_byte mem_bird to first byte of data
    - buf_r_byte mem_bird to last byte of data
    - hash_sum poli-hash of stack elements
*/
struct _Stack 
{

    stack_elem *data;

    size_t size,
           capacity;       

#ifdef stack_mem_birds

    mem_bird st_l_byte,
             st_r_byte,
             buf_l_byte,
             buf_r_byte;
#endif           

#ifdef stack_hash_checker

    long long hash_sum;
#endif           
};
typedef struct _Stack Stack;


#ifdef stack_init_checker

static Stack **arr_of_stack_ptr            = NULL;
static bool arr_of_stack_ptr_have_mem      = false;
static const int arr_of_stack_ptr_size_inc = 1000;
static size_t arr_of_stack_ptr_size        = 0;
static int top_unused_arr_of_stack_ptr     = 0;
#endif


#ifdef stack_hash_checker

static int stack_hash_mod                  = (int)(1e9 + 7);
static int stack_hash_base                 = 103;
#endif


typedef enum _stack_return_code
{

    STACK_OK,
    STACK_NULL_POINTER,
    STACK_DATA_UNDERFLOW,
    STACK_IS_EMPTY,

#ifdef stack_multy_capacity

    STACK_CAPACITY_OVERFLOW,
#else

    STACK_DATA_OVERFLOW,
#endif  

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    STACK_IS_NOT_INIT,
#endif  

#ifdef stack_mem_birds

    STACK_STRUCT_LEFT_MEM_BIRD_IS_DYE,
    STACK_STRUCT_RIGTH_MEM_BIRD_IS_DYE,

    STACK_BUF_LEFT_MEM_BIRD_IS_DYE,
    STACK_BUF_RIGTH_MEM_BIRD_IS_DYE,
#endif      

#ifdef stack_hash_checker

    STACK_HASH_NOT_EQU,
#endif  

#ifdef stack_poison

    STACK_INTOXICATED,
#endif  

    STACK_GET_MEMORY_ERROR
} stack_return_code;


#ifdef stack_multy_capacity

/*!
    @brief this func init stack if multy_capacity is on
    @param st ptr to stack, that need to init
    @return stack_return_code
*/
stack_return_code Stack_init     (Stack *st);
#else

/*!
    @brief this func init stack if multy_capacity is off
    @param st ptr to stack, that need to init
    @return stack_return_code
*/
stack_return_code Stack_init     (Stack *st, const size_t cap);
#endif

/*!
    @brief this func push val into stack
    @param st ptr to stack, where we want push val
    @param val val, that we want push
    @return stack_return_code
*/
stack_return_code Stack_push     (Stack *st, const stack_elem val);

/*!
    @brief this func check, stack is empty or not
    @param st ptr to stack, that we want check
    @val, ptr to bool, where we want set info about stack 
    @return stack_return_code
*/
stack_return_code Stack_is_empty (Stack *st, bool *val);

/*!
    @brief this func pop val from stack
    @param st ptr to stack, that we want pop
    @return stack_return_code
*/
stack_return_code Stack_pop      (Stack *st);

/*!
    @brief this func delete stack
    @param st ptr to stack, that we want delete
    @return stack_return_code
*/
stack_return_code Stack_delete   (Stack *st);

/*!
    @brief this func check top val of stack
    @param st ptr to stack, that we want check
    @param val ptr to val, where we want set info
    @return stack_return_code
*/
stack_return_code Stack_top      (Stack *st, stack_elem *val);

/*!
    @brief this func check size of stack 
    @param st ptr to stack, that we want check
    @param val ptr to val, where we want set info
    @return stack_return_code
*/
stack_return_code Stack_size     (Stack *st, size_t *val);

/*!
    @brief this func check capacity of stack 
    @param st ptr to stack, that we want check 
    @param val ptr to val, where we want set info
    @return stack_return_code
*/
stack_return_code Stack_capacity (Stack *st, size_t *val);

#ifdef stack_dump

/*!
    @brief this func print dump
    @param st ptr to stack, that we want dump
    @param depth how muth stack_elemnt we want dump
    @return void
*/
void Stack_dump                  (Stack *st, const size_t depth);
#endif


#ifdef stack_multy_capacity

static stack_return_code cap_resize (Stack *st, bool up);
#endif

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

static void arr_of_stack_ptr_resize (int inc);
static void arr_of_stack_ptr_push   (Stack *ptr);
static bool arr_of_stack_ptr_exist  (Stack *ptr);

    #ifdef stack_init_checker_del_rubbish
static void arr_of_stack_ptr_del    (Stack *ptr);
    #endif
#endif

#ifdef stack_hash_checker

static long long hash_of_stack      (Stack *st);
#endif


#ifdef stack_poison

static void pour_poison             (Stack *st);
static bool check_poison            (Stack *st);
#endif

//===========================================================================================

#ifdef stack_mem_birds

#define stack_mem_birds_init                                                                \
                                                                                            \
    st->st_l_byte.exp_val   = *((char*)st);                                                 \
    st->st_l_byte.cell_ptr  = (char*)st;                                                    \
                                                                                            \
    st->st_r_byte.exp_val   = *((char*)st + sizeof(Stack) - 1);                             \
    st->st_r_byte.cell_ptr  = (char*)st + sizeof(Stack) - 1;                                \
                                                                                            \
                                                                                            \
    st->buf_l_byte.exp_val  = *((char*)st->data);                                           \
    st->buf_l_byte.cell_ptr = (char*)st->data;                                              \
                                                                                            \
    st->buf_r_byte.exp_val  = *((char*)st->data + st->capacity * sizeof(stack_elem) - 1);   \
    st->buf_r_byte.cell_ptr = (char*)st->data + st->capacity * sizeof(stack_elem) - 1;      
//---------------------------------------------------------------------------------------

#define stack_mem_birds_check                                                               \
                                                                                            \
    if (st->st_l_byte.exp_val != *st->st_l_byte.cell_ptr)                                   \
        return STACK_STRUCT_LEFT_MEM_BIRD_IS_DYE;                                           \
                                                                                            \
    if (st->st_r_byte.exp_val != *st->st_r_byte.cell_ptr)                                   \
        return STACK_STRUCT_RIGTH_MEM_BIRD_IS_DYE;                                          \
                                                                                            \
                                                                                            \
    if (st->buf_l_byte.exp_val != *st->buf_l_byte.cell_ptr)                                 \
        return STACK_BUF_LEFT_MEM_BIRD_IS_DYE;                                              \
                                                                                            \
    if (st->buf_r_byte.exp_val != *st->buf_r_byte.cell_ptr)                                 \
        return STACK_BUF_RIGTH_MEM_BIRD_IS_DYE;                             
//-------------------------------------------------------------------------------------------
#endif

//===========================================================================================


#ifdef stack_multy_capacity

stack_return_code Stack_init (Stack *st)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

    st->data = malloc(sizeof(stack_elem) * stack_base_capacity);
    if (st->data == NULL)
        return STACK_GET_MEMORY_ERROR;

    st->capacity = stack_base_capacity;
    st->size = 0;

#ifdef stack_poison

    pour_poison(st);
#endif

#ifdef stack_hash_checker

    st->hash_sum = hash_of_stack(st);
#endif

#ifdef stack_mem_birds

    stack_mem_birds_init
#endif  

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    arr_of_stack_ptr_push(st);
#endif

    return STACK_OK;
}
#else 

stack_return_code Stack_init (Stack *st, const size_t cap)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

    st->data = malloc(sizeof(stack_elem) * cap);
    if (st->data == NULL)
        return STACK_GET_MEMORY_ERROR;

    st->capacity = cap;
    st->size = 0;

#ifdef stack_poison

    pour_poison(st);
#endif  

#ifdef stack_hash_checker

    st->hash_sum = hash_of_stack(st);
#endif

#ifdef stack_mem_birds
    stack_mem_birds_init
#endif  

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    arr_of_stack_ptr_push(st);
#endif  

    return STACK_OK;
}
#endif 

//===========================================================================================

stack_return_code Stack_push (Stack *st, const stack_elem val)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif

#ifdef stack_mem_birds
    stack_mem_birds_check   
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif  

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif  

#ifdef stack_multy_capacity

    stack_return_code temp_return_code = STACK_OK;

    if (st->size == st->capacity)
        temp_return_code = cap_resize(st, true);

    if (temp_return_code == STACK_OK)
        st->data[st->size++] = val;

    #ifdef stack_hash_checker

        pour_poison(st);
    #endif

    #ifdef stack_hash_checker

    st->hash_sum = hash_of_stack(st);
    #endif

    #ifdef stack_mem_birds
    stack_mem_birds_init
    #endif  

    return temp_return_code;
#else

    if (st->size == st->capacity)
        return STACK_DATA_OVERFLOW;

    st->data[st->size++] = val;

#ifdef stack_hash_checker

    st->hash_sum = hash_of_stack(st);
#endif

#ifdef stack_mem_birds
    stack_mem_birds_init
#endif          

    return STACK_OK;
#endif  
}

//===========================================================================================

stack_return_code Stack_is_empty (Stack *st, bool *val)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif  

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif

    if (st->size == 0)
        *val = true;
    else
        *val = false;

    return STACK_OK;
}

//===========================================================================================

stack_return_code Stack_pop (Stack *st)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif  

    if (st->size == 0)
        return STACK_DATA_UNDERFLOW;

    st->size--;

#ifndef stack_multy_capacity

    #ifdef stack_poison

    pour_poison(st);
    #endif

    #ifdef stack_mem_birds

    stack_mem_birds_init
    #endif
#else 

    #if defined(stack_mem_birds) || defined(stack_hash_checker)

    stack_return_code temp_return_code = STACK_OK;
    if (st->capacity / de_multy_of_capacity_mark > st->size)
        temp_return_code = cap_resize(st, false);


        #ifdef stack_hash_checker

    pour_poison(st);
        #endif

        #ifdef stack_hash_checker

    st->hash_sum = hash_of_stack(st);
        #endif

        #ifdef stack_mem_birds

    stack_mem_birds_init
        #endif

    return temp_return_code;        
    #else

    if (st->capacity / de_multy_of_capacity_mark > st->size)
        return cap_resize(st, false);
    #endif
#endif

    return STACK_OK;
}

//===========================================================================================

#ifdef stack_multy_capacity

static stack_return_code cap_resize (Stack *st, bool up)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif 


    if (up)
    {

        if (st->capacity * multy_of_capacity > stack_maximum_capacity)
            return STACK_CAPACITY_OVERFLOW;

        stack_elem *temp = malloc(sizeof(stack_elem) * (st->capacity * multy_of_capacity));
        if (temp == NULL)
            return STACK_GET_MEMORY_ERROR;

        for (int i = 0; i < st->size; ++i)
            temp[i] = st->data[i];


        free(st->data);
        st->data = temp;
        st->capacity *= multy_of_capacity;

        return STACK_OK;
    }
    else
    {

        stack_elem *temp = malloc(sizeof(stack_elem) * (st->capacity / de_multy_of_capacity));
        if (temp == NULL)
            return STACK_GET_MEMORY_ERROR;

        for (int i = 0; i < st->size; ++i)
            temp[i] = st->data[i];

        free(st->data);
        st->data = temp;
        st->capacity /= de_multy_of_capacity;

        return STACK_OK;
    }
}
#endif

//===========================================================================================

stack_return_code Stack_delete (Stack *st)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif  

    free(st->data);
    st->size = 0;
    st->capacity = 0;

#ifdef stack_mem_birds

    st->st_r_byte.exp_val   = 0;
    st->st_r_byte.cell_ptr  = NULL;

    st->st_l_byte.exp_val   = 0;
    st->st_l_byte.cell_ptr  = NULL;


    st->buf_r_byte.exp_val  = 0;
    st->buf_r_byte.cell_ptr = NULL;

    st->buf_l_byte.exp_val  = 0;
    st->buf_l_byte.cell_ptr = NULL;
#endif  

#ifdef stack_hash_checker

    st->hash_sum = 0;
#endif  

#ifdef stack_init_checker_del_rubbish

    arr_of_stack_ptr_del(st);
#endif      

    return STACK_OK;    
}

//===========================================================================================

stack_return_code Stack_top (Stack *st, stack_elem *val)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif  

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif      

    if (st->size == 0)
        return STACK_IS_EMPTY;

    *val = st->data[st->size - 1];

    return STACK_OK;
}

//===========================================================================================

stack_return_code Stack_size (Stack *st, size_t *val)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif  

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif      

    *val = st->size;

    return STACK_OK;
}

//===========================================================================================

stack_return_code Stack_capacity (Stack *st, size_t *val)
{

    if (st == NULL)
        return STACK_NULL_POINTER;

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
        return STACK_IS_NOT_INIT;
#endif  

#ifdef stack_mem_birds

    stack_mem_birds_check
#endif  

#ifdef stack_hash_checker

    long long temp_hash_of_stack = hash_of_stack(st);
    if (temp_hash_of_stack != st->hash_sum)
        return STACK_HASH_NOT_EQU;
#endif  

#ifdef stack_poison

    if (!check_poison(st))
        return STACK_INTOXICATED;
#endif      

    *val = st->capacity;

    return STACK_OK;
}

//===========================================================================================

#if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

static void arr_of_stack_ptr_resize (int inc)
{

    Stack **temp_arr = malloc(sizeof(Stack*) * (arr_of_stack_ptr_size + inc));

    for (int i = 0; i < top_unused_arr_of_stack_ptr; ++i)
        temp_arr[i] = arr_of_stack_ptr[i];

    free(arr_of_stack_ptr);
    arr_of_stack_ptr = temp_arr;
    arr_of_stack_ptr_size += inc;
}

static void arr_of_stack_ptr_push (Stack *ptr)
{

    if (top_unused_arr_of_stack_ptr == arr_of_stack_ptr_size)
        arr_of_stack_ptr_resize(arr_of_stack_ptr_size_inc);

    arr_of_stack_ptr[top_unused_arr_of_stack_ptr] = ptr;

    top_unused_arr_of_stack_ptr++;
}

static bool arr_of_stack_ptr_exist (Stack *ptr)
{

    for (int i = 0; i < top_unused_arr_of_stack_ptr; ++i)
        if (arr_of_stack_ptr[i] == ptr)
            return true;

    return false;   
}
    
    #ifdef stack_init_checker_del_rubbish
static void arr_of_stack_ptr_del (Stack *ptr)
{

    for (int i = 0; i < top_unused_arr_of_stack_ptr; ++i)
        if (arr_of_stack_ptr[i] == ptr)
        {

            for (int j = i + 1; j < top_unused_arr_of_stack_ptr; ++j)
                arr_of_stack_ptr[j - 1] = arr_of_stack_ptr[j];

            break;
        }

    top_unused_arr_of_stack_ptr--;

    if (top_unused_arr_of_stack_ptr == 0)
        arr_of_stack_ptr_resize(-arr_of_stack_ptr_size);
}
    #endif
#endif

//===========================================================================================

#ifdef stack_hash_checker

static long long hash_of_stack (Stack *st)
{

    long long temp_hash_sum = 0;

    for (int i = 0; i < st->capacity * sizeof(stack_elem); ++i)
    {

        temp_hash_sum *= stack_hash_base;
        temp_hash_sum %= stack_hash_mod;

        temp_hash_sum += *((char*)st->data + i);
        temp_hash_sum %= stack_hash_mod;
    }

    return temp_hash_sum;
}
#endif

//===========================================================================================

#ifdef stack_poison

static void pour_poison (Stack *st)
{

    for (int i = st->size; i < st->capacity; ++i)
        st->data[i] = poison_value;
}

static bool check_poison (Stack *st)
{

    for (int i = st->size; i < st->capacity; ++i)
        if (st->data[i] != poison_value)
            return false;

    return true;    
}
#endif

//===========================================================================================

#ifdef stack_dump

void Stack_dump (Stack *st, const size_t depth)
{

    
    printf("\n\nStack [%p] ", st);

    if (st == NULL)
    {

        printf("(STACK_NULL_POINTER)\n\n");
        return;
    }

    #if defined(stack_init_checker) || defined(stack_init_checker_del_rubbish)

    if (!arr_of_stack_ptr_exist(st))
    {

        printf("(STACK_IS_NOT_INIT)\n\n");
        return;
    }   
    #endif

    printf("\n{\n\n");
    printf("    size = %lu\n\n", st->size);

    printf("    data [%lu] = %p\n", st->capacity, st->data);
    printf("    {\n\n");

    for (int i = 0; i < depth; ++i)
    {

        if (i < st->size)
            printf("        *[%d] = "STACK_ELEM_PRINTF_IDENT, 
                   i, st->data[i]);
        else
            printf("        [%d] = "STACK_ELEM_PRINTF_IDENT, 
                   i, st->data[i]);

    #ifdef stack_poison

        if (st->data[i] == poison_value)
            printf(" [poison]");
    #endif      

        printf("\n");
    }

    printf("    }\n\n");

    #ifdef stack_hash_checker

    printf("    hash_exp      = %lld\n", st->hash_sum);
    printf("    hash_of_stack = %lld\n", hash_of_stack(st));
    printf("\n\n");
    #endif

    #ifdef stack_mem_birds

    if (st->st_l_byte.exp_val != *st->st_l_byte.cell_ptr)                                   
        printf("    Stack_struct_left_bird  = DYE\n");
    else
        printf("    Stack_struct_left_bird  = LIFE\n");

                                                                                            
    if (st->st_r_byte.exp_val != *st->st_r_byte.cell_ptr)                                   
        printf("    Stack_struct_rigth_bird = DYE\n\n");
    else
        printf("    Stack_struct_rigth_bird = LIFE\n\n");                                             
                                                                                            
                                                                                            
    if (st->buf_l_byte.exp_val != *st->buf_l_byte.cell_ptr)                                 
        printf("    Stack_data_left_bird    = DYE\n");
    else
        printf("    Stack_data_left_bird    = LIFE\n");                                             
                                                                                            
    if (st->buf_r_byte.exp_val != *st->buf_r_byte.cell_ptr)                                 
        printf("    Stack_data_rigth_bird   = DYE\n");
    else
        printf("    Stack_data_rigth_bird   = LIFE\n");                             
    #endif

    printf("}\n\n");
}
#endif

#undef stack_mem_birds_check
#undef stack_mem_birds_init
