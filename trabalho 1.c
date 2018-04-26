#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>

forkBranch(int hAtual, int h) // hAtual = altura atual, h = altura inicial
{
  pid_t idPEsq;
  pid_t idPDir;
  int statusE, statusD;
  hAtual++;

  if(hAtual > h) // testa se a altura atual é menor que a altura da árvore
    return;

  idPEsq = fork();

  if(idPEsq < 0){ // erro da esquerda
    fprintf(stderr, "fork falhou\n");
    exit(-1);
  }else if(idPEsq == 0){  // filho da esquerda
    printf("n = %d      C[%d, %d]\n", hAtual, getpid(), getppid());
    forkBranch(hAtual, h);
    printf("           T[%d, %d]\n", getpid(), getppid());
    exit(0);
  }else{ //  pai da esquerda -> vai pra direita
    waitpid(idPEsq, &statusE, 0); // espera os da esquerda
    idPDir = fork();

    if(idPDir < 0){ // erro da direita
      fprintf(stderr, "fork falhou\n");
      exit(-1);
    }else if(idPDir == 0){  // filho da direita
      printf("n = %d      C[%d, %d]\n", hAtual, getpid(), getppid());
      forkBranch(hAtual, h);
      printf("           T[%d, %d]\n", getpid(), getppid());
      exit(0);
    }
    waitpid(idPDir, &statusD, 0);
  }
  return;
}

forkLivre(int hAtual, int h) // hAtual = altura atual, h = altura inicial
{
  pid_t idPEsq;
  pid_t idPDir;
  int statusE, statusD;
  hAtual++;

  if(hAtual > h) // testa se a altura atual é menor que a altura da árvore
    return;

  idPEsq = fork();

  if(idPEsq < 0){ // erro da esquerda
    fprintf(stderr, "fork falhou\n");
    exit(-1);
  }else if(idPEsq == 0){  // filho da esquerda
    printf("n = %d      C[%d, %d]\n", hAtual, getpid(), getppid());
    forkLivre(hAtual, h);
    printf("           T[%d, %d]\n", getpid(), getppid());
    exit(0);
  }else{ //  pai da esquerda -> vai pra direita
    idPDir = fork();

    if(idPDir < 0){ // erro da direita
      fprintf(stderr, "fork falhou\n");
      exit(-1);
    }else if(idPDir == 0){  // filho da direita
      printf("n = %d      C[%d, %d]\n", hAtual, getpid(), getppid());
      forkLivre(hAtual, h);
      printf("           T[%d, %d]\n", getpid(), getppid());
      exit(0);
    }
    waitpid(idPEsq, &statusE, 0); // espera os da esquerda
    waitpid(idPDir, &statusD, 0); // espera os da direita
  }
  return;
}

int main(int argc, char const *argv[]) {
  int h = atoi(argv[1]);
  if(h <= 1 ){
    printf("Altura inválida!\n");
    exit(0);
  }
  int hAtual = 0, id = getpid();

  struct timeval tv1, tv2, tvB, tvL; // variáveis de tempo

  //confirma a altura, se por acaso vierem 2 ou mais argumentos,
  printf("Usando altura = %d\n", h); // já que só é tratado o primeiro

  gettimeofday(&tv1, NULL); // tempo inicial Branch

  printf("------------- Branch -------------\n\n");
  printf("n = %d      PID = %d (root)\n", hAtual, id);
  forkBranch(hAtual, h);  // função que vai para a árvore de branch
  printf("\n--- Fim da criação por Branch ---\n");

  gettimeofday(&tvB, NULL); // tempo final Branch

  gettimeofday(&tv2, NULL); // tempo inicial Livre

  printf("\n\n----------- Livre -----------\n\n");
  printf("n = %d      PID = %d (root)\n", hAtual, id);
  forkLivre(hAtual, h);  // função que vai para a árvore livre
  printf("\n--- Fim da criação Livre ---\n\n");

  gettimeofday(&tvL, NULL); // tempo final Livre

  if(getpid() == id){ // só entra quando a raiz terminar
    float tpB = (double)(tvB.tv_usec - tv1.tv_usec) / 1000000 + (double)(tvB.tv_sec - tv1.tv_sec);
    float tpL = (double)(tvL.tv_usec - tv2.tv_usec) / 1000000 + (double)(tvL.tv_sec - tv2.tv_sec);

    printf("Tempo execucao 'Branch': %f seg\n", tpB);
    printf("Tempo execucao 'Livre': %f seg\n", tpL);
    printf("Fim PID = %d (root)\n", id);
  }

  return 0;
}
