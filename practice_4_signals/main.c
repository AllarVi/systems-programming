#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static const int SIGNUM_CTRL_C = 2;

static int countCtrlC = 0;

// Blocks all other signals anyway
void signalHandler(int signum) {
    if (signum == SIGNUM_CTRL_C) {
        printf("\nctrl C pressed %d times \n", ++countCtrlC);
    }
}

int main() {

    signal(SIGINT, signalHandler);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    while (1) {
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == 0) {
            printf(".\n");
        }
        sigprocmask(SIG_UNBLOCK, &mask, NULL);

        usleep(500000);
    }

    return 0;
}
