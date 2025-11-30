#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i = 0; i < N; i++){
		if (strcmp(cmd, allowed[i]) == 0){
			return 1;
		}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

		char *argv[MAXARGS];
		int argc = 0;
		char *tok = strtok(line, " ");
		while (tok != NULL && argc < MAXARGS -1){
			argv[argc++] = token;
		}
		argv[argc] = NULL;
		if (argc == 0){
			continue;
		}
		char *cmd = argv[0];
		if(!isAllowed(cmd)){
			printf("NOT ALLOWED!\n");
			continue;
		}

		if (strcmp(cmd, "exit") == 0){
			return 0;
		}

		if(strcmp(cmd, "help")==0){
			printf("The allowed commands are:\n");
			for (int i = 0; i < N; i++){
				printf("%d: %s\n", i+1, allowed[i]);
			}
			continue;
		}

		if(strcmp(cmd, "cd") == 0){
			if (argc > 2){
				printf("-rsh: cd: too many arguments\n");
				continue;
			}
			char *targ = (argc == 1) ? getenv("HOME") : argv[1];
			chdir(targ);
			continue;
		}
		pid_t pid;
		int stat;
		if (posix_spawnp(&pid, cmd, NULL, NULL, argv, environ) != 0){
			printf("NOT ALLOWED!\n");
			continue;
		}
		wait(pid, &stat, 0);
    }
    return 0;
}
