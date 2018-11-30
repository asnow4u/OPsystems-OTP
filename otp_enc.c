/************************
 * otp_enc
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



int main(int argc, char* argv[]){

    int sock;
    int port;
    int charsWritten;
    int charRead;
    int i = 0;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char buffer[256];
    char arg[256];
    char text[300];
    FILE *textFile;


    //Check for arguments
    if (argc < 4 || argc > 4){
        fprintf(stderr, "ERROR: Wrong Number Of Arguments\n");
    
    } else {
      
        //Check for bad characters
        textFile = fopen(argv[1], "r");
        
        if (textFile == NULL){
            fprintf(stderr, "ERROR: File Does Not Exist\n");
            exit(1);
        }

        fgets(text, sizeof(text), textFile); 

        while (i < strlen(text) -1){
            if (!isalnum(text[i]) && text[i] != ' '){
                fprintf(stderr, "ERROR: Bad Character");
                exit(1);
            }

            i++;
        }

        fclose(textFile);

        //Set Up Variables
        strcpy(arg, argv[1]);
        strcat(arg, "\n");
        strcat(arg, argv[2]);
        strcat(arg, "\nenc");

        /***********************
         * Setup the Conection to the server
         * ********************/
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
            fprintf(stderr, "Unable To Connect To The Server\n");
        }

        //Send Argument To Server
        charsWritten = send(sock, arg, strlen(arg), 0);

        if (charsWritten < 0){
            fprintf(stderr, "Unable To Write To The Server\n");
        }

        
        //Recv Encripted Message From Server
        memset(buffer, '\0', sizeof(buffer));
        charRead = recv(sock, buffer, sizeof(buffer), 0);

        if (charRead < 0){
            fprintf(stderr, "Nothing Recieved From Server\n");
        }

        printf("%s\n", buffer);

    }

    close(sock);
    return 0;

}
