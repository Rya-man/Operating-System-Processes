#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	const char *fifo_name = "/tmp/FIFO";
        // Create the named pipe (FIFO)
    
	if (mkfifo(fifo_name, 0666) == -1) {
		perror("mkfifo");
	        exit(EXIT_FAILURE);
    	}

    	printf("--------------Hello and welcome to the writer process:---------------\n");
	printf("\nFirst, we want you to set a limit on the maximum characters in your message:\t");

	int n;
    	scanf("%d", &n);
    	getchar(); // Consume the newline character left by scanf

	// Allocate memory for the message
	char *message = (char *)malloc(n + 1); // +1 for the null terminator
    	if (message == NULL) {
        	perror("Memory not allocated\n");
	        exit(EXIT_FAILURE);
	}
	printf("Enter the message to the pipe (FIFO file): \n\n");
    	fgets(message, n + 1, stdin); // Use fgets to read the message
	
	printf("Your message has been saved and will be sent once the receiver process begins its execution.\n");
	// Open the named pipe for writing
	 int fd = open(fifo_name, O_WRONLY);
     	 if (fd == -1) {
		 perror("open");
	 	 free(message);
	 	 exit(EXIT_FAILURE);
     	 }

     	 // Write the message to the named pipe
	  write(fd, message, strlen(message) + 1);
      	  close(fd);

      	  printf("Message sent: %s\n", message);

      	  // Free the allocated memory
	  free(message);

	  //delete the FIFO_file
      	  if (unlink(fifo_name) == -1) {
	  	  perror("unlink");
	  	  exit(EXIT_FAILURE);
      	  }

      	  return 0;
}

