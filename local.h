#ifndef _LOCAL_H_
#define _LOCAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h> 
#include <unistd.h>
#include <string.h>

#define INPUT_FILE "./input.txt"

struct emp_info
{
	int line_id;
	int emp_id;
    char factory_name[4];
};

struct truck_info
{
	int num_of_available;
	int truck_id;
    char factory_name[4];
};

void red (){
    printf("\033[1;31m");
    fflush(stdout);
}

void red2 (){
    printf("\033[7;31m");
    fflush(stdout);
}

void red3 (){
    printf("\033[6;31m");
    fflush(stdout);

}

void green (){
    printf("\033[1;32m");
    fflush(stdout);
}

void yellow(){
    printf("\033[1;33m");
    fflush(stdout);
}

void yellow2(){
    printf("\033[4;33m");
    fflush(stdout);
}

void yellow3(){

   printf("\033[6;33m");
    fflush(stdout);
}

void blue(){
    printf("\033[1;34m");
    fflush(stdout);
}

void blue2(){

    printf("\033[7;34m");
    fflush(stdout);

}

void normals(){

    printf("\033[0;37m");
    fflush(stdout);
}

void cyan(){

    printf("\033[1;36m");
    fflush(stdout);

}

void cyan2(){
    printf("\033[0;36m");
    fflush(stdout);
}

void cyan3(){
    printf("\033[6;36m");
    fflush(stdout);
}

void Purple(){
    printf(" \033[1;35m");
    fflush(stdout);

}

void Purple2(){
    printf(" \033[6;35m");
    fflush(stdout);
}

#endif



