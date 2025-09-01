/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 07:10:42 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/30 12:44:32 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <fcntl.h>

static void	print_error(char *s, char *s2)
{
	ft_putstr_fd("bash: warning: here-document at line ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(s2, 2);
	ft_putendl_fd("')", 2);
	free(s);
}

int	heredoc_while(t_env *minishell, int *i, char *s, t_r_in *redir)
{
	char	*tmp;

	if (*i >= INT_MAX - 10)
		*i = 0;
	i++;
	s = readline("> ");
	if (s)
	{
		if (ft_strncmp(s, redir->path, ft_strlen(redir->path)) \
		== 0 && ft_strlen(redir->path) == ft_strlen(s) && (free(s), 1))
			return (0);
		s = fork_heredoc_expand(minishell, minishell->meta, redir, s);
		(write(redir->fd, s, ft_strlen(s)), write(redir->fd, "\n", 1));
	}
	else
	{
		if (g_exit_code == 130)
			return (free(s), 0);
		tmp = ft_itoa(*i);
		if (!tmp)
			return (free(s), 0);
		return (print_error(tmp, redir->path), free(s), 0);
	}
	free(s);
	return (1);
}

void	fork_heredoc_n(t_env *minishell, t_r_in *redir)
{
	char	*s;
	int		i;

	if (!minishell || !redir)
		return ;
	close(redir->fdi);
	redir->fd = open(redir->file, O_WRONLY, 0644);
	s = NULL;
	if (redir->fd < 0)
	{
		close_r_in(minishell->tree);
		exit_minishell(minishell);
		exit(128);
	}
	i = 0;
	signal(SIGINT, sigin_handler_heredoc);
	while (g_exit_code != 130 && heredoc_while(minishell, &i, s, redir))
		i++;
	close(redir->fd);
	close_r_in(minishell->tree);
	exit_minishell(minishell);
	free(minishell->str_readline);
	exit(0);
}

static void	prepare_heredoc_while(t_env *minishell, t_list *lst_r)
{
	t_r_in	*tmp;
	pid_t	proc;
	int		code;

	while (lst_r)
	{
		tmp = lst_r->data;
		if (tmp->type == 2)
		{
			tmp->fd = open(tmp->file, O_CREAT, 0644);
			if (tmp->fd < 0)
				return ;
			close(tmp->fd);
			tmp->fdi = open(tmp->file, O_RDONLY, 0644);
			proc = fork();
			if (proc < 0)
				return ;
			else if (proc == 0)
				fork_heredoc_n(minishell, tmp);
			waitpid(proc, &code, 0);
		}
		lst_r = lst_r->next;
	}
}

void	prepare_heredoc(t_env *minishell, t_tree *tree)
{
	t_list	*lst_r;

	if (!minishell || g_exit_code == 130)
		return ;
	if (tree->left)
		prepare_heredoc(minishell, tree->left);
	if (tree->right)
		prepare_heredoc(minishell, tree->right);
	lst_r = tree->redir_in;
	prepare_heredoc_while(minishell, lst_r);
}
