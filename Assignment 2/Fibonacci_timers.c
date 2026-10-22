#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "signal.h"

int fib(int i);
void vSec();

//void setIntervals(struct times R, struct times V, struct times  );
long vcSec, rcSec;

int main(int argc, char* argv[]){
    int val= atoi(argv[1]);
    struct itimerval virt;
    struct itimerval real;

    long vcuSec, rcuSec;
    virt.it_interval.tv_sec = 1;
    virt.it_interval.tv_usec = 0;
    virt.it_value.tv_sec = 1;
    virt.it_value.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL,&virt,NULL);

    signal(SIGVTALRM,vSec);
    int fibVal = fib(val);
    getitimer(ITIMER_VIRTUAL,&virt);
    vcuSec =1000000-virt.it_value.tv_usec;
    //getitimer(ITIMER_VIRTUAL, &virt);
    printf("Fibo value: %d Virtual Time: %ld seconds %ld microseconds\n", fibVal, vcSec, vcuSec);

}

void vSec(){
    vcSec++;
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
