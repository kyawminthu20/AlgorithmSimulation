//
//  main.c
//  AlgorithmSimulation
//
//  Created by Kyaw Min Thu on 10/15/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 5
#define PROCESSID_PREFIX 9

typedef enum {Ready = 0, Waiting = 1, Running = 2, Terminated = 3, NotReady = -1} State;

const char* getStateName(State processState){
  switch(processState){
    case 0: return "Ready";
    case 1: return "Waiting";
    case 2: return "Running";
    case 3: return "Terminated";
    case -1: return "Not Ready";
  }
}

typedef enum {SJF = 0, SRTF = 1, RR = 2} SchedulingAlgorithm;

const char* getSchedulingAlgorithmName (SchedulingAlgorithm schAlg){
  switch(schAlg){
    case 0: return "SJF";
    case 1: return "SRTF";
    case 2: return "RR";
  }
}

typedef struct  {
  char *processID; 
  int burstTime;
  int arrivalTime;
  State processState;
  int runTime;
  int remainingTime;
}ProcessInfo;

ProcessInfo processList[8];

int readProcessInformation()
{
  //Open the file for reading
  char *lineBuffer = NULL;
  size_t lineBufferSize = 0;
  unsigned int lineCount = 0;
  ssize_t lineSize;
  char *fileName = "Prog3inputfileF22.txt";
  FILE *fp = fopen(fileName, "r");
  char line[MAX_LINE_LENGTH] = {0};

  //Local string array to store all line read
  char** lines = (char**) malloc(8*8*sizeof(char));
  if(!fp){
    printf("Error Opening file '%s' \n", fileName);
    return 1;
  }

  /* Get the first line of the file */
  lineSize = getline(&lineBuffer, &lineBufferSize, fp);
  unsigned count = 0;

  /* Loop through until wre are done with the file */
  while(lineSize >= 0 ){
    /* Increment line count */
    lineCount++;
    /* Show the line details */
    //printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s\n", lineCount,lineSize, lineBufferSize, lineBuffer);
    //printf("Line Number is : %3d and Line Size is %5d\n",lineCount, sizeof(lineBuffer));   
    lines[lineCount-1] = strdup(lineBuffer); // Add line buffer to string array lines.
    /* Get the next line */
    lineSize = getline(&lineBuffer, &lineBufferSize, fp);
  }

  for(int i=0; i < 8; i++){
    count = 0;
    char *deLim = " ";
    char *token = strtok(lines[i],deLim);

    while(token != NULL){
      //=========printf("Token no. %d : %s \n", count,token); 
      if(count == 0){
        strcpy(processList[i].processID, token);
        //========processList[i].processID = token;
        //=========printf("%s --> %s \n", token, processList[i].processID);
      }
      else if (count == 1){
        processList[i].burstTime = atoi(token);
        processList[i].remainingTime = processList[i].burstTime;
      }
      else if (count == 2 ){
        processList[i].arrivalTime = atoi(token);
      }
      else{
        printf("Error Reading file");
        return 1;
      }
      token = strtok(NULL,deLim);
      count++;
    }  
  }

  /* Free the allocated line buffer */
  free(lineBuffer);
  lineBuffer = NULL;
  /* Close the file */
  fclose(fp);
  return 0;
}

void displayProcessListSnapshot()
{
  printf("ProcessID | Burst Time | Arrival Time | Process State | Run Time | Remaining Time\n");
  printf("---------------------------------------------------------------------------------\n");

  for(int j = 0; j< 8; j++){
    printf("%9s | %10d | %12d | %13s | %8d | %8d\n", processList[j].processID, processList[j].burstTime, processList[j].arrivalTime, getStateName(processList[j].processState), processList[j].runTime, processList[j].remainingTime);

  }
}

void SJF_Simulation(int maxTime)
{
  char *readyQueue = (char*) malloc(sizeof(char)*(8+1));
  printf("\n\n");
  printf("SJF Scheduling\n");
  printf("Time  |  ProcessID | Status \n");
  printf("----------------------------\n");
  bool contextSwitch = true;
  for(int k=0; k <= maxTime; k++)
  {
    //========printf("Time %d\n", k);
    //========printf("=========\n");

    //Reduce Running time from Running process
    for(int j = 0; j < 8; j++)
    {
      if (processList[j].processState == Running){
        processList[j].remainingTime = processList[j].remainingTime -1;
      }
    }
    // Check if the processes are in the ready state
    for(int j = 0; j < 8; j++)
    {
      if (processList[j].arrivalTime == k){
        processList[j].processState = Ready;
        //======================printf("Process %s is ready at time %d .\n", processList[j].processID, k);
      }
    }

    // Check if processs are completed
    for(int j = 0; j < 8; j++)
    {
      if (processList[j].remainingTime == 0 && processList[j].processState == Running){
        processList[j].processState = Terminated;
        //=======================printf("Process %s is Terminated at time %d .\n", processList[j].processID, k);
        printf("%5d | %10s | %s \n", (k), processList[j].processID, getStateName(processList[j].processState));
        contextSwitch = true;
      }
    }
    // Check the shortest process Burst time
    int tempBurstTime = 1000;
    char *shortestBurstTimeProcessAtTime;
    shortestBurstTimeProcessAtTime = (char*) malloc(sizeof(char)*(2+1));
    for(int l=7; l >= 0; --l)
    {
      //printf(">> %d Process is %s \n", l, processList[l].processID);
      
      if(processList[l].processState == Ready)
      {
        if (processList[l].burstTime <= tempBurstTime ){
          tempBurstTime = processList[l].burstTime;
          strcpy(shortestBurstTimeProcessAtTime,"");
          strcpy(shortestBurstTimeProcessAtTime, processList[l].processID);        
        }
      }
    }
    //=========================printf("Shortest Burst Time at time %d is %3s. \n", k, shortestBurstTimeProcessAtTime);
    //Check if it is time to Switch
    //If there is no process under running State
    if(contextSwitch == true)
    {
      //========================printf("=======Context Switching======\n");
      //Start Executing shortest Burst Time process.
      for(int m = 0; m < 8; m++){
        if(strcmp(shortestBurstTimeProcessAtTime, processList[m].processID) == 0)
        {
          processList[m].processState = Running;
          contextSwitch = false;
        }
      }
    }
    //displayProcessListSnapshot();
  }
}

//#######################################################################
int main(int argc, const char * argv[]) {

  //Initialize all processes
  for(int i = 0; i < 8; i++)
  {
    processList[i].processID = (char*) malloc(sizeof(char)*(1+1));
    processList[i].burstTime = 0;
    processList[i].arrivalTime = 100;
    processList[i].processState = NotReady;
    processList[i].runTime = 0;
    processList[i].remainingTime = 0;
  }

  printf("Algorithm Simulation");
  printf("\n\n");

  readProcessInformation();
  
  int maxRunTime = 0;
  for(int j = 0; j< 8; j++){
    maxRunTime = maxRunTime + processList[j].burstTime;
  }
  // SJF Scheduling
  SJF_Simulation(maxRunTime);

  // SRTF Scheduling
  SRTF_Simulation(maxRunTime);
  return 0;
}
