#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Struct to represent a call in the call center.
 */
struct call {
    int id;
    char name[50];
    char reason[100];
};

/*
 * Function to create a new call object.
 */
struct call* create_call(int id, char* name, char* reason) {
    struct call* new_call = malloc(sizeof(struct call));
    new_call->id = id;
    strncpy(new_call->name, name, sizeof(new_call->name) - 1);
    new_call->name[sizeof(new_call->name) - 1] = '\0';
    strncpy(new_call->reason, reason, sizeof(new_call->reason) - 1);
    new_call->reason[sizeof(new_call->reason) - 1] = '\0';
    return new_call;
}

/*
 * Function to print call details.
 */
void print_call(struct call* c) {
    if (c) {
        printf("Call ID: %d\n", c->id);
        printf("Caller Name: %s\n", c->name);
        printf("Call Reason: %s\n", c->reason);
    }
}

/*
 * Function to receive a new call and add it to the queue.
 */
void receive_call(struct queue* q, int* call_id) {
    char name[50], reason[100];

    printf("Enter caller's name: ");
    scanf(" %[^\n]", name);

    printf("Enter call reason: ");
    scanf(" %[^\n]", reason);

    struct call* new_call = create_call((*call_id)++, name, reason);
    queue_enqueue(q, new_call);
    printf("Call received and added to the queue.\n");
}

/*
 * Function to answer a call (move from queue to stack).
 */
void answer_call(struct queue* q, struct stack* s) {
    if (queue_isempty(q)) {
        printf("No calls in queue.\n");
        return;
    }

    struct call* answered_call = queue_dequeue(q);
    stack_push(s, answered_call);
    printf("Call answered:\n");
    print_call(answered_call);
}

/*
 * Function to display the last answered call.
 */
void display_answered_calls(struct stack* s) {
    if (stack_isempty(s)) {
        printf("No calls have been answered yet.\n");
        return;
    }

    struct call* last_call = stack_top(s);
    printf("Last answered call:\n");
    print_call(last_call);
}

/*
 * Function to display the next call in queue.
 */
void display_waiting_calls(struct queue* q) {
    if (queue_isempty(q)) {
        printf("No calls are waiting.\n");
        return;
    }

    struct call* next_call = queue_front(q);
    printf("Next call to be answered:\n");
    print_call(next_call);
}

/*
 * Function to free all allocated memory before exiting.
 */
void cleanup(struct queue* q, struct stack* s) {
    while (!queue_isempty(q)) {
        free(queue_dequeue(q));
    }
    while (!stack_isempty(s)) {
        free(stack_pop(s));
    }
    queue_free(q);
    stack_free(s);
}

int main() {
    struct queue* call_queue = queue_create();
    struct stack* answered_stack = stack_create();
    int call_id = 1;
    int choice;

    while (1) {
        printf("\nCall Center Menu:\n");
        printf("1. Receive a new call\n");
        printf("2. Answer a call\n");
        printf("3. Display answered calls\n");
        printf("4. Display waiting calls\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            receive_call(call_queue, &call_id);
            break;
        case 2:
            answer_call(call_queue, answered_stack);
            break;
        case 3:
            display_answered_calls(answered_stack);
            break;
        case 4:
            display_waiting_calls(call_queue);
            break;
        case 5:
            cleanup(call_queue, answered_stack);
            printf("Exiting program. Goodbye!\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
