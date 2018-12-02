/********************
 * otp_dec_d
 * Name: Andrew Snow
 * Program 4
 * CS 344
 * ****************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


    /**************************
     * Convert char to defined number
     * **********************/

int findNum(char c){
    
    switch(c){
        case 'A':   return 0;
        case 'B':   return 1;
        case 'C':   return 2;
        case 'D':   return 3;
        case 'E':   return 4;
        case 'F':   return 5;
        case 'G':   return 6;
        case 'H':   return 7;
        case 'I':   return 8;
        case 'J':   return 9;
        case 'K':   return 10;
        case 'L':   return 11;
        case 'M':   return 12;
        case 'N':   return 13;
        case 'O':   return 14;
        case 'P':   return 15;
        case 'Q':   return 16;
        case 'R':   return 17;
        case 'S':   return 18;
        case 'T':   return 19;
        case 'U':   return 20;
        case 'V':   return 21;
        case 'W':   return 22;
        case 'X':   return 23;
        case 'Y':   return 24;
        case 'Z':   return 25;
        case ' ':   return 26;

    }

}


    /****************************
     * Find numaric value for ctext and ckey
     * Subtract values and find letters for message
     * ************************/

char decription(char ctext, char ckey){

    
    int i=0;
    int j=0;

    //Capitalize char
    ctext = toupper(ctext);

    //Find numaric value
    i = findNum(ctext);
    j = findNum(ckey);

    i -= j;

    if (i < 0) {
        i += 27;
    }

    switch(i){
        case 0:     return 'A';
        case 1:     return 'B';
        case 2:     return 'C';
        case 3:     return 'D';
        case 4:     return 'E';
        case 5:     return 'F';
        case 6:     return 'G';
        case 7:     return 'H';
        case 8:     return 'I';
        case 9:     return 'J';
        case 10:    return 'K';
        case 11:    return 'L';
        case 12:    return 'M';
        case 13:    return 'N';
        case 14:    return 'O';
        case 15:    return 'P';
        case 16:    return 'Q';
        case 17:    return 'R';
        case 18:    return 'S';
        case 19:    return 'T';
        case 20:    return 'U';
        case 21:    return 'V';
        case 22:    return 'W';
        case 23:    return 'X';
        case 24:    return 'Y';
        case 25:    return 'Z';
        case 26:    return ' ';
    
    }
}


/*******************************
 * Server Main
 * ****************************/


int main(int argc, char* argv[]){

    //Function Varaiables
    FILE *textFile;
    FILE *keyFile;
    char *text;
    char *key;
    char *client;
    char buffer[1000];
    char encMessage[200000];
    char *message;
    int i=0;
    int child;
    int numListen=0;
    long int textSize;
    long int keySize;

    //Server Variables
    int sock;
    int connection;
    int port;
    int charsRead;
    int charsWritten;
    socklen_t sizeOfClientInfo;
    struct sockaddr_in serverAddress, clientAddress;

    //Check arguments
    if (argc < 2 || argc > 2){
        fprintf(stderr, "ERROR: Not Propper Amount Of Arguments\n");

    } else {

        /*************************************
         * Set Up Server
         * *********************************/

        //Set Up Address Struct
        memset((char*)&serverAddress, '\0', sizeof(serverAddress));
        port = atoi(argv[1]);
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
         
        //Set Up Socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        //Check Socket
        if (sock < 0){
            fprintf(stderr, "ERROR: Unable To Open Socket\n");
        }
        
        //Check Bind
        if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
            fprintf(stderr, "ERROR: Unable To Bind\n");
        }

        listen(sock, 5);

        sizeOfClientInfo = sizeof(clientAddress);
 
        //Loop
        while((connection = accept(sock, (struct sockaddr *)&clientAddress, &sizeOfClientInfo)) > 0){

        
        /********************************
         * Fork Process
         * Grab Incoming Data From Client
         * Divide up by \n
         * Remove tailling \n
         * ****************************/

        //New child
        numListen++;

        child = fork();

        switch(child){

            case -1:
                fprintf(stderr, "ERROR: Child Process Not Formed\n");
                charsWritten = send(connection, "ERROR1", strlen("Error1"), 0);
                break;


            /****************************
             * Child Process
             * ************************/

            case 0:

            close(sock);
            
            //Grab from client
            while (encMessage[strlen(encMessage)-1] != 'c'){
                memset(buffer, '\0', sizeof(buffer));
                charsRead = recv(connection, buffer, sizeof(buffer) -1, 0);
                strcat(encMessage, buffer);
            }
           
            //divide up
            text = strtok(encMessage, "\n");
            key = strtok(NULL, "\n");
            client = strtok(NULL, "\n");
           

            //Check for correct client
            if (strcmp("dec", client) != 0){
                
                charsWritten = send(connection, "Error1", strlen("Error1"), 0);
                exit(2);
            }


            message = (char*)malloc(strlen(text));
            //memset(message, '\0', textSize * sizeof(char));

            //Decript Message
            while (i < strlen(text)) {
            
                message[i] = decription(text[i], key[i]);

                i++;
            }


            /**************************
            * Send Message To The Client
            * ***********************/


            charsWritten = send(connection, message, strlen(message), 0);

            //Check charWritten
            if (charsWritten < 0){
                fprintf(stderr, "ERROR: Unable To Write To Socket\n");
            }

            close(connection);
            exit(0);
            break;

            default:

            /*******************
             * Parent Case
             * ***************/
            
            close(connection);
            waitpid(child, NULL, 0);

            //Child Finished
            numListen--;
        }

        }
    }
    
    close(sock);

    return 0;

}
