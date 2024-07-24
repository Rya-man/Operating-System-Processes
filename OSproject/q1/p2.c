// reader.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    const char *fifo_name = "/tmp/my_fifo";
    mkfifo(fifo_name, 0666);

    char buffer[100];
    int fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    read(fd, buffer, sizeof(buffer));
    close(fd);

    printf("Message received: %s\n", buffer);
    return 0;
}

