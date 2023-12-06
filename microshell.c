#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void err(char *str) 
{
    while (*str)
        write(2, str++, 1);
}

void cd(char **argv, int i) 
{
    if (i != 2)
        return err("error: cd: bad arguments\n");
    if (chdir(argv[1]) == -1)
        return err("error: cd: cannot change directory to "), err(argv[1]), err("\n");
}

void exec(char **argv, char **envp, int i) 
{
    int fd[2];
    int has_pipe = argv[i] && !strcmp(argv[i], "|");

    if (has_pipe && pipe(fd) == -1)
        return err("error: fatal\n");
    
    int pid = fork();
    if (!pid) 
    {
        argv[i] = 0;
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return err("error: fatal\n");
        execve(*argv, argv, envp);
        return err("error: cannot execute "), err(*argv), err("\n");
    }
    waitpid(-1, 0, 0);
    if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        return err("error: fatal\n");
}

int main(int argc, char **argv, char **envp) 
{
    int    i = 0;

    if (argc > 1) 
    {
        while (argv[i] && argv[++i]) 
        {
            argv += i;
            i = 0;
            while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
                i++;
            if (!strcmp(*argv, "cd"))
                cd(argv, i);
            else if (i)
                exec(argv, envp, i);
        }
    }
    return 0;
}