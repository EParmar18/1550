#include <linux/cs1550.h>

DEFINE_SPINLOCK(sem_lock);

struct sem_list *sem_list_new(void)
{
  struct sem_list *s;
  s = (struct sem_list *)kmalloc(sizeof(struct sem_list), GFP_ATOMIC);

  if(s == NULL)
  {
    return NULL;
  }

  s->head = NULL;
  s->tail = NULL;
}


struct queue *queue_new(void)
{
  struct cs1550_queue *q;
  q = (struct cs1550_queue *)kmalloc(sizeof(struct cs1550_queue), GFP_ATOMIC);

  if(q == NULL)
  {
    return NULL;
  }
  q->head = NULL;
  q->tail = NULL;
  return q;
}

struct sem_list *semList;
int listSize = 0;
long semIdentifier = 1000;

struct cs1550_sem *list_find_name_key(char name[32], char key[32])
{ 
    struct list_node *curr = semList->head;
    struct cs1550_sem *semaphore;
    bool isKey = true;
    int i;
    while(curr != NULL)
    {
      for(i = 0; i < 32; i++)
      {
        if(name[i] != (*curr).sem.name[i] || key[i] != (*curr).sem.key[i])
        {
          isKey = false;
          break;
        }
        isKey = true;
      }
      
      if(isKey)
      {
        semaphore = &(curr->sem);
        return semaphore;
      }
      curr = curr->next;
    }
    return NULL;
}

struct cs1550_sem *list_find_id(long sem_id)
{ 
    struct list_node *curr = semList->head;
    struct cs1550_sem *semaphore;
  
    while(curr != NULL)
    {
      if((*curr).sem.sem_id = sem_id)
      {
        return &(curr->sem);
      }
      curr = curr->next;
    }
    return NULL;
}

/* This syscall creates a new semaphore and stores the provided key to protect access to the semaphore. The integer value is used to initialize the semaphore's value. The function returns the identifier of the created semaphore, which can be used to down and up the semaphore. */
asmlinkage long sys_cs1550_create(int value, char name[32], char key[32])
{
  if(listSize == 0)
  {
    sem_list_new();
  }

  int i;
  struct cs1550_sem *newSem;
  (*newSem).value = value;

  for(i = 0; i < 32; i++)
  {
    (*newSem).key[i] = key[i];
    (*newSem).name[i] = name[i];
  }

  newSem->sem_id = semIdentifier;
 
  if(semList->head == NULL)
  {
    semList->head = newSem;
  }
  else
  {
    semList->tail->next = newSem;
    semList->tail = newSem;
  }
  

  semIdentifier++;
  return 0;
}
/* This syscall opens an already created semaphore by providing the semaphore name and the correct key. The function returns the identifier of the opened semaphore if the key matches the stored key or -1 otherwise. */
asmlinkage long sys_cs1550_open(char name[32], char key[32]){
  struct cs1550_sem *semaphore;
  spin_lock(&sem_lock);
  semaphore = list_find_name_key(name, key);
  spin_unlock(&sem_lock);

  if(semaphore != NULL)
  {
    return semaphore->sem_id;
  }
  return -1;
}
/* This syscall implements the down operation on an already opened semaphore using the semaphore identifier obtained from a previous call to sys_cs1550_create or sys_cs1550_open. The function returns 0 when successful or -1 otherwise (e.g., if the semaphore id is invalid or if the queue is full). Please check the lecture slides for the pseudo-code of the down operation. */
asmlinkage long sys_cs1550_down(long sem_id){
  // Here the process can sleep
  // Mark the tasks as not ready
  // Set the current state as "TASK_INTERRUPTIBLE"
  // Invoke schedule() to get next task
  // Create a spinlock with DEFINE_SPINLOCK(sem_lock);

  // Surround critical reagions with the following 
  /*
  lock(global_spinlock)
  let sem = get_semaphore_by_id(sem_id)
  sem->refcount++
  unlock(global_spinlock)

  spin_lock(sem->lock)
    //Implementation of down() follows
  spin_unlock(sem->lock)

  lock(global_spinlock)
  sem->refcount--
  unlock(global_spinlock)
  */
  struct cs1550_sem *semaphore;
  spin_lock(&sem_lock);
  semaphore = list_find_id(sem_id);
  semaphore->value -= 1;
  spin_unlock(&sem_lock);

  if(semaphore == NULL)
  {
    return -1;
  }
  if(semaphore->value < 0)
  {
    struct queue_node* initialNode = (struct queue_node*)kmalloc(sizeof(struct queue_node), GFP_ATOMIC);
    initalNode->task_struct = current;
    initialNode->next = NULL;

    if(semList->head == NULL)
    {
      semaphore->head = initialNode;
    }
    else
    {
      semaphore->tail->next = initialNode;
    }

    semaphore->tail = initialNode;
    set_current_state(TASK_INTERRUPTIBLE);
    spin_unlock(&sem_lock);
    schedule();
  }
  else
  {
    spin_unlock(&sem_lock);
  }
  
  return 0;
}
/* This syscall implements the up operation on an already opened semaphore using the semaphore identifier obtained from a previous call to sys_cs1550_create or sys_cs1550_open. The function returns 0 when successful or -1 otherwise (e.g., if the semaphore id is invalid). Please check the lecture slides for pseudo-code of the up operation. */
asmlinkage long sys_cs1550_up(long sem_id){
  // Find the process we want to wake up in semaphore list
  // wake_up_process(sleeping_task);

 // Psuedo Code 
  spin_lock(&sem_lock);
  semList->value += 1;
  spin_unlock(&sem_lock);
  if(semList->value <= 0)
  {
    struct task_struct* task;
    struct cs1550_node* initialNode = semList->head;
    if(initialNode != NULL)
    {
      process = initialNode->process;

      if(initialNode == semList->tail)
      {
        semList->head = NULL;
        semList->tail = NULL;
      }
      else
      {
        semList->head = initialNode->next;
      }
      wake_up_process(process);
      
    }
    kfree(initialNode);
  }
  spin_unlock(&sem_lock);

  return 0;

  return 0;
}
/* This syscall removes an already created semaphore from the system-wide semaphore list using the semaphore identifier obtained from a previous call to sys_cs1550_create or sys_cs1550_open. The function returns 0 when successful or -1 otherwise (e.g., if the semaphore id is invalid or the semaphore's process queue is not empty). */
asmlinkage long sys_cs1550_close(long sem_id){
  /*
  lock(global_spinlock)
  let sem = get_semaphore_by_id(sem_id)
  if(sem->refcount == 0)
    free_semaphore(sem)
    unlock(global_lock)
    return 0
  else
    unlock(global_lock)
    return -1
  */

  struct cs1550_sem *semaphore;
  spin_lock(&sem_lock);

  semaphore = list_find_name_key(name, key);
  if(semaphore->value == 0)
  {
    kfree(semaphore);
    spin_unlock(&sem_lock);
    return 0;
  }
  else
  {
    spin_unlock(&sem_lock);
    return -1;
  }
}
