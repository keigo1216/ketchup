#include "list.h"

static void list_insert(
    list_elem_t *prev,
    list_elem_t *next,
    list_elem_t *new
) {
    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

static void list_elem_nullify(list_elem_t *elem) {
    elem->prev = NULL;
    elem->next = NULL;
}

void list_init(list_t *list) {
    list->prev = list;
    list->next = list;
}

void list_elem_init(list_elem_t *elem) {
    list_elem_nullify(elem);
}

bool list_is_linked(list_elem_t *elem) {
    return elem -> next != NULL;
}

void list_remove(list_elem_t *elem) {
    // TODO: check list_is_linked (not null)
    if (!list_is_linked(elem)) {
        return;
    }

    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    list_elem_nullify(elem);
}

void list_push_back(list_t *list, list_elem_t *new_tail) {
    list_insert(list->prev, list, new_tail);
}

list_elem_t *list_pop_front(list_t *list) {
    struct list *head = list->next;

    if (head == list) {
        return NULL;
    }

    struct list *new = head -> next;
    list -> next = new;
    new -> prev = list;

    list_elem_nullify(head);
    return head;
}