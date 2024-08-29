#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char **environ;

/**
 * main - simple shell
 *
 * Return: int
 */

int cmd_exist(char* cmd)
{
char *path, *path_copy, *tmp;
char full_path[1024];
path = getenv("PATH");
if (path == NULL)
{
return(-1);
}
path_copy = strdup(path);
if (path_copy == NULL)
{
return (-1);
}
tmp = strtok(path_copy, ":");
while (tmp != NULL)
{
snprintf(full_path, sizeof(full_path), "%s/%s", tmp, cmd);
if (access(full_path, X_OK) == 0)
{
free(path_copy);
return (0);
}
tmp = strtok(NULL, ":");
}
free(path_copy);
return (-1);
}

void execute_ncommand(char *line)
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
if (cmd_exist(cmd) != 0)
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
if (execve(cmd, args, environ) == -1)
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
execute_ncommand(line);
}
free(line);
printf("\n");
return (0);
}
