/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:28:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 15:49:12 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"
#include <signal.h>

volatile sig_atomic_t	g_exit_code;

static void	init_main(t_env *minishell)
{
	minishell->exec.is_pipeline = 0;
	minishell->exec.fd[0] = -1;
	minishell->exec.fd[1] = -1;
	minishell->exec.fd2[0] = -1;
	minishell->exec.fd2[1] = -1;
	minishell->stdin_cpy = -1;
}

int	main(int ac, char **av, char **env)
{
	t_env			minishell;
	t_meta			meta;
	struct termios	test;
	t_list			*varlist;

	(void)ac;
	(void)av;
	(void)varlist;
	signal(SIGPIPE, SIG_IGN);
	ft_memset(&minishell, 0, sizeof(t_env));
	if (env)
		minishell.env = env;
	varlist = NULL;
	init_env(&minishell);
	init_sig(&minishell.sig);
	init_main(&minishell);
	ft_memset(&test, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &test);
	minishell.term_cpy = &test;
	minishell.meta = &meta;
	read_stdin(&minishell, &meta, NULL, NULL);
	return (0);
}
