/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:54:53 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 14:54:54 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

void	check_exec_cmd(t_env *minishell, t_tree *node)
{
	t_r_in	*r_in;

	r_in = get_last_redir(node);
	if (!minishell || !node || ((!node->path || !node->content) \
	&& !node->built_in))
	{
		if (r_in && r_in->fd)
			close(r_in->fd);
		(exit_minishell(minishell), exit(127));
	}
	if (r_in && r_in->fd == -1)
	{
		exit_minishell(minishell);
		exit(130);
	}
	if (check_cmd(node->content) == -1 \
	|| is_a_directory(node->content))
	{
		print_error_d(node->content);
		(exit_minishell(minishell), exit(127));
	}
}

int	is_a_directory(char *cmd)
{
	struct stat	is_path;

	if (!cmd)
		return (0);
	if (stat(cmd, &is_path) == -1)
		return (0);
	return (is_path.st_mode & S_IFDIR);
}

int	check_cmd(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd || ft_strlen(cmd) <= 0)
		return (-1);
	if (*cmd == '.')
		i++;
	if (cmd[i] == '/' && ft_isalnum(cmd[ft_strlen(cmd) - 1]))
		return (0);
	while (cmd[i])
		if (!ft_isalnum(cmd[i++]))
			return (-1);
	return (0);
}

void	print_error_d(char *s)
{
	if (!s)
		return ;
	if (access(s, X_OK) != 0)
		return ;
	ft_putstr_fd("minishell: ", 1);
	ft_putstr_fd(s, 1);
	ft_putendl_fd(": is a directory", 1);
}
