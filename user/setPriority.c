#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]){
  if (argc != 3)
  {
    printf("Exactly 3 arguments required!\n");
    exit(1);
  }
  int newPriority = 0;
  newPriority = atoi(argv[1]);
  int pid = 0;
  pid = atoi(argv[2]);
  if(newPriority > 100)
  {
    printf("Invalid priority, should be in range [0, 100]\n");
    exit(1);
  }
  int ret = set_priority(newPriority, pid);
  if(ret < 0)
  {
    printf("No process corrensponding to the pid!\n");
    exit(1);
  }
  printf("pid: %d\nold_priority: %d\nnew_priority: %d\n", pid, ret, newPriority);
  exit(0);
}