//
// Created by juanfra on 18/11/16.
//
#include "include/stdlib.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdtime.h"
#include "include/math.h"

int donPepito();
int donJose();

int pepitoJose(){
    void** parg = (void**)malloc(sizeof(void*) * 2);
    parg[0] = (void*)"donPepito";
    exec(&donPepito, 1, parg, 1);
    parg[0] = (void*)"donJose";
    exec(&donJose, 1, parg, 1);

}

int donPepito(){
    char* msgs[4]={
            "Hola don Jose",
            "Por su casa yo pase",
            "A su abuela yo la vi",
            "Adios don Jose"
    };
    char buffer[100]={0};
    int jose=openPipe("PepitoToJose");
    int me=openPipe("JoseToPepito");
    read(me,buffer,99);
    printf("Jose: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(jose,msgs[0],strlen(msgs[0])+1);
    read(me,buffer,99);
    printf("Jose: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(jose,msgs[1],strlen(msgs[1])+1);
    read(me,buffer,99);
    printf("Jose: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(jose,msgs[2],strlen(msgs[2])+1);
    read(me,buffer,99);
    printf("Jose: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(jose,msgs[3],strlen(msgs[3])+1);
    closePipe("PepitoToJose");
    closePipe("JoseToPepito");


}

int donJose(){
    char* msgs[4]={
            "Hola don Pepito",
            "Paso usted por mi casa?",
            "Vio usted a mi abuela?",
            "Adios don Pepito"
    };
    char buffer[100]={0};
    int me=openPipe("PepitoToJose");
    int pepito=openPipe("JoseToPepito");

    sleep(randBound(1000,5000));
    write(pepito,msgs[0],strlen(msgs[0])+1);
    read(me,buffer,99);
    printf("Pepito: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(pepito,msgs[1],strlen(msgs[1])+1);
    read(me,buffer,99);
    printf("Pepito: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(pepito,msgs[2],strlen(msgs[2])+1);
    read(me,buffer,99);
    printf("Pepito: %s\n",buffer);
    sleep(randBound(1000,5000));
    write(pepito,msgs[3],strlen(msgs[3])+1);
    read(me,buffer,99);
    printf("Pepito: %s\n",buffer);
    closePipe("PepitoToJose");
    closePipe("JoseToPepito");

}