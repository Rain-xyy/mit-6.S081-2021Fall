+ If there is a page fault and the va is invalid, just mark the process as killed.

Procedure
+ mmu interrupts the kernel with r_scause() set
+ kernel deals with the page fault with different strategies according to the r_scause() & the pte
    + maybe: alloc a new physical page & map it to the virtual address triggering the page fault