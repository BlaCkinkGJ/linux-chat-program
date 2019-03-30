#define UNIT_TEST_1_DISABLE
#ifndef UNIT_TEST_1_DISABLE
#include "linked_list.h"

//@TODO: linked list가 shared memory로 유지될 수 있는 지를 확인해보도록 해봅시다.
int main(void) {
    struct linked_list list;
    char *data1 = (char *)malloc(sizeof(char)*256);
    char *data2 = (char *)malloc(sizeof(char)*256);
    sprintf(data1, "sample text 1");
    sprintf(data2, "sample text 2");

    list = create_linked_list();
    list.l_op->add_last(&list, (char *)data1);
    list.l_op->add_last(&list, (char *)data2);

    printf("%s\n", (char *)(list.l_op->get(&list, 0)));
    printf("%s\n", (char *)(list.l_op->get(&list, 1)));

    printf("%s\n", (char *)(list.l_op->remove_first(&list)));
    printf("%s\n", (char *)(list.l_op->remove_first(&list)));
    free(data1);
    free(data2);
    return 0;
}
#endif