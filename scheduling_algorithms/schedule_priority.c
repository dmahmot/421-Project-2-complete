#include "schedulers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "cpu.h"


struct node *head = NULL; //need to use to free whole list
static int pCount = 0;

void delete_and_free_node(struct node** h, struct node* n){
    delete(h, n->task);
    free(n->task->name);
    free(n->task);
    free(n);
    // n = NULL;
}

void delete_and_free_list(struct node* listHead){
    struct node* temp = listHead;
    while(listHead){
        
        delete_and_free_node(&listHead, temp);
        temp = listHead;
    }
}

void deepCopyTask(Task* new, struct node* old){

    *(new) = *(old->task);
    new->name = malloc((strlen(old->task->name)+ 1) * sizeof(char));
    strcpy(new->name, old->task->name);
}

void reverseList(struct node* oldH){

    struct node* newHead = NULL;
    struct node* ptr = oldH;
    
    while(ptr){
        Task *newTask = (Task*)malloc(sizeof(Task));
        deepCopyTask(newTask, ptr);
        insert(&newHead, newTask);
        ptr = ptr->next;
    }
    
    //save old list in old head, and make head point to newHead
    struct node* oldHead = oldH;
    head = newHead;

    delete_and_free_list(oldHead);
}


struct node* pickNextTask(){
    struct node *ptr = head;
    struct node *nextNode = ptr;

    printf("\n* finding task with highest priority... *\n");

    while(ptr){//find task with highest priority
        if(ptr->task->priority > nextNode->task->priority){
            nextNode = ptr;
        }
        ptr = ptr->next;
    }
    return nextNode;
}

// add a task to the list 
void add(char *name, int priority, int burst){

    Task *task = malloc(sizeof(Task)); //need to free
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    task->tid = pCount++;

    insert(&head, task);
}

// invoke the scheduler
void schedule(){

    reverseList(head);

    printf("***\nPRIORITY: Tasks will be picked based on the highest priority, and run for their full burst.");
    printf("\nIf tasks have the same priority, the order is based on which task requested the CPU first.\n***\n");

    for(int i=0; i<pCount;i++){
        struct node *n = pickNextTask();
        run(n->task, n->task->burst);
        delete(&head,n->task);
        free(n->task->name);
        free(n->task);
        free(n);
    }

    printf("\n* No more tasks! *\n");
}