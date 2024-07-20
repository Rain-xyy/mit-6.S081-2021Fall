# Lab: Syscall

## 1. System call tracing

添加系统调用`trace`，它会监视进程使用特定的系统调用。当指定的系统调用被调用时，会输出对应的进程号、系统调用名、返回值。

```shell
# 命令格式
$ trace [MASK] [OPTIONS...] # MASK是一个数字n; 如果(n >> i) & 1 == 1 表示i号系统调用需要trace
# 输出的形式
pid: syscall <syscall_name> -> <return_value>
```

添加系统调用

```c
uint64
sys_trace(void)
{  
  int n;

  if(argint(0, &n) < 0)
    return -1;
  struct proc *p = myproc();
  p->trace = n;
  return 0;
}
```

修改系统调用入口，监听mask指定的系统调用

```c
void
syscall(void)
{
  int num;
  struct proc *p = myproc();

  num = p->trapframe->a7;	// get syscall numm from register a7
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    p->trapframe->a0 = syscalls[num]();
    if (p->trace & 1 << num) {
      printf("%d: syscall %s -> %d\n", p->pid, syscall_names[num], p->trapframe->a0);
    }
  } else {
    printf("%d %s: unknown sys call %d\n",
            p->pid, p->name, num);
    p->trapframe->a0 = -1;
  }
}
```

## 2. Sysinfo

添加一个新的系统调用`Sysinfo`，获取内存当前剩余的空闲页数量和没有使用的进程号数量。

添加系统调用

```c
uint64
sys_sysinfo(void)
{
  uint64 addr;
  if(argaddr(0, &addr) < 0) {
    return -1;
  }
  struct sysinfo st;
  struct proc *p = myproc();
  st.freemem = freemem();
  st.nproc = nproc();
  if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
    return -1;
  return 0;
}
```

统计空间内存页数量

```c
uint64 
freemem(void) {
  struct run *r;
  uint64 free_size = 0;

  acquire(&kmem.lock);
  r = kmem.freelist;
  while (r)
  {
    free_size += PGSIZE;
    r = r->next;
  }
  release(&kmem.lock);
  return free_size;
}
```

统计未使用进程号

```c
uint64
nproc(void) {
  struct proc *p;
  uint count = 0;
  for(p = proc; p < &proc[NPROC]; p++) {
    if (p->state != UNUSED) {
      count++;
    }
  }
  return count;
}
```

