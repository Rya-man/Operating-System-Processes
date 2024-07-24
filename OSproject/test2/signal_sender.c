#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    pid_t pid;
    FILE *fp = fopen("/tmp/child_pid.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fscanf(fp, "%d", &pid);
    fclose(fp);
    
    int signal_number;
    FILE *status_fp = fopen("/tmp/signal_choice.txt", "r");
    if (status_fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fscanf(status_fp, "%d", &signal_number);
    fclose(status_fp);

    switch (signal_number) {
        case 6:
            kill(pid, SIGINT);
            break;
        case 8:
            kill(pid, SIGTERM);
            break;
        case 9:
            kill(pid, SIGHUP);
            break;
        case 10:
            kill(pid, SIGQUIT);
            break;
        case 11:
            kill(pid, SIGKILL);
            break;
        case 14:
            kill(pid, SIGUSR1);
            break;
        case 15:
            kill(pid, SIGUSR2);
            break;
        case 16:
            kill(pid, SIGSTOP);
            break;
        case 17:
            kill(pid, SIGTSTP);
            break;
        case 18:
            kill(pid, SIGTTIN);
            break;
        case 19:
            kill(pid, SIGTTOU);
            break;
        case 20:
            kill(pid, SIGCONT);
            break;
        default:
            break;
    }

    return 0;
}

