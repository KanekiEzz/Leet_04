#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void err(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(2, &str[i], 1);
        i++;
    }
}

int cd(char **av, int i)
{
    if (i != 2)
        return err("error: cd: bad arguments\n"), 1;
    if(av[i] && chdir(av[i]) == -1)
        return err("error: cd: cannot change directory to "), err(av[i]), err("\n"), 1;
    return(0);
}

void    setpipe(int has_pipe, int *fd, int end)
{
    if(has_pipe && (dup2(fd[end], end) == -1 || close(fd[1]) == -1 || close(fd[0]) == -1))
        err("error: fatal\n"), exit(1);
}

int exec(char **av, int i, char **env)
{
    int fd[2], has_pipe, status, pid;
    
    has_pipe = av[i] && !strcmp(av[i], "|");
    
    if(!has_pipe &&  !strcmp(*av, "cd"))
        return(cd(av, i));
    if(has_pipe && pipe(fd) == -1)
        err("error: fatal\n"), exit(1);
    if((pid = fork()) == -1)
        err("error: fatal\n"), exit(1);
    if(pid == 0)
    {
        av[i] = 0;
        setpipe(has_pipe, fd, 1);
        if(!strcmp(*av, "cd"))
            exit(cd(av, i));
        execve(*av, av, env);
        err("error: cannot execute "), err(*av), err("\n"), exit(1); 
    }
    waitpid(pid, &status, 0);
    setpipe(has_pipe, fd, 0);
    return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **env)
{
    (void) ac;
    int i = 0, status = 0;

    while(av[i])
    {
        av += i + 1;
        i = 0;
        while(av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if(i)
            status = exec(av, i, env);
    }
    return(status);
}
