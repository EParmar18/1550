#include <linux/smp_lock.h>

struct cs1550_sem
{
   int value;
   long sem_id;
   spinlock_t lock;
   char key[32];
   char name[32];
   //Some FIFO queue of your devising
};

//Add to this file any other struct definitions that you may need
