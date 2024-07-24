#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to simulate all possible exit statuses and signals
void perform_exit_status() {
    int choice;
    printf("Choose an exit status to simulate:\n");
    printf("1. Normal termination (exit 0)\n");
    printf("2. Custom exit status (exit 1-255)\n");
    printf("3. Abort signal (SIGABRT)\n");
    printf("4. Floating-point exception (SIGFPE)\n");
    printf("5. Illegal instruction (SIGILL)\n");
    printf("6. Interrupt from keyboard (SIGINT)\n");
    printf("7. Segmentation fault (SIGSEGV)\n");
    printf("8. Termination signal (SIGTERM)\n");
    printf("9. Hangup signal (SIGHUP)\n");
    printf("10. Quit signal (SIGQUIT)\n");
    printf("11. Kill signal (SIGKILL)\n");
    printf("12. Broken pipe signal (SIGPIPE)\n");
    printf("13. Timer signal (SIGALRM)\n");
    printf("14. User-defined signal 1 (SIGUSR1)\n");
    printf("15. User-defined signal 2 (SIGUSR2)\n");
    printf("16. Stop signal (SIGSTOP)\n");
    printf("17. Stop typed at terminal (SIGTSTP)\n");
    printf("18. Terminal input for background process (SIGTTIN)\n");
    printf("19. Terminal output for background process (SIGTTOU)\n");
    printf("20. Continue if stopped (SIGCONT)\n");
    printf("Enter your choice (1-20): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Simulating normal termination.\n");
            exit(0);
            break;
        case 2:
            {
                int custom_exit_status;
                printf("Enter custom exit status (1-255): ");
                scanf("%d", &custom_exit_status);
                printf("Simulating custom exit status %d.\n", custom_exit_status);
                exit(custom_exit_status);
            }
            break;
        case 3:
            printf("Simulating abort signal (SIGABRT).\n");
            abort();
            break;
        case 4:
            printf("Simulating floating-point exception (SIGFPE).\n");
            int x = 1 / 0; // Cause floating-point exception
            break;
        case 5:
            printf("Simulating illegal instruction (SIGILL).\n");
            raise(SIGILL); // No direct way to generate SIGILL in a portable way
            break;
        case 6:
            printf("Simulating interrupt from keyboard (SIGINT).\n");
            raise(SIGINT); // Needs manual interrupt (Ctrl+C)
            break;
        case 7:
            printf("Simulating segmentation fault (SIGSEGV).\n");
            int *ptr = NULL;
            *ptr = 42; // Cause segmentation fault
            break;
        case 8:
            printf("Simulating termination signal (SIGTERM).\n");
            raise(SIGTERM); // Needs external process to send SIGTERM
            break;
        case 9:
            printf("Simulating hangup signal (SIGHUP).\n");
            raise(SIGHUP); // Needs controlling terminal to send SIGHUP
            break;
        case 10:
            printf("Simulating quit signal (SIGQUIT).\n");
            raise(SIGQUIT); // Needs manual quit (Ctrl+\)
            break;
        case 11:
            printf("Simulating kill signal (SIGKILL).\n");
            raise(SIGKILL); // Needs external process to send SIGKILL
            break;
        case 12:
            printf("Simulating broken pipe signal (SIGPIPE).\n");
            FILE *pipe = popen("cat", "w");
            fclose(pipe); // Close pipe to cause broken pipe error
            fprintf(pipe, "data"); // Cause broken pipe signal
            break;
        case 13:
            printf("Simulating timer signal (SIGALRM).\n");
            alarm(1); // Set an alarm to go off in 1 second
            pause(); // Wait for the alarm signal
            break;
        case 14:
            printf("Simulating user-defined signal 1 (SIGUSR1).\n");
            raise(SIGUSR1); // Needs external process to send SIGUSR1
            break;
        case 15:
            printf("Simulating user-defined signal 2 (SIGUSR2).\n");
            raise(SIGUSR2); // Needs external process to send SIGUSR2
            break;
        case 16:
            printf("Simulating stop signal (SIGSTOP).\n");
            raise(SIGSTOP); // Needs external process to send SIGSTOP
            break;
        case 17:
            printf("Simulating stop typed at terminal (SIGTSTP).\n");
            raise(SIGTSTP); // Needs manual stop (Ctrl+Z)
            break;
        case 18:
            printf("Simulating terminal input for background process (SIGTTIN).\n");
            raise(SIGTTIN); // Needs external process to send SIGTTIN
            break;
        case 19:
            printf("Simulating terminal output for background process (SIGTTOU).\n");
            raise(SIGTTOU); // Needs external process to send SIGTTOU
            break;
        case 20:
            printf("Simulating continue if stopped (SIGCONT).\n");
            raise(SIGCONT); // Needs external process to send SIGCONT
            break;
        default:
            printf("Invalid choice. Exiting with status 1.\n");
            exit(1);
            break;
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child process running with PID %d\n", getpid());
        sleep(2);
        perform_exit_status();
    } else {
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Child process %d terminated normally with exit status %d\n", child_pid, exit_status);
            if (exit_status == 0) {
                printf("Exit status %d shows that the process has been completed without any errors.\n", exit_status);
            } else {
                printf("Exit status %d indicates an error occurred.\n", exit_status);
            }
            return exit_status;
        } else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            printf("Child process %d terminated abnormally due to signal %d\n", child_pid, signal_number);

            switch (signal_number) {
                case SIGHUP:
                    printf("SIGHUP: Hangup detected on controlling terminal or death of controlling process.\n");
                    break;
                case SIGINT:
                    printf("SIGINT: Interrupt from keyboard (typically Ctrl+C).\n");
                    break;
                case SIGQUIT:
                    printf("SIGQUIT: Quit from keyboard.\n");
                    break;
                case SIGILL:
                    printf("SIGILL: Illegal Instruction.\n");
                    break;
                case SIGABRT:
                    printf("SIGABRT: Abort signal from abort(3).\n");
                    break;
                case SIGFPE:
                    printf("SIGFPE: Floating-point exception.\n");
                    break;
                case SIGKILL:
                    printf("SIGKILL: Kill signal.\n");
                    break;
                case SIGSEGV:
                    printf("SIGSEGV: Invalid memory reference (segmentation fault).\n");
                    break;
                case SIGPIPE:
                    printf("SIGPIPE: Broken pipe: write to pipe with no readers.\n");
                    break;
                case SIGALRM:
                    printf("SIGALRM: Timer signal from alarm(2).\n");
                    break;
                case SIGTERM:
                    printf("SIGTERM: Termination signal.\n");
                    break;
                case SIGUSR1:
                    printf("SIGUSR1: User-defined signal 1.\n");
                    break;
                case SIGUSR2:
                    printf("SIGUSR2: User-defined signal 2.\n");
                    break;
                case SIGCHLD:
                    printf("SIGCHLD: Child stopped or terminated.\n");
                    break;
                case SIGCONT:
                    printf("SIGCONT: Continue if stopped.\n");
                    break;
                case SIGSTOP:
                    printf("SIGSTOP: Stop process.\n");
                    break;
                case SIGTSTP:
                    printf("SIGTSTP: Stop typed at terminal.\n");
                    break;
                case SIGTTIN:
                    printf("SIGTTIN: Terminal input for background process.\n");
                    break;
                case SIGTTOU:
                    printf("SIGTTOU: Terminal output for background process.\n");
                    break;
                default:
                    printf("Unknown signal %d.\n", signal_number);
                    break;
            }
        } else {
            printf("Child process %d terminated abnormally\n", child_pid);
        }
    }

    return 0;
}

