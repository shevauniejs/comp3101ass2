#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "signal.h"

int fib(int i);
void shCountSec();
struct times{
    struct itimerval prof;
    struct itimerval real;
    struct itimerval kern;
    struct itimerval virt;
    int profSec;
    int kernSec;
    int virtSec;
    };

int realSec, realuSec=0;

int main(int argc, char* argv[]){
    int val= atoi(argv[1]);
    struct times oneTime;
    oneTime.real.it_interval.tv_sec = 1;
    oneTime.real.it_interval.tv_usec = 0;
    oneTime.real.it_value.tv_sec = 1;
    oneTime.real.it_value.tv_usec = 0;

    signal(SIGALRM,shCountSec);

    setitimer(ITIMER_REAL,&oneTime.real, NULL);
    int fibVal = fib(val);
    getitimer(ITIMER_REAL, &oneTime.real);

    printf("Fibo value: %d\nTime: %d seconds %d microseconds", fibVal, realSec, realuSec);
}

//as 1 second pass, raise signal for real time alarm and increment seconds,
int fib(int i){
   if(i==0){
       return 0;
   }
   else if(i==1){
       return 1;
   }
   else{
        return fib(i-1) + fib(i-2);
   }
}

void shCountSec(){
    realSec++;
    realuSec++;
}
