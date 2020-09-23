#include <linux/smp_lock.h>



struct cs1550_sem
{
   int value;
   long sem_id;
   spinlock_t lock;
   char key[32];
   char name[32];
   struct cs1550_node* head;
   struct cs1550_node* tail;
   //Some FIFO queue of your devising
};

struct cs1550_node{
   struct cs1550_node* next;
   struct task_struct* process;
}

//Add to this file any other struct definitions that you may need
