// ./predication -o predicated -l 1023 -v 128 -s 1 -r 10

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>
#include <immintrin.h>
#include "functions.h"
#include "lfsr.h"



static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s -o <[normal, predicated]> -l <lenght> -v <value> -s <vector_size> -r <number_of_repetitions>\n", progname);
  exit(EXIT_FAILURE);
};


int main(int argc, char *argv[])
{
  int opt, value;
  char *str1, *str2, *val, *len, *operation;
  double start, end, elapsed;
  long long int lenght;
  vector_t *base_vec, *vec_cmp, *V;
  vector_s vector_size, vector_bytes, i, iterations;


/* ====================== TRATAMENTO DE LINHA DE COMANDO ====================== */
  if (argc < 11)
    usage(argv[0]);
  
  while ( (opt = getopt (argc, argv, "o:l:v:s:r:")) != -1 )
  {
    switch (opt)
    {
      case 'o':
        operation = optarg;
        break;
      
      case 's':
        str1 = optarg;
        break;

      case 'r':
        str2 = optarg;
        break;

      case 'l':
        len = optarg;
        break;

      case 'v':
        val = optarg;
        break;

      case ':':
        printf("Essa opção precisa de um valor!\n");
        break;

      case '?':
        printf("Opção desconhecida: %c\n", optopt);
        break;

      default:
        usage(argv[0]);
    }
  }
/* ====================== FIM DO TRATAMENTO DE LINHA DE COMANDO ====================== */
  
  vector_bytes = atoll(str1) * 1024 * 1024;
  vector_size = vector_bytes / sizeof(vector_t);
  iterations = atoll(str2);
  lenght = atoll(len);
  value = atoi(val);



//ALOCAGEM DOS VETORES
  base_vec = allocate_vector(vector_bytes);
  vec_cmp = allocate_vector(vector_bytes);
  V = allocate_vector(vector_bytes);


  if (!base_vec || !vec_cmp || !V)
  {
    fprintf(stderr, "Erro ao alocar os vetores utilizados no algoritmo!\n");
    exit(EXIT_FAILURE);
  }


//INICIALIZAÇÃO DOS VETORES COM OS VALORES (ALEATÓRIOS)
  init_vector(base_vec, vector_size, lenght);
  init_vector_cmp(vec_cmp, vector_size, value);


//impressão do tamanho [em MBtyes] a ser testado
  //fprintf(stdout, "%llu  ", atoll(str1));

//-------------------------------------------------------------- []
    if (!strcmp(operation, "normal"))
    {
      start = timestamp();
      for (i = 0; i < iterations; ++i)
      {
        sum_selection_normal(base_vec, vector_size, value);
      }
      end = timestamp();
      elapsed = end - start;
      fprintf(stdout, "%.8g\n", elapsed);
    }
    else if (!strcmp(operation, "predicated"))
    {
      start = timestamp();
      for (i = 0; i < iterations; ++i)
      {
        predicate(base_vec, vec_cmp, V, vector_size, value);
      }
      end = timestamp();
      elapsed = end - start;
      fprintf(stdout, "%.8g\n", elapsed);
    }
    else
    {
      usage(argv[0]);
    }
    


//DESALOCAGEM DOS VETORES
  free(base_vec);
  free(vec_cmp);
  free(V);

  exit(EXIT_SUCCESS);
}