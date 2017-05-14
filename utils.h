#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include <time.h>
#include<unistd.h>
#include <inttypes.h>

struct Request{
  double duration;
  char * gender;
  int serial_number;
};




void writeDescriptor(char *type, int id, char * gender, int dur,struct timespec init_time, char* file_type){
  printf("\nentrou no writeDescriptor para escrever no ficheiro %s", file_type);
   FILE * fp;
   char pid[15];
   char file_name[100];
   struct timespec msg_time;


   clock_gettime(CLOCK_MONOTONIC_RAW, &msg_time);
   uint64_t delta_us = (msg_time.tv_sec - init_time.tv_sec) * 1000 + (msg_time.tv_nsec - init_time.tv_nsec) / 1000000;

  sprintf(pid, "%d", (int) getpid());
  sprintf(file_name,"%s", file_type);
  strcat(file_name, pid);

  fp = fopen (file_name, "a+");
  fprintf(fp, "%-10" PRIu64 " - %-10d - %-10d: %-10s - %-10d - %-10s\n",delta_us, getpid(),id,gender,dur,type);

  fclose(fp);

}

int readLine(int fd, char *str){
  int n;
  do{
      n = read(fd,str,1);
    }while (n>0 && *str++ != '\0');
    return (n>0);
}

struct Request getRequest(char * request_str){

   const char delimiter[2] = " ";
   char *word;
   struct Request r;

   word = strtok(request_str, delimiter);
   r.serial_number = atoi(word);

   word = strtok(NULL, delimiter);
   r.gender = word;

   word = strtok(NULL, delimiter);
   r.duration = atoi(word);

   return r;

}

void sendBackRequest(int fd, int id, char * gender, int dur, char * file){

  char request[100];

  sprintf(request, "%d", id);
  strcat(request, " ");
  strcat(request, gender);
  strcat(request, " ");
  char duration[100];
  sprintf(duration, "%d", dur);
  strcat(request, duration);


  if(write(fd, request, strlen(request)+1) != strlen(request)+1){
    perror(file);
    exit(3);
  }

  write(fd, "\n", 1);
    
}
