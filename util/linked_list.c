#include "linked_list.h"
#include <stdlib.h>

void list_insert_before (struct linked_list* list, struct list_node *node, struct list_node *new_node) {
    struct list_node* prev = node->prev;

    // to make [new_node]-->[node]
    node->prev = new_node;
    new_node->next = node;
    prev->next = new_node;
    new_node->prev = prev;
}

void list_add (struct linked_list* list, void* data, int pos){
    const int regard_next = pos - 1;
    if (is_list_out_of_bound(regard_next, list->size)){
        fprintf(stderr,"list_add: index out of bound");
        exit(-1);
    }

    if (is_list_first(pos)) {
        list->l_op->add_first(list, data);
    } else if (is_list_last(regard_next, list->size)) {
        list->l_op->add_last(list, data);
    } else { // in normal case add elements
        struct list_node *node = list->head;
        struct list_node *new_node = list->l_op->create_node(data);

        list_go_to(node, pos); // node go to the target node in list

        list->l_op->insert_before(list, node, new_node);
        inc_list_size(list->size); // increment list size
    }
}

void list_add_first (struct linked_list* list, void* data){
    struct list_node* new_node = list->l_op->create_node(data);
    struct list_node* head     = list->head;

    if (is_empty(head)) { // 'is_empty' is different from 'list_is_empty'
        list->head = new_node;
    } else { // attach new and move the original, link the first
        struct list_node* last = list->tail;
        last = (is_empty(last)? head : last);
        new_node->next = head;
        head->prev = new_node;
        list->head = new_node;
        list->tail = last;
    }
    inc_list_size(list->size);
}

void list_add_last  (struct linked_list* list, void* data){
    struct list_node* new_node = list->l_op->create_node(data);
    struct list_node* head     = list->head;
    struct list_node* tail     = list->tail;

    if (is_empty(head)) {
        list->head = new_node;
    } else { // add the last
        struct list_node* last_node = tail;
        last_node = (is_empty(tail) ? head : last_node);
        last_node->next = new_node;
        new_node->prev = last_node;
        list->tail = new_node;
    }
    inc_list_size(list->size);
}

void* list_get (struct linked_list *list, int pos){
    if (is_list_empty(list->size)) {
        fprintf(stderr, "list_get: the list is empty");
        exit(-1);
    } else if (is_list_out_of_bound(pos, list->size)) {
        fprintf(stderr, "list_get: index out of bound");
        exit(-1);
    }

    if (is_list_first(pos)) {
        return list->l_op->get_first(list);
    } else if (is_list_last(pos, list->size)) {
        return list->l_op->get_last(list);
    } else { // get the specific position value
        struct list_node* node = list->head;

        list_go_to(node, pos);
        
        return node->data;
    }
}

void* list_get_first (struct linked_list *list) {
    if (is_list_empty(list->size)) {
        fprintf(stderr, "list_get_first: the list is empty");
        exit(-1);
    }
    return list->head->data;
}
void* list_get_last  (struct linked_list *list) {
    if (is_list_empty(list->size)) {
        fprintf(stderr, "list_get_last: the list is empty");
        exit(-1);
    }

    if (is_list_has_only_head(list->size)) {
        return list->l_op->get_first(list);
    }

    return list->tail->data;
}

void list_entry_attach(struct list_node *prev, struct list_node *next) {
    prev->next = next;
    next->prev = prev;
}

void* list_remove (struct linked_list *list, int pos) {
    if (is_list_empty(list->size)) {
        fprintf (stderr, "list_remove: the list is empty.");
        exit(-1);
    } else if (is_list_out_of_bound(pos, list->size)) {
        fprintf(stderr, "list_remove: the list is empty");
        exit(-1);
    }

    if (is_list_first(pos)) {
        return list->l_op->remove_first(list);
    } else if (is_list_last(pos, list->size)) {
        return list->l_op->remove_last(list);
    } else {
        struct list_node *node = list->head;
        void             *data;

        list_go_to(node, pos);
        data = node->data;

        // attach prev and next each other
        list_entry_attach(node->prev, node->next);

        free(node);
        dec_list_size(list->size); // decrement list size

        return data;
    }
}

void* list_remove_first (struct linked_list *list) {
    struct list_node *head = list->head;
    struct list_node *next;
    void             *data;

    if (is_empty(head)) {
        fprintf(stderr, "list_remove_last: the list is empty.");
        exit(-1);
    }

    data = head->data;
    next = head->next;
    list->head = next;

    if(is_list_has_next_item(next))
        next->prev = NULL;
    
    free(head);

    dec_list_size(list->size);

    if(is_list_has_only_head(list->size))
        list->tail = NULL;

    return data;
}
void* list_remove_last (struct linked_list *list) {
    if (is_list_empty(list->size)) {
        fprintf(stderr, "list_remove_first: the list is empty.");
        exit(-1);
    }

    if (is_list_has_only_head(list->size)) {
        return list->l_op->remove_first(list);
    } else {
        struct list_node *tail = list->tail;
        struct list_node *prev = tail->prev;
        void*             data = tail->data;

        prev->next = NULL;
        if (is_list_has_over_one(list->size))
            list->tail = prev;
        dec_list_size(list->size);
        if (is_list_empty(list->size) || is_list_has_only_head(list->size))
            list->tail = NULL;
        
        return data;
    }
}

struct list_node* list_create_node (void *data){
    struct list_node* node = (struct list_node *) malloc (sizeof(struct list_node));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

struct list_operation list_l_op = {
    .add           = list_add,
    .add_last      = list_add_last,
    .add_first     = list_add_first,
    .insert_before = list_insert_before,
    .get           = list_get,
    .get_last      = list_get_last,
    .get_first     = list_get_first,
    .remove        = list_remove,
    .remove_last   = list_remove_last,
    .remove_first  = list_remove_first,
    .create_node   = list_create_node
};

// constructor
struct linked_list create_linked_list(){
    struct linked_list list;
    list.size = 0;
    list.head = NULL;
    list.tail = NULL;
    list.l_op = &list_l_op;
    return list;
}
