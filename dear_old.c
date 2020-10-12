#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#define  MAX_SLEEP_TIME     5
#define  MAX_DEPOSIT_AMOUNT   100
#define  MAX_STUDENT_NEED  50
void old_dad(int*, int*);
void deposit_amt(int*, int*);
void poor_student(int*, int*);

int main(int  argc, char *argv[])
{
  int acct;
  int *b_PTR;
  int *t_PTR;
  int turn;
  pid_t pid;
  int status;
  turn = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  acct = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  t_PTR = (int *) shmat(turn, NULL, 0);
  b_PTR = (int *) shmat(acct, NULL, 0);
  if ((*b_PTR == -1) || (*t_PTR == -1)){
    printf("*** shmat error (server) ***\n");
    exit(1);
  }
  if ((acct < 0) || (turn < 0)){
    printf("*** shmget error (server) ***\n");
    exit(1);
  }
  pid = fork();
  srand(getpid());
  if (pid == 0){
    poor_student(b_PTR, t_PTR);
    exit(0);
   }
  else if (pid < 0){
    printf("*** fork error (server) ***\n");
    exit(1);
  }
  else {
    old_dad(b_PTR, t_PTR);
    wait(&status);
    exit(0);
  }
  exit(0);
}
void deposit_amt(int * ptr_b, int * ptr_t)
{
  int balance = rand() % MAX_DEPOSIT_AMOUNT;
  if ((balance % 2) != 0)
  {
    printf("Dear old Dad: Doesn't have any money to give\n");
  } 
  else {
    *ptr_b += balance;
    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, *ptr_b);
  }
  *ptr_t = 1;
}
void poor_student(int * ptr_b, int * ptr_t)
{
  int j;
  for(j = 0; j < 15; j++){
    sleep(rand() % MAX_SLEEP_TIME);
    while(*ptr_t != 1){}
    int balance = rand() % MAX_STUDENT_NEED;
    printf("Poor Student needs $%d\n", balance);
    if (balance <= *ptr_b)
    {
      *ptr_b -= balance;
      printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, *ptr_b);
    } else {
      printf("Poor Student: Not Enough Cash ($%d)\n", *ptr_b);
    }
    *ptr_t = 0;
  }
}
void old_dad(int * ptr_b, int * ptr_t)
{
  int i;
    for (i = 0; i < 15; i++){
    sleep(rand() % MAX_SLEEP_TIME);
    while(*ptr_t != 0){}
    if(*ptr_b <= 100){
      deposit_amt(ptr_b, ptr_t);
    } else {
      printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", *ptr_b);
    }
    *ptr_t = 1;
  }
}