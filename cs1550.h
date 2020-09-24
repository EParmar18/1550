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
   struct cs1550_qeue *semaphores;
};

// Global semaphore list
struct list_node{
   struct cs1550_sem *head;
   struct cs1550_sem *tail;
}

// Node for the semaphore queues
struct qeue_node{
   struct cs1550_node* next;
   struct task_struct* process;
}

// Semaphore qeue
struct cs1550_qeue{
   int count;
   qeue_node *front;
   qeue_node *rear;
}

//Add to this file any other struct definitions that you may need
