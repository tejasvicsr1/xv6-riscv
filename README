# Operating Systems and Networks

## Extending xv6

### Tejasvi Chebrolu

## Implementing Systrace Call

The system call traces all the system call of the OS, given as a masked input to the system call using command 'strace mask command ```[args]```'.
We implemented the systrace system call given as `extern uint64 sys_trace(void);` and the function implementing it given by `int trace(int);`.

```c
trace(atoi(argv[1]));
exec(argv[2], argv+2);
```

The argument given in ```argv[1]``` is mask value.
We output the process ID, name, arguments and the return value of the system call.

## Schedulers

The `SCHEDULER` flag has been passed into the Makefile with the default value of RR. The `SCHED` macro in the code has then been set using the `-D` flag. Thereafter, the same macro has been used throughout the code to find out which scheduler is in use. If no `SCHEDULER` flag has been passed, then the default Round Robin scheduler will be used as the default value.

## First Come First Serve Scheduler

To implement FCFS scheduling, we first need to find the process with the minimum runnable arrival time, ie, with the minimum value of `ctime` and state `RUNNABLE`. We do this by a simple loop through the process table:

```c
for( p = proc ; p < &proc[NPROC]; p++)
{
  acquire(&p->lock);
  if (p->ctime < minimum && p->state == RUNNABLE)
  {
    minimum = p->ctime;
    fcfsproc = p;
  }
  release(&p->lock);
}
```

We store the process that have minimimum 'ctime' and then we check if the process is in `RUNNABLE` state, if yes we then provide the CPU to the process and the process execute.

To make the scheduler non-preemptive, we need the process to not yield the system resources it is using after a single tick but only when its state changes. This can be done by a simple check in `trap.c` file. There, we were calling the `yield()` function if our process was in `RUNNING` state and we got caught in the time trap. However, we now put a simple check that we do this only when our scheduler is not FCFS:

## Priority Based Scheduler

For this type of scheduler, we first need to find the process with the highest priority, meaning we need to find the process that is state `RUNNABLE` with the lowest `dynamic probability`.

```c
for( p = proc ; p < &proc[NPROC]; p++ )
{
  acquire(&p->lock);
  dpval = getDP(p->priority, p->rtime, p->stime);
  if(dpval < minimum && p->state == RUNNABLE)
  {
    p->priority, dpval, p->rtime, p->stime);
    minimum = dpval;
    pbsproc = p;
    schedulesMax = p->schedules;
    startTime = p->ctime;
  }
  else if(dpval == minimum)
  {
    if(p->schedules < schedulesMax)
    {
      pbsproc = p;
      schedulesMax = p->schedules;
      startTime = p->ctime;
    }
    else if( p->schedules == schedulesMax)
    {
      if(p->ctime > startTime)
      {
        pbsproc = p;
        startTime = p->ctime;
      }
    }
  }
  release(&p->lock);
}
```

We can change the static priority of the system calls as:

**int set_priority(int, int) - System call**

This system call is defined as `int set_priority(int new_priority, int pid)`. The call first acquires the process table mutex lock. It then finds the process whose pid is the same as `pid`. If no such process is found, -1 is returned. If found, the priority of the process is changed to `new_priority` and if this new priority is lower than the previous priority of the process, the current process yields and the scheduler is called again.

The `dynamic_priority` takes care of changes in the priority of the processes using the niceNess ratio as given the assignment document.

## Multi-level Feedback Queue Scheduler

First we declated a new data strcuture queue which consists of array of the processes and the length of the queue.

```c
struct Queue {
  struct proc *processes[NPROC];
  uint64 numProcesses;
};
```

We also implement functions that add and remove from the queue.

```c
void
addToQueue(int queueNumber, struct proc *process){
  queue_mlfq[queueNumber].processes[queue_mlfq[queueNumber].numProcesses] = process;
  queue_mlfq[queueNumber].numProcesses += 1;
}

void
deleteFromQueue(int queueNumber, int index){
  queue_mlfq[queueNumber].numProcesses--;
  for(int i = index; i < queue_mlfq[queueNumber].numProcesses ; i++){
    queue_mlfq[queueNumber].processes[index] = queue_mlfq[queueNumber].processes[index+1];
  }
}
```

Now for scheduling we check whether the process in the queues are aged or not, if yes, them we shift the process to the next queue with less priority.

Here we keep the aged time as 30 queue ticks in a particular queue before we switch to the next one.

```c
for(int i = 1; i < 5; i++)
{
  int length = queue_mlfq[i].numProcesses;
  int breakLength = 0;
  for(int j = 0; j<length ; j++)
  {
    carryProc = queue_mlfq[i].processes[j];
    int checkTime = ticks - carryProc->queueEntry;
    if(checkTime < 30)
    {
    // if 30 clock ticks are spent in a low priority queue, then you move to previous queue
      breakLength = j;
      break;
    }
  }
  for(int j = 0 ; j<breakLength ; j++)
  {
    carryProc = queue_mlfq[i].processes[j];
    acquire(&carryProc->lock);
    carryProc->queueEntry = ticks;
    carryProc->queueNumber--;
    carryProc->wtime = 0;
    addToQueue(carryProc->queueNumber, carryProc);
    release(&carryProc->lock);
  }
  for(int j = 0 ; j<breakLength ; j++)
    deleteFromQueue(i, j);
}
```
After that we take the ```0th``` process from the highest priority queue that is non empty.

### Possible exploitation

Since a process leaves the queuing system when it voluntarily leaves relinquishes control of the CPU and is inserted back when it is ready again, a process could, theoretically, enter into queue 1, run for 1 clock tick, relinquish, then immediately become available again and run for just 1 clock tick again. With this, the process will never get demoted from queue 1. Although the time slice it will get will be lower, but if the number of process in queue 2, 3 and 4 is large, this will allow the process to prioritise its execution by a large factor and exploit the system. This is like the major disadvantage of a simple MLFQ algorithm as taught in class.

### Comparisons
As expected and as taught in class also FCFS scheduler performs worst of the three schedulers for which we were able to gather results.
The round robin and PBS performed, similarly on metrics.

| Scheduler                            | rtime | wtime |
| ------------------------------------ | ----- | ----- |
| Round Robin Scheduler                | 4     | 2500  |
| First Come First Serve Scheduler     | 4     | 3200  |
| Priority Based Scheduler             | 6     | 2459  |

## Executing the xv6
```
make qemu SCHEDULER=[]
```