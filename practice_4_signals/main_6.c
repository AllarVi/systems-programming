#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static const int SIGNUM_CTRL_C = 2;

// It tells the compiler that the value of the variable may change at any time
// without any action being taken by the code the compiler finds nearby.
static volatile sig_atomic_t gotSignal = 0;

static int countCtrlC = 0;

void signalHandler(int signum) {
    gotSignal++;
}

int main() {

    signal(SIGINT, signalHandler);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    while (1) {
        if (gotSignal) {
            if (sigprocmask(SIG_BLOCK, &mask, NULL) == 0) {
                gotSignal--;
            }
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
            printf("\nctrl C pressed %d times \n", ++countCtrlC);
        }
        printf(".\n");

        usleep(500000);
    }

    return 0;
}
