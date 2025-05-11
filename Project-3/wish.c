#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <fcntl.h>
#include <time.h>
#include "wish.h"

void errorHandle(char* errMsg){
    if(errMsg == NULL) errMsg = "An error has occurred\n";
    write(STDERR_FILENO, errMsg, strlen(errMsg));
    return;
}

void exitHandle(Node_t* tokens, Node_t* path){
    freeList(tokens, NULL);
    freeList(path, NULL);
    exit(0);
}

void errorExitHandle(Node_t* tokens, Node_t* path, char* errMsg){
    errorHandle(errMsg);
    freeList(tokens, NULL);
    freeList(path, NULL);
    exit(1);
}

int readInput(char* buffer, FILE* fp){
    size_t bufsize = BUFSIZE*sizeof(char);
    return getline(&buffer, &bufsize, fp);
}

//parses the input with strsep returns the amount of tokens
Node_t* parseInput(char* buffer, Node_t *tokens){
    char* token = strtok(buffer, " \f\n\r\t\v");

    while(token != NULL){
        tokens = addToList(tokens, token);
        token = strtok(NULL, " \f\n\r\t\v");
    }

    return tokens;
}

int changeDirectory(Node_t* tokens){
    if(getValue(tokens, 1) == NULL){
        printf("No directory specified.\n");
        return 1;
    }

    if(chdir(getValue(tokens, 1)) == -1){
        printf("Directory not found.\n");
        return 1;
    }

    return 0;
}

int setPath(Node_t* tokens, Node_t* path){
    if(getValue(tokens, 1) == NULL){
        printf("No path specified.\n");
        return 1;
    }

    //clear the path
    freeList(path->next, NULL);
    //Copy from tokens to path
    for(Node_t* tmp = tokens->next;tmp != NULL; tmp = tmp->next){
        addToList(path, tmp->value);
    }

    return 0;
}


//TODO Nice to have: handle uppers and lowers
int handleShellCommands(Node_t* tokens, Node_t* path){
    if(strcmp(getValue(tokens, 0), "exit") == 0){
        exitShellMsg();
        exitHandle(tokens, path);
    } else if(strcmp(getValue(tokens, 0), "help") == 0){
        helpText();
        return(0);
    } else if(strcmp(getValue(tokens, 0), "cd") == 0){
        changeDirectory(tokens);
        return(0);
    } else if(strcmp(getValue(tokens, 0), "path") == 0){
        setPath(tokens, path);
        return(0);
    }
    return -1;
}

int checkPath(Node_t* tokens, Node_t* path){
    char checkPath[PATHLENGTH];
    for(Node_t* tmp = path; tmp != NULL ;tmp = tmp->next){
        strcpy(checkPath, tmp->value);
        strcat(checkPath, "/");
        strcat(checkPath, tokens->value);
        if(access(checkPath, F_OK)==0){
            return(0);
        }
    }
    printf("Command not in path\n");
    return(-1);
}

int handleExternalCommands(Node_t* tokens, Node_t* path, int bgFlag, int index){
    int pid;
    char* argv[PATHLENGTH];

    //empty argv
    memset(argv, 0, sizeof(argv));
    getArray(tokens, argv, index);

    //fork
    switch(pid = fork()){
        case -1:
            errorExitHandle(tokens, path, "Fork failed\n");
        case 0:
            if(checkPath(tokens, path) == 0) if(execvp(argv[0], argv) == -1) errorExitHandle(tokens, path, "Exec failed\n");
            break;
        default:
            if(bgFlag == 0) if(wait(&pid) == -1) errorExitHandle(tokens, path, "Wait failed\n");
            break;
    }

    return(0);
}

int handleTokenInput(Node_t* tokens, Node_t* path){
    int bgFlag = 0;
    int length = 0;

    //save previous stdin and stdout
    int oldStdin = dup(STDIN_FILENO);
    int oldStdout = dup(STDOUT_FILENO);

    //Go through the tokens, split and call commands
    for(Node_t* tmp = tokens; tmp != NULL; tmp = tmp->next){
        if(strcmp(tmp->value, "&") == 0){
            bgFlag = 1;
            handleTokenInput(tmp->next, path);
            break;
        } else if(strcmp(tmp->value, ">") == 0){
            if(tmp->next == NULL){
                printf("No file specified.\n");
                return(1);
            }
            if(dup2(open(tmp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644), 1) == -1) errorHandle("Redirect failed\n");
            break;
        } else if(strcmp(tmp->value, "<") == 0){
            if(tmp->next == NULL){
                printf("No file specified.\n");
                return(1);
            }
            if(dup2(open(tmp->next->value, O_RDONLY), 0) == -1) errorHandle("Redirect failed\n");
            break;
        }
        length++;
    }
    if(tokens == NULL) return(0);

    if(handleShellCommands(tokens, path) != 0){
        handleExternalCommands(tokens, path, bgFlag, length);
    }

    //restore stdin and stdout
    dup2(oldStdin, STDIN_FILENO);
    dup2(oldStdout, STDOUT_FILENO);
    return(0);
}

//shell 
int main(int argc, char* argv[]){
    //set up the path
    char inputBuffer[BUFSIZE]; 
    Node_t* tokens = NULL;  
    Node_t* path = NULL; 
    int iaFlag = 1;
    int oldStdin = dup(STDIN_FILENO);


    if(argc > 2){
        errorExitHandle(tokens, path, "Too many arguments\n");
    } else if (argc == 2){
        if(strcmp(argv[1], "-h") == 0){
            helpText();
            return(0);
        } else {
            iaFlag = 0;
            FILE* fp;
            if((fp = fopen(argv[1], "r")) == NULL) errorExitHandle(tokens, path, "File not found\n");
            int fnum = fileno(fp);
            if(fnum == -1) errorExitHandle(tokens, path, "File not found.\n");
            dup2(fnum, STDIN_FILENO);
        }
    }
    
    //clear arrays
    memset(inputBuffer, 0, BUFSIZE*sizeof(char));

    path = addToList(path, DEFFAULTPATH);

    signal(SIGALRM, autoLogout);


    if(iaFlag != 0) welcomeText();

    //shell loop
    while(1){
        alarm(LOGOUTITME); //autologout
        
        if(iaFlag != 0) printf(PROMPT);

        if (readInput(inputBuffer, stdin) == -1){
            printf("File has been processed\n");
            exitHandle(tokens, path);
        };

        tokens = parseInput(inputBuffer, tokens);

        handleTokenInput(tokens, path);

        //clearing buffers
        memset(inputBuffer, 0, BUFSIZE*sizeof(char));
        tokens = freeList(tokens, NULL);

        puts("");
    }
    //return stdin to normal
    dup2(oldStdin, STDIN_FILENO);

    exitHandle(tokens, path);
    return(0);
}