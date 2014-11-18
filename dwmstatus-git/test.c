#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void error(const char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, const char *argv[])
{
    unsigned char buffer[10];
    FILE *f;
    int capacity;

    if (!(f = fopen("/sys/class/power_supply/BAT1/capacity", "rb")))
        error("Could not open battery capcity file");

    while (1){

        fscanf(f, "%d", &capacity);

        printf("Current battery level: %d\n", capacity);

        sleep(5);
    }
    fclose(f);
}
