/**
Author: Shevaunie J. Senior
Institution: University of the West Indies, Mona
ID Number: 620109080
Course: Operating Systems (COMP3101)
Assigmnent Number: 02
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int fibo(int i);
void setAllIntervals(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void setAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void getAlliTimers(struct itimerval* v, struct itimerval* r, struct itimerval* p);
void calcMicroSecs(long* vs, long* rs, long* ps, struct itimerval* v, struct itimerval* r, struct itimerval* p);
void printer(long vSec, long* vu, long rSec, long* ru, long pSec, long* pu);
void superInit(int* fibonacci, struct itimerval* v, struct itimerval* r, struct itimerval* p, long* vu, long* ru, long* pu);
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
	
	//child and wait variables
    int fchild1, fchild2;
	int w1 = 0; int w2 = 0;

    long vcuSec, rcuSec, pcuSec; //microseconds for virtual, real and profile times
   
	fchild1 = fork();
	if(fchild1==0){
		struct itimerval virt;
    	struct itimerval real;
    	struct itimerval prof;

    	signal(SIGVTALRM,vSec);
    	signal(SIGALRM,rSec);
    	signal(SIGPROF,pSec);
		printf("[Child 1]\n"); 
		superInit(&child1Value, &virt, &real, &prof, &vcuSec, &rcuSec, &pcuSec);
	}
	if(fchild1>0){
		w1=waitpid(fchild1,NULL,0);
		fchild2=fork();
		if(fchild2==0){
			struct itimerval virt;
    		struct itimerval real;
    		struct itimerval prof;

    		signal(SIGVTALRM,vSec);
    		signal(SIGALRM,rSec);
    		signal(SIGPROF,pSec);
			printf("[Child 2]\n"); 
			superInit(&child2Value, &virt, &real, &prof, &vcuSec, &rcuSec, &pcuSec);
		}
		else{
			w2=waitpid(fchild2,NULL,0);
			struct itimerval virt;
    		struct itimerval real;
    		struct itimerval prof;

    		signal(SIGVTALRM,vSec);
    		signal(SIGALRM,rSec);
    		signal(SIGPROF,pSec);
			printf("[Parent]\n"); 
			superInit(&parentValue, &virt, &real, &prof, &vcuSec, &rcuSec, &pcuSec);
		}
	}
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
Prints all timing information to screen
*/
void printer(long vSec, long* vu, long rSec, long* ru, long pSec, long* pu){
    printf("User Time (Virtual Time): %ld seconds %ld microseconds\n", vSec, *vu);
    printf("Wall Time (Real Time) Spent: %ld seconds %ld microseconds\n", rSec, *ru);
    printf("Profile Time: %ld seconds %ld microseconds\n", pSec, *pu);
    printf("Kernel Time: %ld seconds %ld microseconds\n\n", pSec-vSec, *pu-*vu);
}

/**
Everything put together tbh...
*/
void superInit(int* fibonacci, struct itimerval* v, struct itimerval* r, struct itimerval* p, long* vu, long* ru, long* pu){
	setAllIntervals(v,r,p);
	setAlliTimers(v,r,p);
	int fibVal = fibo(*fibonacci);
    printf("Fibonacci(%d) = %d:  \n", *fibonacci, fibVal);
	
	getAlliTimers(v,r, p);
	calcMicroSecs(vu, ru, pu, v, r, p);
	printer(vcSec, vu, rcSec, ru, pcSec, pu);
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
