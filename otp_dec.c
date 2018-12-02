/************************
 * otp_dec
 * Name: Andrew Snow
 * Program 4
 * CS 344
 * **********************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>


int main(int argc, char* argv[]){

    int sock;
    int port;
    int charsWritten;
    int charRead;
    int i = 0;
    long int textSize;
    long int keySize;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char arg[80000];
    char text[80000];
    char key[80000];
    FILE *textFile;
    FILE *keyFile;


    //Check for arguments
    if (argc < 4 || argc > 4){
        fprintf(stderr, "ERROR: Wront Number Of Arguments\n");
    
    } else {
        
        //Check for bad characters
        textFile = fopen(argv[1], "r");

        if (textFile == NULL){
            fprintf(stderr, "ERROR: File Does Not Exist");
            exit(1);
        }

        fseek(textFile, 0L, SEEK_END);
        textSize = ftell(textFile);
        rewind(textFile);

        fgets(text, textSize, textFile); 
        
        //Check for bad characters
        while (i < strlen(text)){
            if (!isalpha(text[i]) && text[i] != ' '){
                fprintf(stderr, "ERROR: Bad Character\n");
                exit(1);
            }

            i++;
        }
        

        //Open Key File
        keyFile = fopen(argv[2], "r");

        if (keyFile == NULL){
            fprintf(stderr, "ERROR: File Does Not Exist\n");
            exit(1);
        }

        fseek(keyFile, 0L, SEEK_END);
        keySize = ftell(keyFile);
        rewind(keyFile);

        fgets(key, keySize, keyFile);

        //test
        //printf("text: %s\n", text);
        //printf("key: %s\n", key);

        //Check if keysize is shorter than text
        if (textSize > keySize -1){
            fprintf(stderr, "ERROR: Key is to short");
            exit(1);
        }

        fclose(keyFile);
        fclose(textFile);

        //Set Up Variables
        strcpy(arg, text);
        strcat(arg, "\n");
        strcat(arg, key);
        strcat(arg, "\ndec");

        //test
        //printf("arg: %s", arg);

        /**********************
         *Set Up Connection To Server
         * *******************/

        memset((char*)&serverAddress, '\0', sizeof(serverAddress));
        port = atoi(argv[3]);
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverHostInfo = gethostbyname("localhost");
        memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

        //Set Up Sockets
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0){
            fprintf(stderr, "Unable To Open Socket\n");
        }
        
        //Connect To Server
        if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
            fprintf(stderr, "Unable To Connect To Server\n");
        }

        //Send Argument To Server
        charsWritten = send(sock, arg, strlen(arg), 0);

        if (charsWritten < 0){
            fprintf(stderr, "Unable To Write To Server\n");
        }



        //Recv Encripted Message From Server
        memset(text, '\0', sizeof(text));
        charRead = recv(sock, text, sizeof(text), 0);

        if (charRead < 0){
            fprintf(stderr, "Nothing Recieved From Server\n");
        }

        if (strcmp("Error1", text) == 0){
            fprintf(stderr, "ERROR: Refused Connection\n");
            exit(2);

        } else {

            i = 0;    
            while (i < textSize -1){
                printf("%c", text[i]);
                i++;
            }
            printf("\n");
        }
    }

    close(sock);
    return 0;

}
