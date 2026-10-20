#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "signal.h"

int fib(int i);
int countSec();
struct times{
    struct itimerval prof;
    struct itimerval real;
    struct itimerval kern;
    struct itimerval virt;
    int profSec;
    int realSec;
    int kernSec;
    int virtSec;
    };

int main(int argc, char* argv[]){
    int val= atoi(argv[1]);
    struct times oneTime;
    oneTime.real.it_interval.tv_sec = 1;
    oneTime.real.it_interval.tv_usec =0;
    oneTime.real.it_val.tv_sec = 1;
    oneTime.real.it_value.tv_usec = 0;

    int fibVal = fib(val);
    printf("Fibo value: %d\n", fibVal);
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

int countSec(){
    int s;
    return s++;
}
