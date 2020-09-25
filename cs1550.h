#ifndef _LINUX_CS1550_H
#define _LINUX_CS1550_H 1
#include <linux/smp_lock.h>


// Semaphore Queue Element for the global semaphore queue list
struct cs1550_sem
{
   int value;
   long sem_id;
   spinlock_t lock;
   char key[32];
   char name[32];
   //Some FIFO queue of your devising0
   struct cs1550_queue *semaphores;
}

// Global semaphore list
struct list_node{
   struct cs1550_sem sem;
   struct list_node *next;
}

struct sem_list{
   struct list_node *head;
   struct list_node *tail;
}

// Node for the semaphore queues
struct queue_node{
   struct cs1550_node* next;
   struct task_struct* process;
}

// Semaphore queue
struct cs1550_queue{
   int count;
   struct queue_node *front;
   struct queue_node *rear;
}

//Add to this file any other struct definitions that you may need

#endif