#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>

void forkBranch(int h1, int h)    // Branch
{
  pid_t idPEsq;
  pid_t idPDir;
  int statusE, statusD;

  h1++;

  if(h1 > h) // testa se a altura atual é menor que a altura da árvore
    return;

  idPEsq = fork();

  if(idPEsq < 0){    // erro
    fprintf(stderr, "fork falhou\n");
    exit(-1);
  }else if(idPEsq == 0){   // filho
    printf("n = %d      C[%d, %d]\n", h1, getpid(), getppid());
    forkBranch(h1, h);
    printf("           T[%d, %d]\n", getpid(), getppid());
    exit(0);
  }else{     //  pai
    waitpid(idPEsq, &statusE, 0);
    idPDir = fork();

    if(idPDir < 0){    // erro
      fprintf(stderr, "fork falhou\n");
      exit(-1);
    }else if(idPDir == 0){   // filho
      printf("n = %d      C[%d, %d]\n", h1, getpid(), getppid());
      forkBranch(h1, h);
      printf("           T[%d, %d]\n", getpid(), getppid());
      exit(0);
    }
    waitpid(idPDir, &statusD, 0);
  }
  return;
}


forkLivre(int h1, int h)    // Livre
{
  pid_t idPEsq;
  pid_t idPDir;
  int statusE, statusD;

  h1++;

  if(h1 > h) // testa se a altura atual é menor que a altura da árvore
    return;

  idPEsq = fork();

  if(idPEsq < 0){    // erro
    fprintf(stderr, "fork falhou\n");
    exit(-1);
  }else if(idPEsq == 0){   // filho
    printf("n = %d      C[%d, %d]\n", h1, getpid(), getppid());
    forkLivre(h1, h);
    printf("           T[%d, %d]\n", getpid(), getppid());
    exit(0);
  }else{     //  pai
    idPDir = fork();

    if(idPDir < 0){    // erro
      fprintf(stderr, "fork falhou\n");
      exit(-1);
    }else if(idPDir == 0){   // filho
      printf("n = %d      C[%d, %d]\n", h1, getpid(), getppid());
      forkLivre(h1, h);
      printf("           T[%d, %d]\n", getpid(), getppid());
      exit(0);
    }
  waitpid(idPEsq, &statusE, 0);
  waitpid(idPDir, &statusD, 0);
  }
  return;
}

int main(int argc, char const *argv[]) {
  int h = atoi(argv[1]);
  if(h <= 1 ){
    printf("Altura inválida!\n");
    exit(0);
  }
  int h1 = 0, id = getpid(), i;

  struct timeval tv1, tv2, tvB, tvL; // variáveis de tempo
  gettimeofday(&tv1, NULL);

  printf("------------- Branch -------------\n\n");
  printf("n = %d      PID = %d (root)\n", h1, id);
  forkBranch(h1, h);  // função que vai para a árvore de branch

  printf("\n--- Fim da criação por Branch ---\n");
  gettimeofday(&tvB, NULL);

  gettimeofday(&tv2, NULL);
  printf("\n\n----------- Livre -----------\n\n");
  printf("n = %d      PID = %d (root)\n", h1, id);
  forkLivre(h1, h);  // // função que vai para a árvore livre

  printf("\n--- Fim da criação Livre ---\n\n");
  gettimeofday(&tvL, NULL);

  if(getpid() == id){ // só entra quando a raíz terminar
    float tpB = (double)(tvB.tv_usec - tv1.tv_usec) / 1000000 + (double)(tvB.tv_sec - tv1.tv_sec);
    float tpL = (double)(tvL.tv_usec - tv2.tv_usec) / 1000000 + (double)(tvL.tv_sec - tv2.tv_sec);

    printf("Tempo execucao 'Branch': %f seg\n", tpB);
    printf("Tempo execucao 'Livre': %f seg\n", tpL);
    printf("Fim PID = %d (root)\n", id);
  }

  return 0;
}
