#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_BUFFER_SIZE 	1024
#define MAX_FILENAME_SIZE 	64

/*
*  PARAMETRI:
*  1) FILE INPUT, CONTIENE IL NOME DEL FILE CON LE RIGHE DA RIPORTARE
*  2) FI_KEY_POS, CONTIENE LA POSIZIONE DEL CAMPO CHIAVE DEL FILE INPUT
*  3) KEY_LEN,    CONTIENE LA LUNGHEZZA IN CARATTERI DEL CAMPO CHIAVE DEL FILE INPUT
*  4) FILE_CHECK, CONTIENE IL NOME DEL FILE CON LE RIGHE CON CUI CONFRONTARE IL FILE INPUT
*  5) FC_KEY_POS, CONTIENE LA POSIZIONE DEL CAMPO CHIAVE DEL FILE DA CONFRONTARE (SI DA PER ASSUNTO CHE LA LUNGHEZZA DEVE ESSERE UGUALE AL PARAMETRO 3)
*  6) FILE_OUT,   CONTIENE IL NOME DEL FILE DOVE ANDARE A SCRIVERE I RECORD FILTRATI.
*/


int main (int argc, char *argv[]){

	char file_in[MAX_FILENAME_SIZE];
	char file_check[MAX_FILENAME_SIZE];
	char file_out[MAX_FILENAME_SIZE];
	char file_in_buffer[MAX_BUFFER_SIZE];
	char file_check_buffer[MAX_BUFFER_SIZE];

  char c;
  char progressbar[] = "                    ";
  int fi_key_pos;
  int fc_key_pos;
  int key_len;
  float rowcounter;
  float totalrows;
  float progress;
  FILE *f_in = NULL;
  FILE *f_check = NULL;
  FILE *f_out = NULL;

  if (argc != 7){
    /*printf("\nViFilter ver. %s\n", AutoVersion::UBUNTU_VERSION_STYLE);*/
    printf("\nUsage: vifilter.exe <FILE_INPUT> <FI_KEY_POS> <KEY_LEN> <FILE_CHECK> <FC_KEY_POS> <FILE_OUT>\n");
    printf("Verificare i parametri\n");
    exit(EXIT_FAILURE);
  }

start:
  strcpy( file_in, argv[1] );
  strcpy( file_check, argv[4] );
  strcpy( file_out, argv[6] );
  fi_key_pos = atoi(argv[2]);
  fc_key_pos = atoi(argv[5]);
  key_len = atoi( argv[3] );

  char fi_key[ key_len ];
  char fc_key[ key_len ];


   /*else printf("Booo!");*/

	if( ! (f_in = fopen( file_in, "r+" ) ) ) {
		perror(file_in);
		exit(EXIT_FAILURE);
	}
	clearerr(f_in);

	if( ! (f_check = fopen( file_check, "r+" ) ) ) {
		perror(file_check);
		exit(EXIT_FAILURE);
	}
	clearerr(f_check);

	if( ! (f_out = fopen( file_out, "w+" ) ) ) {
		perror(file_out);
		exit(EXIT_FAILURE);
	}
	clearerr(f_out);

  //system("cls");
  printf("\nProcessing: ");

   /* Ciclo lettura file_In */

  totalrows = 0;
  while( fgets(file_in_buffer, MAX_BUFFER_SIZE, f_in) ){
    totalrows++;
  }
  rowcounter = 0;

  // before entering update loop
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  GetConsoleScreenBufferInfo(h, &bufferInfo);

  rewind(f_in);
  while( fgets(file_in_buffer, MAX_BUFFER_SIZE, f_in) ){
      rowcounter++;
      progress = (float) (rowcounter / totalrows * 100.0);
      progressbar[ (int) floorf( fmod( (progress / 5), strlen(progressbar) ) ) ] = 177;
      printf("%2.1f %% [%s]", progress, progressbar );


      strncpy(fi_key, &file_in_buffer[ fi_key_pos - 1 ], key_len);
      fi_key[ key_len ] = '\0';
      SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
      while( fgets(file_check_buffer, MAX_BUFFER_SIZE, f_check) ){
         strncpy(fc_key, &file_check_buffer[ fc_key_pos - 1 ], key_len);
         fc_key[ key_len ] = '\0';
        /* printf("%s %s\n", fi_key, fc_key);*/
         if( strcmp(fi_key, fc_key) == 0 ){
           /* printf("TROVATO %s\n", fi_key);*/
            fputs(file_in_buffer, f_out);
            break;
         }
         /*else printf("MANCANTE %s\n", fi_key);*/
      }
      rewind(f_check);

   }

   printf("\nDone.");


free_resources:
   if(f_in){
      fclose(f_in);
      f_in = NULL;
   }
   if(f_check){
      fclose(f_check);
      f_check = NULL;
   }
   if(f_out){
      fclose(f_out);
      f_out = NULL;
   }

	return EXIT_SUCCESS;
}

