#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

int fibo(int i);
void setAllIntervals(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void setAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void getAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void calcMicroSecs(long* vs, long* rs, long* ps, struct itimerval* v, struct itimerval* r, struct itimerval* p);
void vSec(int sig);
void rSec(int sig);
void pSec(int sig);

//void setIntervals(struct times R, struct times V, struct times  );
//kernel = profile-virtual
long vcSec, rcSec, pcSec;

int main(int argc, char* argv[]){
    //Values to supply to fibo function from shell
    int parentValue = atoi(argv[1]);
    int child1Value = atoi(argv[2]);
    int child2Value = atoi(argv[3]);

    int fchild1, fchild2;
    int w1 = 0; int w2 = 0;
	
    int fibVal;
    long vcuSec, rcuSec, pcuSec; 
   
    struct itimerval virt;
    struct itimerval real;
    struct itimerval prof;
    

    setAllIntervals(&virt,&real,&prof);
	


    signal(SIGVTALRM,vSec);
    signal(SIGALRM,rSec);
    signal(SIGPROF,pSec);
	
	setAlliTimers(&virt,&real,&prof);
	
	fibVal = fibo(parentValue);
    printf("[Parent] fibonacci(%d) = %d:  \n", parentValue, fibVal);
	
	getAlliTimers(&virt,&real, &prof);
	calcMicroSecs(&vcuSec, &rcuSec, &pcuSec, &virt, &real, &prof);



    //getitimer(ITIMER_VIRTUAL, &virt);
    printf("User Time (Virtual Time): %ld seconds %ld microseconds\n", vcSec, vcuSec);
    printf("Wall Time (Real Time) Spent: %ld seconds %ld microseconds\n", rcSec, rcuSec);
    printf("Profile Time: %ld seconds %ld microseconds\n", pcSec, pcuSec);
    printf("Kernel Time: %ld seconds %ld microseconds\n\n", pcSec-vcSec, pcuSec-vcuSec);
    return 1;
}
/**
Set intervals and values for all itimervals
*/
void setAllIntervals(struct itimerval* v, struct itimerval* r, struct itimerval* p){
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

/**
Set timer for virtual, real and profile times
*/
void setAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p){
    setitimer(ITIMER_VIRTUAL,v,NULL);
    setitimer(ITIMER_REAL,r,NULL);
    setitimer(ITIMER_PROF,p,NULL);
}

/**
Get values of virtual, real and profile times microseconds so we can calculate
*/
void getAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p){
    getitimer(ITIMER_VIRTUAL,v);
    getitimer(ITIMER_REAL,r);
    getitimer(ITIMER_PROF,p);
}

/**
Calculate number of microseconds for virtual, real and profile times
*/
void calcMicroSecs(long* vs, long* rs, long* ps, struct itimerval* v, struct itimerval* r, struct itimerval* p){
    *rs= 1000000-r->it_value.tv_usec;
    *vs = 1000000-v->it_value.tv_usec;
    *ps = 1000000-p->it_value.tv_usec;
}
			  
/**
Count seconds for virtual, real, and profile times
*/
void vSec(int sig){
    vcSec++;
    }

void rSec(int sig){
    rcSec++;
    }

void pSec(int sig){
    pcSec++;
}

/**
Fibonacci function
*/
int fibo(int i){
   if(i==0){
       return 0;
   }
   else if(i==1){
       return 1;
   }
   else{
        return fibo(i-1) + fibo(i-2);
   }
}
