# Lab: page tables

## 1. Speed up system calls

加快`getpid()`系统调用，为用户空间映射一个只读的页，该页保存了结构体`syscall`，里面保存了进程的pid。

修改`allocproc`函数，分配一个物理页保存pid。

```c
if((p->usyscall = (struct usyscall *)kalloc()) == 0){
  freeproc(p);
  release(&p->lock);
  return 0;
}
p->usyscall->pid = p->pid;
```

修改`freeproc`函数，在进程释放时释放该物理页。

修改`proc_pagetable`函数，为该物理页创建映射关系。

```c
if(mappages(pagetable, USYSCALL, PGSIZE,
            (uint64)(p->usyscall), PTE_R | PTE_U) < 0){
  uvmunmap(pagetable, TRAMPOLINE, 1, 0);
  uvmunmap(pagetable, TRAPFRAME, 1, 0);
  uvmfree(pagetable, 0);
  return 0;
}
```

## 2. Print a page table

打印进程的页表。

```c
void
do_vmprint(pagetable_t pagetable, int depth) {
  if (depth > 3) {
    return;
  }
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];
    if(pte & PTE_V){
      uint64 child = PTE2PA(pte);
      switch (depth) {
      case 1:
        printf(pattern_1, i, pte, child);
        break;
      case 2:
        printf(pattern_2, i, pte, child);
        break;
      case 3:
        printf(pattern_3, i, pte, child);
        break;
      }
      do_vmprint((pagetable_t)child, depth + 1);
    }
  }
}

void
vmprint(pagetable_t pagetable) {
  printf("page table %p\n", pagetable);
  do_vmprint((pagetable_t)pagetable, 1);
}
```

### 3. Detecting which pages have been accessed

检测某一页是否被访问，实现`pgacess`系统调用，直接访问该页对应页表项的`PTE_A`即可。

```c
int
sys_pgaccess(void)
{
  uint64 base;
  int len;
  uint64 uAddr;

  if(argaddr(0, &base) < 0)
    return -1;

  if(argint(1, &len) < 0)
    return -1;

  if (len > 32) {
    return -1;
  }

  if(argaddr(2, &uAddr) < 0)
    return -1;

  int result = 0;
  for (int i = 0; i < len; i++) {
      pte_t* pte = walk(myproc()->pagetable, base, 0);
      if ((*pte & PTE_A) && (*pte & PTE_V)) {
        result |= 1 << i;
        *pte ^= PTE_A;
      }
      base += PGSIZE;
  }
  copyout(myproc()->pagetable, uAddr, (char *)&result, sizeof(int));
  return 0;
}
```

