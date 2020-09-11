//Julio Villazón | A01370190

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int getMax(int *vector, int n);
void printH(int *v1, int *v2, int n);

int main(int argc, char * argv[]) {
  
  char *cmdopt;
  int cmd;  

  while ((cmd = getopt (argc, argv, "n:")) != -1)  
        switch (cmd)
    {
        case 'n':
            cmdopt = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf (stderr, "", optopt); //Default error description is enough
            else if (isprint (optopt))
                fprintf (stderr, "", optopt); //Default error description is enough
            else
                fprintf (stderr, "", optopt); //Default error description is enough
            return 1;
        default:
            abort ();
    }
  
  int n = atoi(cmdopt);

  int * v1 = (int *) malloc(n * sizeof(int));  
  int * final = v1 + n;

  int * v2 = (int *) malloc(n * sizeof(int));
  int * aux2 = v2;

  
  for (int * aux = v1; aux < final; ++aux)
  {
    pid_t pId;
    int status;
    
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

    pId = fork();
    
    if (pId == -1)
    {
        printf("Could not create child process\n");
        return -1;
    }
    else if (pId == 0)
    {      
      pid_t currentPID = getpid();
      int avg = (currentPID + getppid()) / 2;
      printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n", currentPID, avg);
              
      close(p1[0]);
      write(p1[1], &avg, sizeof(int));

      close (p2[0]);
      write(p2[1], &currentPID, sizeof(int));

      exit(0);
    }
    else 
    {
        if (waitpid(pId, &status, 0) != -1)
        {
          int a =WEXITSTATUS(status);
          if (WIFEXITED(status))
          {
            int eAvg = 0;
            close(p1[1]);
            read(p1[0], &eAvg, sizeof(int));

            *aux = eAvg;

            close(p2[1]);
            read (p2[0], aux2, sizeof(int));
          }
        }
    }
    aux2++;
    }
    aux2 = v2;
    printH(v1, v2, n);

    free(v1);
    free(v2);
    return 0;
}

int getMax(int *vector,int n)
{
  int max = 0;
  int *aux = vector;
  int *final = vector + n;

  for (aux = vector; aux < final; ++aux) 
  {
      if(max < *aux){
        max = *aux;
      }
	}
  return max;
}

void printH(int *v1, int *v2, int n)
{
  int max = getMax(v1, n);
  int factor = 10; 

  int *final = v1 + n;
  int *aux2 = v2;

  printf("\nChild pId \t Average\tHistogram\n\n");
  for (int *aux = v1; aux < final; ++aux) 
  {
    printf("%d: \t\t %d \t\t", *aux2, *aux);
    for (int i = 0; i < (*aux * factor) / max; ++i)
    {
      printf("*");
    }
    printf("\n");

    aux2++;
  }
}