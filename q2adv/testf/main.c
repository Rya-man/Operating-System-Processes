#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

void sigpipe_handler(int signum) {
    printf("Caught SIGPIPE signal.\n");
    exit(1); 
}


void perform_exit_status(pid_t child_pid,bool *a) {
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
    
    FILE *fp = fopen("/tmp/signal_choice.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\n", choice);
    fclose(fp);

    switch (choice) {
        case 1:
            {printf("Simulating normal termination.\n");
            exit(0);
	        *a=false;
            break;}
        case 2:
            {
                int custom_exit_status;
                printf("Enter custom exit status (1-255): ");
                scanf("%d", &custom_exit_status);
                printf("Simulating custom exit status %d.\n", custom_exit_status);
                exit(custom_exit_status);
	        *a=false;
            break;
            }
        case 3:
            {printf("Simulating abort signal (SIGABRT).\n");
            abort();
	        *a=false;
            break;}
        case 4:
            {printf("Simulating floating-point exception (SIGFPE).\n");
            int x = 1 / 0; // Cause floating-point exception
            *a=false;
	        break;}
        case 5:
            {printf("Simulating illegal instruction (SIGILL).\n");
            __asm__("ud2"); // Execute an undefined instruction to cause SIGILL
	        *a=false;
	        break;}
        case 6:
            {printf("Simulating interrupt from keyboard (SIGINT).\n");
            break;} // Signal will be sent by signal_sender.c
        case 7:
            {printf("Simulating segmentation fault (SIGSEGV).\n");
            int *ptr = NULL;
            *ptr = 42; // Cause segmentation fault
	        *a=false;
            break;}
        case 8:
            {printf("Simulating termination signal (SIGTERM).\n");
            break;} // Signal will be sent by signal_sender.c
        case 9:
            {printf("Simulating hangup signal (SIGHUP).\n");
            break;} // Signal will be sent by signal_sender.c
        case 10:
            {printf("Simulating quit signal (SIGQUIT).\n");
            break;} // Signal will be sent by signal_sender.c
        case 11:
            {printf("Simulating kill signal (SIGKILL).\n");
            break;} // Signal will be sent by signal_sender.c
        case 12:
            {
		signal(SIGPIPE, sigpipe_handler);
    		printf("Simulating broken pipe signal (SIGPIPE).\n");
		FILE *pie = popen("cat", "w");
    		if (pie == NULL) {
        		perror("popen failed");
        		exit(1);
    		}
		pclose(pie); 
    		if (fprintf(pie, "data") < 0) {
        		perror("fprintf failed");
    		}
    		fflush(pie);
	        *a=false;
	        break;}
        case 13:
            {printf("Simulating timer signal (SIGALRM).\n");
            alarm(1); // Set an alarm to go off in 1 second
            pause(); // Wait for the alarm signal
	        *a=false;}
            break;
        case 14:
            {printf("Simulating user-defined signal 1 (SIGUSR1).\n");
            break;} // Signal will be sent by signal_sender.c
        case 15:
            {printf("Simulating user-defined signal 2 (SIGUSR2).\n");
            break; }// Signal will be sent by signal_sender.c
        case 16:
            {printf("Simulating stop signal (SIGSTOP).\n");
            break; }// Signal will be sent by signal_sender.c
        case 17:
            {printf("Simulating stop typed at terminal (SIGTSTP).\n");
            break; }// Signal will be sent by signal_sender.c
        case 18:
            {printf("Simulating terminal input for background process (SIGTTIN).\n");
            break; }// Signal will be sent by signal_sender.c
        case 19:
            {printf("Simulating terminal output for background process (SIGTTOU).\n");
            break; }// Signal will be sent by signal_sender.c
        case 20:
            {printf("Simulating continue if stopped (SIGCONT).\n");
            break; }// Signal will be sent by signal_sender.c
        default:
            {printf("Invalid choice. Exiting with status 1.\n");
            exit(1);
            break;}
    }
}

int main() {
    pid_t pid = fork();
    bool a = true;
    bool* ap = &a;
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child process running with PID %d\n", getpid());
        
        FILE *fp = fopen("/tmp/child_pid.txt", "w");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "%d\n", getpid());
        fclose(fp);
        perform_exit_status(pid,ap);
        while (a) {
            pause(); // Wait for signals
        }
    } else {
        
        // perform_exit_status(ap);
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

