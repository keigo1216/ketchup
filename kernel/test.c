#include "test.h"

void print_start_test(char *test_case) {
    printf("########## [%s] ##########\n", test_case);
}

void print_end_test() {
    printf("##########################\n");
}

void test_list_macro() {
    print_start_test("test_list_macro");

    struct process {
        int data;
        list_t list;
        list_elem_t elem;
    };

    // initialize
    struct process p1, p2, p3;
    struct process *p;
    p1.data = 1;
    p2.data = 2;
    p3.data = 3;
    list_init(&p1.list);
    list_init(&p2.list);
    list_init(&p3.list);
    list_elem_init(&p1.elem);
    list_elem_init(&p2.elem);
    list_elem_init(&p3.elem);

    // test LIST_CONTAINER
    p = LIST_CONTAINER(&p1.elem, struct process, elem);
    i_assert_eq(p->data, p1.data, "LIST_CONTAINER");

    // test LIST_POP_FRONT
    list_push_back(&p1.list, &p2.elem);
    list_push_back(&p1.list, &p3.elem);
    p = LIST_POP_FRONT(&p1.list, struct process, elem);
    i_assert_eq(p->data, p2.data, "LIST_POP_FRONT");
    p = LIST_POP_FRONT(&p1.list, struct process, elem);
    i_assert_eq(p->data, p3.data, "LIST_POP_FRONT");
    p = LIST_POP_FRONT(&p1.list, struct process, elem);
    assert_null(p, "LIST_POP_FRONT");

    // test LIST_FOR_EACH
    list_push_back(&p1.list, &p2.elem);
    list_push_back(&p1.list, &p3.elem);
    int cnt = 0;
    LIST_FOR_EACH(p, &p1.list, struct process, elem) {
        if (cnt == 0) i_assert_eq(p->data, p2.data, "LIST_FOR_EACH");
        if (cnt == 1) i_assert_eq(p->data, p3.data, "LIST_FOR_EACH");
        cnt++;
    }
    i_assert_eq(cnt, 2, "LIST_FOR_EACH");

    print_end_test();
}