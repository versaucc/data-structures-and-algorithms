/*
 * This file is where you should implement your linked list. It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <assert.h>
#include "list.h"

 /*
  * This structure is used to represent a single node in a singly-linked list.
  * It is not defined in list.h, so it is not visible to the user.  You should not
  * modify this structure.
  */
struct node {
    void* val;
    struct node* next;
};

/*
 * This structure is used to represent an entire singly-linked list. Note that
 * we're keeping track of just the head of the list here, for simplicity.
 */
struct list {
    struct node* head;
};

struct list* list_create() {
    struct list* lst = malloc(sizeof(struct list));
    assert(lst);

    lst->head = NULL;
    return lst;
}

void list_free(struct list* lst) {
    assert(lst);

    struct node* current = lst->head;
    while (current) {
        struct node* temp = current;
        current = current->next;
        free(temp);
    }

    free(lst);
}

void list_insert(struct list* lst, void* val) {
    assert(lst);

    struct node* new_node = malloc(sizeof(struct node));
    assert(new_node);

    new_node->val = val;
    new_node->next = lst->head;
    lst->head = new_node;
}

void list_insert_end(struct list* lst, void* val) {
    assert(lst);

    struct node* new_node = malloc(sizeof(struct node));
    assert(new_node);
    new_node->val = val;
    new_node->next = NULL;

    if (!lst->head) {
        lst->head = new_node;
    }
    else {
        struct node* current = lst->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void list_remove(struct list* lst, void* val, int (*cmp)(void* a, void* b)) {
    assert(lst);

    struct node* current = lst->head;
    struct node* previous = NULL;

    while (current) {
        if (cmp(val, current->val) == 0) {
            if (previous) {
                previous->next = current->next;
            }
            else {
                lst->head = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void list_remove_end(struct list* lst) {
    assert(lst);

    if (!lst->head) {
        return;
    }

    if (!lst->head->next) {
        free(lst->head);
        lst->head = NULL;
        return;
    }

    struct node* current = lst->head;
    while (current->next->next) {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;
}

int list_position(struct list* lst, void* val, int (*cmp)(void* a, void* b)) {
    assert(lst);

    struct node* current = lst->head;
    int index = 0;

    while (current) {
        if (cmp(val, current->val) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

void list_reverse(struct list* lst) {
    assert(lst);

    struct node* prev = NULL;
    struct node* current = lst->head;
    struct node* next = NULL;

    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    lst->head = prev;
}

