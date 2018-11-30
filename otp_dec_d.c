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

    if (i <= 0) {
        i += 26;
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


int main(int argc, char* argv[]){

    //Function Varaiables
    FILE *textFile;
    FILE *keyFile;
    char *text;
    char *key;
    char *client;
    char buffer[300];
    char *message;
    int i=0;
    int child;
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
        error("ERROR: Not Propper Amount Of Arguments\n");

    } else {

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
            error("ERROR: Unable To Open Socket\n");
        }
        
        //Check Bind
        if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
            error("ERROR: Unable To Bind\n");
        }

        listen(sock, 5);

        //Accept Connection
        sizeOfClientInfo = sizeof(clientAddress);
        connection = accept(sock, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);

        //Check Accept
        if (connection < 0){
            error("ERROR: Unable To Connect\n");
        }

        /********************************
         * Fork Process
         * Grab Incoming Data From Client
         * Divide up by \n
         * Remove tailing \n
         * Open Files
         * ****************************/

        child = fork();

        switch(child){

            case -1:
                fprintf(stderr, "ERROR: Child Process Not Formed\n");
                exit(1);

            /****************************
             * Child Process
             * ************************/

            case 0:

            charsRead = recv(connection, buffer, sizeof(buffer), 0);

            if (charsRead < 0){
                error("ERROR: Unable To Read From The Socket\n");
            }

            text = strtok(buffer, "\n");
            key = strtok(NULL, "\n");
            client = strtok(NULL, "\n");
            
            //Check for correct client
            if (strcmp("dec", client) != 0){
                fprintf(stderr, "ERROR: Refused Connection");
                exit(2);
            }

            /***************************
            * Check text and key files
            * Grab strings
            * Compare Length (if difference send error)
            * ************************/

        
            textFile = fopen(text, "r");
            keyFile = fopen(key, "r");

            if (textFile == NULL) {
                fprintf(stderr, "ERROR: Can't Open File\n");
                exit(1);
            }

            if (keyFile == NULL) {
                fprintf(stderr, "ERROR: Can't Open File\n");
                exit(1);
            }

            //Find Size of TextFile
            fseek(textFile, 0L, SEEK_END);
            textSize = ftell(textFile);
            rewind(textFile);
            text = malloc(textSize +1);

            //Find Size of keySize
            fseek(keyFile, 0L, SEEK_END);
            keySize = ftell(keyFile);
            rewind(keyFile);
            key = malloc(keySize +1);

            fgets(text, textSize, textFile);
            fgets(key, keySize, keyFile);

            //Close Files
            fclose(textFile);
            fclose(keyFile);

            if (strlen(text) != strlen(key) -1){
                fprintf(stderr, "ERROR: Text and Key are not the same length\n");
                exit(1);
            }

            message = (char*)malloc(strlen(text));
        
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
            
            default:

            /*******************
             * Parent Case
             * ***************/

            waitpid(child, NULL, 0);
        }
    }
    
    close(sock);

    return 0;

}
