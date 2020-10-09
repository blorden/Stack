#include <stdio.h>
#include <stdbool.h>

typedef int stack_elem;

#ifdef stack_mem_birds

struct _mem_bird
{

    char *cell_ptr,
          exp_val;
};
typedef struct _mem_bird mem_bird;
#endif

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


extern stack_return_code Stack_init     (Stack *st);

extern stack_return_code Stack_push     (Stack *st, const stack_elem val);

extern stack_return_code Stack_is_empty (Stack *st, bool *val);

extern stack_return_code Stack_pop      (Stack *st);

extern stack_return_code Stack_delete   (Stack *st);

extern stack_return_code Stack_top      (Stack *st, stack_elem *val);

extern stack_return_code Stack_size     (Stack *st, size_t *val);

extern stack_return_code Stack_capacity (Stack *st, size_t *val);

extern void Stack_dump                  (Stack *st, const size_t depth);

int main ()
{

    Stack a;
    Stack_init(&a);

    for (int i = 0; i < 10; ++i)
        Stack_push(&a, i);

    Stack_dump(&a, 15);

    Stack_delete(&a);

    return 0;
}