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

void execute_command(char *command)
{
pid_t pid; 
char *args[2];
args[0] = command;
args[1] = NULL;
if (access(command, X_OK) != 0)
{
printf("Error: command %s is not found or is not executable\n", command);
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
if (execve(command, args, NULL) == -1)
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
if (line[read - 1] == '\n')
{
line[read - 1] = '\0';
}
if (strlen(line) == 0)
{
continue;
}
execute_command(line);
}
free(line);
printf("\n");
return 0;
}
