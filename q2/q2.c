#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child process running with PID %d\n", getpid());
        // Simulate some work in the child process
        sleep(2);
        // Uncomment one of the lines below to test normal or abnormal termination
        exit(0);    // Normal termination
        //abort();  // Abnormal termination (uncomment to test abnormal termination)
    } else {
        // Parent process
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            // Child terminated normally
            int exit_status = WEXITSTATUS(status);
            printf("Child process %d terminated normally with exit status %d\n", child_pid, exit_status);
        } else if (WIFSIGNALED(status)) {
            // Child terminated by a signal
            int signal_number = WTERMSIG(status);
            printf("Child process %d terminated abnormally due to signal %d\n", child_pid, signal_number);
        } else {
            // Other abnormal termination
            printf("Child process %d terminated abnormally\n", child_pid);
        }
    }

    return 0;
}

