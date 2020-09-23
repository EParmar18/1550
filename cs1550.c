#include <linux/cs1550.h>

/* This syscall creates a new semaphore and stores the provided key to protect access to the semaphore. The integer value is used to initialize the semaphore's value. The function returns the identifier of the created semaphore, which can be used to down and up the semaphore. */
asmlinkage long sys_cs1550_create(int value, char name[32], char key[32]){
  return 0;
}
/* This syscall opens an already created semaphore by providing the semaphore name and the correct key. The function returns the identifier of the opened semaphore if the key matches the stored key or -1 otherwise. */
asmlinkage long sys_cs1550_open(char name[32], char key[32]){
  return 0;
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
  lock(global_spinlock);
  return 0;
}
/* This syscall implements the up operation on an already opened semaphore using the semaphore identifier obtained from a previous call to sys_cs1550_create or sys_cs1550_open. The function returns 0 when successful or -1 otherwise (e.g., if the semaphore id is invalid). Please check the lecture slides for pseudo-code of the up operation. */
asmlinkage long sys_cs1550_up(long sem_id){
  // Find the process we want to wake up in semaphore list
  // wake_up_process(sleeping_task);

 // Psuedo Code 


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
  return 0;
}
