#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char ** argv)
{
  trace(atoi(argv[1]));
  exec(argv[2], argv+2);
  // int pid = fork();
  // if(pid<0)
  // {
  //   printf("fork(): failed\n");
  //   return -1;
  // }
  // else if(pid == 0){
  //   exec(argv[2], argv+2);
  // }
  // else{
  //   trace(atoi(argv[1]));
  // }
  exit(0); 
}