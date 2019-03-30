#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdio.h>
#include <stdlib.h>

#define is_empty(ptr) (ptr==NULL)
#define is_list_empty(size) (size <= 0)
#define is_list_out_of_bound(ptr, size) (ptr >= size)
#define is_list_first(ptr) (ptr == 0)
#define is_list_last(ptr , size) (ptr+1 == size)
#define is_list_has_only_head(size) (size == 1)
#define list_go_to(node, pos) do{ \
    int index = 0; while (index < pos) { node = node->next; index++; } \
}while(0)
#define dec_list_size(size) (size = size - 1)
#define inc_list_size(size) (size = size + 1)
#define is_list_has_next_item(ptr) (ptr != NULL)
#define is_list_has_over_one(size) (size > 1)

struct list_node {
    void *data;
    struct list_node *next;
    struct list_node *prev;
};

struct linked_list {
    struct list_node      *head;
    struct list_node      *tail;
    size_t                 size;
    struct list_operation *l_op;
};

struct list_operation {
    void (*add)       (struct linked_list* list, void* data, int pos);
    void (*add_last)  (struct linked_list* list, void* data);
    void (*add_first) (struct linked_list* list, void* data);

    void (*insert_before) (struct linked_list* list, struct list_node *prev, struct list_node *after);

    void* (*get)       (struct linked_list *list, int pos);
    void* (*get_last)  (struct linked_list *list);
    void* (*get_first) (struct linked_list *list);

    void* (*remove)       (struct linked_list *list, int pos);
    void* (*remove_last)  (struct linked_list *list);
    void* (*remove_first) (struct linked_list *list);

    struct list_node* (*create_node) (void *data);
};

void list_add       (struct linked_list *list, void *data, int pos);
void list_add_last  (struct linked_list *list, void *data);
void list_add_first (struct linked_list *list, void *data);

void list_insert_before (struct linked_list *list, struct list_node *prev, struct list_node *after);

void* list_get       (struct linked_list *list, int pos);
void* list_get_last  (struct linked_list *list);
void* list_get_first (struct linked_list *list);

void* list_remove       (struct linked_list *list, int pos);
void* list_remove_last  (struct linked_list *list);
void* list_remove_first (struct linked_list *list);

struct list_node* (*create_node) (void *data);

struct linked_list create_linked_list();
#endif