#pragma once
#include "common.h"

#define LIST_INIT(list) \
    { .prev = &(list), .next = &(list) }

#define LIST_POP_FRONT(list, structure, field) \
    ({ \
       list_elem_t *__elem = list_pop_front(list);     \
       (__elem) ? LIST_CONTAINER(__elem, structure, field) : NULL;     \
    })

#define LIST_CONTAINER(elem, structure, field) \
    ((structure *) ((vaddr_t) (elem) - offsetof(structure, field)))

#define LIST_FOR_EACH(elem, list, structure, field) \
    for (structure *elem = LIST_CONTAINER((list)->next, structure, field),       \
                   *__next = NULL;                                               \
         (&elem->field != (list)                                                 \
          && (__next = LIST_CONTAINER((elem)->field.next, structure, field)));   \
         elem = __next)

struct list {
    struct list *prev;
    struct list *next;
};

typedef struct list list_t;
typedef struct list list_elem_t;

void list_init(list_t *list);
void list_elem_init(list_elem_t *elem);
bool list_is_linked(list_elem_t *elem);
void list_remove(list_elem_t *elem);
void list_push_back(list_t *list, list_elem_t *new_tail);
list_elem_t *list_pop_front(list_t *list);