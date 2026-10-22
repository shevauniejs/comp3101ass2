#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

int fib(int i);
void setIntervals(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void vSec();
void rSec();
void pSec();

//void setIntervals(struct times R, struct times V, struct times  );
//kernel = profile-virtual
long vcSec, rcSec, pcSec;

int main(int argc, char* argv[]){
    int parentValue = atoi(argv[1]);
    int child_1Value = atoi(argv[2]);
    int child_2Value = atoi(argv[3]);
    int fibVal, w1, w2;
    int fchild1, fchild2;

    struct itimerval virt;
    struct itimerval real;
    struct itimerval prof;

    long vcuSec, rcuSec, pcuSec;

    w1,w2 = 0,0;

    setIntervals(&virt,&real,&prof);

    setitimer(ITIMER_VIRTUAL,&virt,NULL);
    setitimer(ITIMER_REAL,&real,NULL);
    setitimer(ITIMER_PROF,&prof,NULL);

    signal(SIGVTALRM,vSec);
    signal(SIGALRM,rSec);
    signal(SIGPROF,pSec);

    fchild2 = fork();

    fibVal = fib(parentValue);
    printf("[Parent] fibonacci(%d) = %d:  \n", parentValue, fibVal);
    getitimer(ITIMER_VIRTUAL,&virt);
    getitimer(ITIMER_REAL,&real);
    getitimer(ITIMER_PROF,&prof);

    rcuSec = 1000000-real.it_value.tv_usec;
    vcuSec = 1000000-virt.it_value.tv_usec;
    pcuSec = 1000000-prof.it_value.tv_usec;

    //getitimer(ITIMER_VIRTUAL, &virt);
    printf("User Time (Virtual Time): %ld seconds %ld microseconds\n", vcSec, vcuSec);
    printf("Wall Time (Real Time) Spent: %ld seconds %ld microseconds\n", rcSec, rcuSec);
    printf("Profile Time: %ld seconds %ld microseconds\n", pcSec, pcuSec);
    printf("Kernel Time: %ld seconds %ld microseconds\n\n", pcSec-vcSec, pcuSec-vcuSec);
    return 1;
}

void setIntervals(struct itimerval* v, struct itimerval* r, struct itimerval* p){
    v->it_interval.tv_sec = 1;
    v->it_interval.tv_usec = 0;
    v->it_value.tv_sec = 1;
    v->it_value.tv_usec = 0;

    r->it_interval.tv_sec = 1;
    r->it_interval.tv_usec = 0;
    r->it_value.tv_sec = 1;
    r->it_value.tv_usec = 0;

    p->it_interval.tv_sec = 1;
    p->it_interval.tv_usec = 0;
    p->it_value.tv_sec = 1;
    p->it_value.tv_usec = 0;
}

void vSec(){
    vcSec++;
    }

void rSec(){
    rcSec++;
    }

void pSec(){
    pcSec++;
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
