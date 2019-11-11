#include <stdio.h>
int main(int argc, char *argv[]){
    // Test program to be the payload in a UPX ELF, this will tell if my technique works
	char BUFFER[230];
	printf("Please enter input: ");
	gets(BUFFER);
	printf("You Entered: %s\n", BUFFER);
    
}
