#include "schedulers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "cpu.h"


struct node *head = NULL; //need to use to free whole list
static int pCount = 0;
static int sameCount = 0;


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

//reduces the task's burst by the number specified by slice
int updateBurst(Task *t, int slice){

    if(t->burst < QUANTUM){
        t->burst = 0;

    }
    else{
        t->burst = t->burst - slice;
    }


    return t->burst;
}

struct node* getHighestPriority(){
    struct node *ptr = head;
    struct node *nextNode = ptr;
    
    while(ptr){//find task with highest priority
        if(ptr->task->priority > nextNode->task->priority){
            nextNode = ptr;
        }
        ptr = ptr->next;
    }
    return nextNode;
}

struct node* getLowestPriority(){
    struct node *ptr = head;
    struct node *nextNode = ptr;
    
    while(ptr){//find task with lowest priority
        if(ptr->task->priority < nextNode->task->priority){
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

void sortByPriority(){
   
    struct node* newHead = NULL;
    struct node* ptr = head;
    
    while(ptr){
        Task *newTask = (Task*)malloc(sizeof(Task));
        struct node *tempNode = getLowestPriority();

        deepCopyTask(newTask, tempNode);
        insert(&newHead, newTask);

        delete_and_free_node(&head, tempNode);
        
        ptr = head;

    }
    head = newHead;
}

// if there are multiple nodes with the same priority (up next), 
//   this function will 
//   - return a list containing the nodes with same priorities
//   - delete and free those same nodes from the original list
// else, returns null
struct node* check_for_same_priority(){
    if(!head || !head->next)
        return NULL;

    int priorityToCheck = head->task->priority;

    sameCount = 0;
    // if multiple tasks with same priority
    if((head->next) && (head->next->task->priority == priorityToCheck)){
        //handle first node
        struct node* newHead = NULL;
        Task *newTask = (Task*)malloc(sizeof(Task));
        struct node *tempNode = head;

        deepCopyTask(newTask, tempNode);
        insert(&newHead, newTask);
        sameCount++;

        delete_and_free_node(&head, tempNode);
       
        // handle 2nd node (and however many until a lower priority is found)
        while(head->task->priority == priorityToCheck){ //loop until next is not the same priority
            
            Task *newTask = (Task*)malloc(sizeof(Task));
            tempNode = head;

            deepCopyTask(newTask, tempNode);
            insert(&newHead, newTask);
            sameCount++;
            
            delete_and_free_node(&head, tempNode);
            
            //head will already be incremented due to it being deleted from it's own list
            //if head is null, break (at end of list)
            if(!head){
                break;
            }

        }
    
        return newHead;
    }

    else {return NULL;}
}

void run_round_robin(struct node* rrhead){
    
    struct node* curr = rrhead;

    while(sameCount > 0){
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
            curr = rrhead;
        }

        // if task is done, delete and free
        if(done_flag){
            delete_and_free_node(&rrhead, old);
            sameCount--;
        }
    }
}

// invoke the scheduler
void schedule(){
    
    printf("***\nPRIORITY WITH RR: The order of tasks picked is determined based on which task has the highest priority.\n");
    printf("If two or more tasks have the same priority, those tasks will run in RR until they are all completed.\n***\n");

    reverseList(head);
    sortByPriority();

    struct node* samePri = check_for_same_priority();
    struct node* temp = head;
    while(temp || samePri){
        if(samePri){
            printf("\n* The next series of tasks are run in RR because they have the same priority, *\n* and that priority is the highest of all tasks remaining. *\n");
            run_round_robin(samePri);    
        }
        else{
            printf("\n* This next task is being run for its full burst because it has the highest priority *\n* in the list of remaining tasks, and it's the only task with that priority *\n");
            run(head->task, head->task->burst);
            delete_and_free_node(&head, temp); //head should update?
        }
        temp = head;
        samePri = check_for_same_priority();
    }
    
    printf("\n* No more tasks! *\n");
    delete_and_free_list(head);
}