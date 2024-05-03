Uthread: switching between threads
+ thread_create: 
    + set state RUNNABLE; 
    + set ra the called function address; 
    + set sp the address create in the data area in user space
+ thread_schedule
    + find a runnable thread
    + switch
        + set current thread running
        + save/restore user-space registers
            + the restored ra will change the execution flow in user-space threads
            + the restored sp represtants the user-space stack for user-space threads