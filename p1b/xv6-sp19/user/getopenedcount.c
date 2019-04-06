#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  // For testing
  // for(int i=0; i<atoi(argv[1]); i++)
  // 	open("Jack",0);

  printf(1,"%d\n", getopenedcount());
  exit();
}
