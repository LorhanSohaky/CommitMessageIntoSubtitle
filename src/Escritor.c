#include <stdio.h>
#include <string.h>
#include <time.h>

#define STRING_SIZE 3000
#define MSG_SIZE 5
#define DATE_SIZE 6

time_t convert_date(const char *string);

/*
Lê o conteúdo do arquivo e procura pelas tags '<date>','</date>','<msg>' e '</msg>'. Quando ele encontra a data, ele faz a
conversão para timestamp e escreve no arquivo. Quando ele encontra a mensagem, ele escreve a mensagem no arquivo.
*/
int main( int argc, char const *argv[] ){
  if( argc < 2 ){
    fprintf(
      stderr, "\n\tERRO!\nInforme o arquivo de log do svn!\nUse:\n\t%s projetolog.xml", argv[0] );
      fflush( stderr );

      return -1;
    }

    FILE *input_file, *output_file;
    input_file = fopen( argv[1], "r" );
    if( !input_file ){
      return -1;
    }

    output_file = fopen( "timestamp.txt", "w" );
    if( !output_file ){
      fclose( input_file );
      return -1;
    }
    char *start,*end, string[STRING_SIZE];
    do {
      if(fgets(string,sizeof(char)*STRING_SIZE,input_file)==NULL){
        break;
      }
      if((start=strstr(string,"<date>"))){
        end=strstr(string,"</date>");
        start+=DATE_SIZE;
        end-=8;
        start[end-start]='\0';
        fprintf(output_file, "%ld|",convert_date(start));
      }else if((start=strstr(string,"<msg>"))){
        start+=MSG_SIZE;
        if((end=strstr(string,"</msg>"))){
          *end='\0';
          fprintf(output_file,"%s\n",start);
        }else{
          do{
            if(!end && string[strlen(string)-1]=='\n'){
              string[strlen(string)-1]=' ';
              fprintf(output_file,"%s",start);
            }else if(!end && string[strlen(string)-1]!='\n'){
              fseek(input_file,-MSG_SIZE-1,SEEK_CUR);
              string[strlen(string)-MSG_SIZE-1]='\0';
              fprintf(output_file,"%s",start);
            }
            fgets(string,sizeof(char)*STRING_SIZE,input_file);
            end=strstr(string,"</msg>");
            start=string;
            if(end){
              *end='\0';
              fprintf(output_file,"%s",start);
              break;
            }
          }while(1);
          fprintf(output_file,"\n");
        }
      }
    } while(!feof(input_file));

    fclose(input_file);
    fclose(output_file);

    return 0;
  }

  /*
  Recebe a string da data e converte para o formato timestamp, retorna time_t
  */
  time_t convert_date(const char *string){
    int year, month, day, hour, min, sec;
    sscanf( string, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &min, &sec );
    struct tm data;

    data.tm_year = year - 1900;
    data.tm_mon = month - 1;
    data.tm_mday = day;
    data.tm_hour = hour;
    data.tm_min = min;
    data.tm_sec = sec;

    return mktime( &data );
  }
