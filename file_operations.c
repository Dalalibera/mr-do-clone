#include "file_operations.h"

//Carrega o arquivo de cenário inicial
//@TODO tratar erros?
FILE* load_level(int level){
  FILE *level_file;

  if(level == 1){
    level_file = fopen("level1.txt", "rb");
  }else if(level == 2){
    level_file = fopen("level2.txt", "rb");
  }else{
    level_file = fopen("continua.txt","rb");
  }

  return level_file;
}

//Gera o mapa de acordo com o arquivo de texto
//@TODO substituir os caracteres pelos corretos e tratamento de erros
void make_map(FILE *level, char p[MAX_Y][MAX_X]){

  long l_size;
  char *buffer;
  int i, j, cont;

  //Aloca espaço na memória e lê o arquivo carregado
  fseek(level, 0L, SEEK_END);
  l_size = ftell(level);
  rewind(level);
  buffer = calloc( 1, l_size + 1 );
  fread(buffer, l_size, 1, level);

  //Atualiza a matriz, ignorando caracteres fora do padrão
  for(i = 0; i < MAX_Y; i++){
    for(j = 0; j < MAX_X; j++){
      while(buffer[cont] != 'P' && buffer[cont] != 'c'){
        cont ++;
      }

      switch(buffer[cont]){

        case 'P': 
                  p[i][j] = '#';
                  break;
        case 'c': 
                  p[i][j] = ' ';
                  break;
        default: break;
      }
      cont++;
    }
  }

}