/***************************************
 * Name: Andrew Snow
 * Program 4
 * CS 344
 * ************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/*******************
 * Find letter based on random number
 * ****************/

char findLetter (int num){

   switch (num){
    case 0: return('A');
    case 1: return('B');
    case 2: return('C');
    case 3: return('D');
    case 4: return('E');
    case 5: return('F');
    case 6: return('G');
    case 7: return('H');
    case 8: return('I');
    case 9: return('J');
    case 10: return('K');
    case 11: return('L');
    case 12: return('M');
    case 13: return('N');
    case 14: return('O');
    case 15: return('P');
    case 16: return('Q');
    case 17: return('R');
    case 18: return('S');
    case 19: return('T');
    case 20: return('U');
    case 21: return('V');
    case 22: return('W');
    case 23: return('X');
    case 24: return('Y');
    case 25: return('Z');
    case 26: return(' ');
    }
}


int main (int argc, char* argv[]){

    int keyLength;
    int i=0;
    char *key;
    int num;
    

    /*********************
    *Check Argument
    *Nessisary to have length of key
    *Throw error otherwise
    *********************/

    if (argc > 2){
        perror("Error: To many arguments\n");
        exit(1);

    } else if (argc < 2){
        perror("Error: Need a specified key length\n");
        exit(1);
    
    } else {
       
       keyLength = atoi(argv[1]);
       
       key = (char*)malloc(keyLength + 1);
    
       srand(time(NULL));
    }

    

    /********************
    *Randomize letters to length of string
    *Output string
    ********************/

    while (i < keyLength){
        
        num = rand() % 27;
        key[i] = findLetter(num);
        i++;
    }

    strcat(key, "\n");

    printf("%s", key);


    return 0;

}
