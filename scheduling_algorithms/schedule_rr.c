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

//reduces the task's burst by the number specified by slice
int updateBurst(Task *t, int slice){

    if(t->burst < QUANTUM){
        printf("* updating burst of %s. was:%d, is now:%d *\n\n", t->name, t->burst, 0);
        t->burst = 0;

    }
    else{
        printf("* updating burst of %s. was:%d, is now:%d *\n\n", t->name, t->burst, (t->burst - slice));
        t->burst = t->burst - slice;
    }


    return t->burst;
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

// invoke the scheduler
void schedule(){

    printf("***\nRR: Tasks will run in order that they request the CPU, but only for a specified amount of time (Quantum).\n***\n\n");

    reverseList(head);

    struct node* curr = head;
    while(pCount != 0){

        int done_flag = 0;

        // if burst<=QNTM run for burst and set done flag
        if(curr->task->burst <= QUANTUM){
            run(curr->task, curr->task->burst);
            updateBurst(curr->task, QUANTUM);
            done_flag = 1;
        }
        else{ // burst>QNTM, run for QNTM
            run(curr->task, QUANTUM);
            updateBurst(curr->task, QUANTUM);
        }

        //in case of deletion, save before iterating
        struct node* old = curr;

        // if not at end of list, increment pointer
        if(curr->next){
            curr = curr->next;
        }
        else{// end of list, loop back
            curr = head;
        }

        // if task is done, delete and free
        if(done_flag){

            delete_and_free_node(&head, old);
            pCount--;
        }
    }
    printf("* No more tasks! *\n");
}