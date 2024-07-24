#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    const char *fifo_name = "/tmp/my_fifo";
    mkfifo(fifo_name, 0666);
//	printf("--------------Hello and welcome to the writer process:---------------\n first we want you to set a limit of the maximum characters in our message:\t");

	const char *message = "Hello from writer process!";
	/*	int n;
	scanf("%d",&n);
    char *message = (char*)malloc(n);
    printf("Enter message to the pipe that is a FIFO file \n");
    if (message == NULL) {
        perror("Memory not allocated.\n");
        exit(0);
    }*/
    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    write(fd, message, strlen(message) + 1);
    close(fd);

    printf("Message sent: %s\n", message);
    return 0;
}

