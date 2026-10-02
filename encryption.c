#include <stdio.h>
#include <stdlib.h>

// Variable
char command[4095];
char ncode[4095];
int i=1;
int chr=0;
char message[4095];
int UPR=126;
int LWR=32;
int RNG=95;

//Escape
char * escp(char code[4095]){
    size_t len = strlen(code);
    for ( i = len ; i >= 0; i--){
        // 34 = '"'   36 = '$'  96 = '`'  92='\'
        if ( code[i] == 34 || code[i] == 36 || code[i] == 96){ 
            len = strlen(code);
            int pointer = len;
            while (pointer > i){
                code[pointer] = code[pointer - 1];
                pointer--;
            }
            code[i] = 92;
        }
    }
    return code;
}

//Overflow
char * ovflow(char code[4095]){
    if (code[chr] > UPR) {
        code[chr] -= RNG;
    }else if (code[chr] < LWR) {
        code[chr] += RNG;
    }return code;
}

//Encryption
void encrypt(char code[4095]){
    size_t len = strlen(code);
    while (i < len) {
        code[chr] += i;
        ovflow(code);
        chr += 1;
        i += 1;
    }

    printf("Encrypted Message:%s",code);
    escp(code);
    len = strlen(code);
    code[len-1] = code[len];
    sprintf(command, "termux-clipboard-set \"%s\"", code);
    system(command);
}

// Decrypt
void dcrypt(char code[4095]){
    size_t len = strlen(code);
    while (i < len) {
        code[chr] -=i;
        ovflow(code);
        chr += 1;
        i += 1;
    }
    printf("Original Message:%s", code);
}

//Get clipboard content into variable
char * getTermuxClipboard() {
    FILE * fp = popen("termux-clipboard-get", "r");
    if (!fp) return NULL;
    char * buffer = malloc(4095);
    size_t len = fread(buffer, 1, 4095, fp);
    buffer[len] = '\0';
    pclose(fp);
    return buffer;
}

int main (int argc, char * argv[])
{
    //If no arguments
    if (argc < 2) {

        //Get User Input
        printf("Original message:");
        fgets(message, sizeof(message), stdin);
        encrypt(message);

    //Decrypt
    } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "d") == 0) {

        sprintf(command, "termux-clipboard-get");
        char * message= getTermuxClipboard();
        size_t len = strlen(message);
        message[len] = '\n';
        printf("Encrypted Message:%s", message);

        dcrypt(message);
        
    //Encrypt with argument
    } else if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "e") == 0) {
        strcpy(message, argv[2]);

        size_t len = strlen(message);
        message[len] = '\n';

        printf("Original Message:%s", message);
        encrypt(message);

    //Invalid Arguments
    }else {
        printf("Invalid Argument");
    }
    return(0);
}
