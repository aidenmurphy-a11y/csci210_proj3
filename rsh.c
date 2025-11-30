#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12
#define MAXARGS 21   // 20 args + NULL

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char* cmd) {

    for (int i = 0; i < N; i++) {
        if (strcmp(cmd, allowed[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int main() {

    char line[256];

    while (1) {

        fprintf(stderr, "rsh>");

        if (fgets(line, 256, stdin) == NULL) continue;

        if (strcmp(line, "\n") == 0) continue;

        line[strlen(line)-1] = '\0';

        // -----------------------------
        // Build argv
        // -----------------------------
        char *argv[MAXARGS];
        int argc = 0;

        char *tok = strtok(line, " ");

        while (tok != NULL && argc < MAXARGS - 1) {
            argv[argc++] = tok;
            tok = strtok(NULL, " ");
        }

        argv[argc] = NULL;

        if (argc == 0) continue;

        char *cmd = argv[0];

        // -----------------------------
        // Check allowed
        // -----------------------------
        if (!isAllowed(cmd)) {
            printf("NOT ALLOWED!\n");
            continue;
        }

        // -----------------------------
        // exit
        // -----------------------------
        if (strcmp(cmd, "exit") == 0) {
            return 0;
        }

        // -----------------------------
        // help
        // -----------------------------
        if (strcmp(cmd, "help") == 0) {
            printf("The allowed commands are:\n");
            for (int i = 0; i < N; i++) {
                printf("%d: %s\n", i+1, allowed[i]);
            }
            continue;
        }

        // -----------------------------
        // cd
        // -----------------------------
        if (strcmp(cmd, "cd") == 0) {

            if (argc > 2) {
                printf("-rsh: cd: too many arguments\n");
                continue;
            }

            char *targ = (argc == 1) ? getenv("HOME") : argv[1];
            chdir(targ);
            continue;
        }

        // -----------------------------
        // spawn
        // -----------------------------
        pid_t pid;
        int stat;

        if (posix_spawnp(&pid, cmd, NULL, NULL, argv, environ) != 0) {
            printf("NOT ALLOWED!\n");
            continue;
        }

        waitpid(pid, &stat, 0);
    }

    return 0;
}
