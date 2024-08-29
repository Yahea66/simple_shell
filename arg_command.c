#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
 * main - simple shell
 *
 * Return: int
 */

void execute_ocommand(char *line) 
{
pid_t pid;
char *args[10];
int count = 0;
char *cmd;
char *token = strtok(line, " \t\n");
if (token == NULL) 
{
return;
}
cmd = token;
args[count++] = cmd;
while ((token = strtok(NULL, " \t\n")) != NULL && count < 9) 
{
args[count++] = token;
}
args[count] = NULL;

if (access(cmd, X_OK) != 0) 
{
printf("Error: command %s is not found or is not executable\n", cmd);
return;
}

pid = fork();
if (pid == -1) 
{
perror("fork failed");
exit(EXIT_FAILURE);
} 
else if (pid == 0) 
{
if (execve(cmd, args, NULL) == -1) 
{
perror("execve failed");
exit(EXIT_FAILURE);
}
} 
else 
{
wait(NULL);
}
}
int main() 
{
char *line = NULL;
size_t len = 0;
ssize_t read;
while (1) 
{
printf("> ");
fflush(stdout);
read = getline(&line, &len, stdin);
if (read == -1) 
{
if (feof(stdin)) 
{
printf("EOF encountered.\n");
} 
else 
{
perror("can't read command");
}
break;
}
execute_ocommand(line);
}
free(line);
printf("\n");
return (0);
}
