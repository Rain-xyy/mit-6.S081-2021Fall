+ Spinlock is based on cas atomic operation, which is provided by hardware.
+ Uthread: switching between threads
    + Why do we need to simulate a stack in struct thread?
        + The simulated stack (in .data area) is used to perform operations on. So each uthread can work on it's own stack.