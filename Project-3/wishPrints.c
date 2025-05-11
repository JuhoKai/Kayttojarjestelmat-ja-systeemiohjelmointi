#include <stdio.h>
#include <stdlib.h>
#include "wish.h"

void autoLogout(){
    printf("You have been logged out due to inactivity.\n");
    exit(0);
}

void welcomeText(){
    printf("Welcome to the wish shell.\n");
    printf("Autologout has bee set to %d seconds.\n", LOGOUTITME);
    printf("Type 'help' for a list of commands.\n");
    return;
}

void helpText(){
    printf("Available commands:\n");
    printf("'help' - Prints this help text.\n");
    printf("'cd' - Changes the current working directory.\n");
    printf("'path' - Sets a new path.\n");
    printf("'exit' - Exits the shell.\n");
    return;
}

void exitShellMsg(){
    printf("Exiting shell...\n");
    return;
}