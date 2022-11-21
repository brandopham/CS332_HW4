#include "header.h"

char *cpyInputStr(char *str, int i) {
    int chr;
    char *temp_line = malloc(sizeof(char) * strlen(str));
    for (;(chr = str[++i]) != '\0';)
        temp_line[i] = chr;
    temp_line[i] = '\0';    

    return temp_line;
}

bool nullChar(char chr) {
    if (chr == '\r' || chr == '\n' || chr == ' ' || chr == '\x0c' || chr == '\x0b' || chr == '\t')
        return true;
    else 
        return false;
}

char *charPointer1(char *str, int i) {
    char *fcp;
    for(i=0; nullChar(str[i])==true;)        
        i++;        
    fcp = str + i;

    return fcp;
}

int inputs(int k, char *str, int w) {
    int i, chr;
    for (i = 0; i < k - 1 && (chr = getchar()) != '\n'; ++i) {
        if (chr == EOF)
            return -1;
        str[i] = chr;
    }
    str[i] = '\0';

    return i;
}

char *cpyNewStr(char *str, int i) {
    char *temp_line = malloc(sizeof(char) * strlen(str));;
    int  chr;

    for(;(chr = str[++i]) != '\0' && chr != '\n'; )
        temp_line[i] = chr;
    temp_line[i] = '\0';

    return temp_line;
}

char *timeDateStr(char *nsc) {
    time_t timer = time(NULL);
    nsc = cpyNewStr(ctime(&timer), -1);

    return nsc;
}

char **arguments(char *inputLine) {
    char *arg;
    char *temp_line = malloc(sizeof(char) * (strlen(inputLine) + 1));
    char **args = malloc(sizeof(char *));
    strcpy(temp_line, inputLine);

    int i = 0;
    while ((arg = strtok(temp_line, " \t")) != NULL) {
        args[i] = malloc(sizeof(char) * (strlen(arg) + 1));
        strcpy(args[i], arg);
        args = realloc(args, sizeof(char *) * (++i + 1));
        temp_line = NULL;
    }

    args[i] = NULL;

    return args;
}