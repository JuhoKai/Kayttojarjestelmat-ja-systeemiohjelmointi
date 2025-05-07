#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

int main(int argc, char *argv[]){

    FILE *file;
    char buffer[MAX];
    
    //Go through each file that is given
    for(int filenum=1; filenum < argc; filenum++){
        
        //Open file. If there is an error print error msg and exits w/ code 1
        if((file = fopen(argv[filenum], "r"))==NULL){
            printf("my-cat: cannot open file\n");
            exit(1);
        }

        //Print file content
        while(fgets(buffer, MAX, file)){
            printf("%s", buffer);
        }

        //Close file
        fclose(file);
    }

    return 0;
}