#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>

#define MAX_INPUT_LENGTH (65536)

sigjmp_buf jump;

void signal_redirect(void *signal_handler) {
    struct sigaction signal_action;
    signal_action.sa_handler = signal_handler;
    sigemptyset(&signal_action.sa_mask);
    sigaction(SIGINT, &signal_action, NULL);
    sigaction(SIGTSTP, &signal_action, NULL);
}

void signal_handler(int signal) {
    printf("\n");
    siglongjmp(jump, 1);
}

void execute_command(char *input) {
    printf("%s", input);
}

int run_shell() {
    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s> ", cwd);
        char input[MAX_INPUT_LENGTH];
        if (sigsetjmp(jump, 1) != 0) continue;
        fgets(input, MAX_INPUT_LENGTH, stdin);
        if(strcasecmp(input, "\n") == 0) continue;
        // command execution logic goes here!
        execute_command(input);
        if (strcasecmp(input, "exit\n") == 0) return 0;
    }
}

int main() {
    signal_redirect(signal_handler);
    return run_shell();
}
