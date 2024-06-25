// Assignment name  : union
// Expected files   : union.c
// Allowed functions: write
// --------------------------------------------------------------------------------

// Write a program that takes two strings and displays, without doubles, the
// characters that appear in either one of the strings.

// The display will be in the order characters appear in the command line, and
// will be followed by a \n.

// If the number of arguments is not 2, the program displays \n.

// Example:

// $>./union "zpadinton" "paqefwtdjetyiytjneytjoeyjnejeyj" | cat -e
// zpadintoqefwjy$
// zpadintoqefwjy$
// $>./union ddf6vewg64f gtwthgdwthdwfteewhrtag6h4ffdhsd | cat -e
// df6vewg4thras$
// df6vewg4thras$
// $>./union "rien" "cette phrase ne cache rien" | cat -e
// rienct phas$
// rienct phas$
// $>./union | cat -e
// $
// $>
// $>./union "rien" | cat -e
// $
// $>

#include <unistd.h>

int		ft_already_exists(char *str, int start, char position)
{
	int i;

	i = -1;
	while (str && ++i < start)
		if (str[i] == position)
			return (0);
	return (1);
}

int main(int ac, char **av)
{
	int i;
	int j;

	i = -1;
	j = -1;
	if (ac == 3)
	{
		while (av[1][++i])
			if (ft_already_exists(av[1], i, av[1][i]))
				write(1, &av[1][i], 1);
		while (av[2][++j])
			if (ft_already_exists(av[1], i, av[2][j]))
				if (ft_already_exists(av[2], j, av[2][j]))
					write(1, &av[2][j], 1);
	}
	write(1, "\n", 1);
	return (0);
}
