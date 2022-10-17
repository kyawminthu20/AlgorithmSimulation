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
#define MAX_LINE_LENGTH 5


struct processInfo {
    char processID;
    int burstTime;
    int arrivalTime;
};

void printGanttChart(char *schAlgName, struct processInfo processList[8])
{
  printf("%s", schAlgName);
  printf("\n");
    
}

int readProcessInformation(struct processInfo processList[8])
{
  //Open the file for reading
  char *lineBuffer = NULL;
  size_t lineBufferSize = 0;
  unsigned int lineCount = 0;
  ssize_t lineSize;
  char *fileName = "Prog3inputfileF22.txt";
  FILE *fp = fopen(fileName, "r");
  char line[MAX_LINE_LENGTH] = {0};


  if(!fp){
    printf("Error Opening file '%s' \n", fileName);
    return 1;
  }

  /* Get the first line of the file */
  lineSize = getline(&lineBuffer, &lineBufferSize, fp);
  char *deLim = " ";

  /* Loop through until wre are done with the file */
  while(lineSize >= 0){
    /* Increment line count */
    lineCount++;

    /* Show the line details */
    //printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", lineCount,lineSize, lineBufferSize, lineBuffer);
    printf("%5d\n",sizeof(lineBuffer));
    
    /* Get the next line */
    lineSize = getline(&lineBuffer, &lineBufferSize, fp);
   }

  

  /* Free the allocated line buffer */
  free(lineBuffer);
  lineBuffer = NULL;
  /* Close the file */
  fclose(fp);

  return 0;
}


int main(int argc, const char * argv[]) {

  struct processInfo processList[8];
    
  printf("Algorithm Simulation");
  printf("\n");

  
  printGanttChart("Round Robin Algorithm Gantt Chart", processList);
  
  readProcessInformation(processList);
  
  return 0;
}
