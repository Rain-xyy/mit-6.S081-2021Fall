+ Backtrace
    + only one page for each process's stack
    + just fetch previous fp from current stack frame and jump back to the previous frame
    + recursive
+ Alarm
    + sigalarm: Save the ticks and handler(function pointer) in proc
    + When alarm triggered, save current trapframe(the context for current user process) and set the user process's pc with the value handler
    + sigreturn: When the handler finishes, we want to jump back to the previous instruction and execute it. In sigreturn, we resume the trapframe with the saved one in step2.
    + To avoid conflicts, we use a has_return flag indicating whether we are in handler.