/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:27:12 by jhogonca          #+#    #+#             */
/*   Updated: 2023/12/22 12:59:07 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void error(char *str) {
	write(2, str, strlen(str));
}

void cd(char **av, int i) 
{
	if (i != 2)
		return error("error: cd: bad arguments\n");
	if (chdir(av[1]) == -1)
		return error("error: cd: cannot change directory to "), error(av[1]), error("\n");
}

void exec(char **av, char **envp, int i) 
{
	int fd[2];
	int has_pipe = av[i] && !strcmp(av[i], "|");

	if (has_pipe && pipe(fd) == -1)
		return error("error: fatal\n");

	if (fork() == 0)
	{
		av[i] = 0;
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return error("error: fatal\n");
		execve(*av, av, envp);
		return error("error: cannot execute "), error(*av), error("\n");
	}
	waitpid(-1, 0, 0);
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return error("error: fatal\n");
}

int main(int ac, char **av, char **envp) 
{
	if (ac < 2)
		return 0;
	int i = 0;

	while (av[i] && av[++i]) 
	{
		av += i;
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (!strcmp(*av, "cd"))
			cd(av, i);
		else if (i)
			exec(av, envp, i);
	}
	return 0;
}
