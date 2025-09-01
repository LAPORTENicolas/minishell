/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:40:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/30 12:59:21 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

char	**exec_part1(t_env *minishell, t_tree *node)
{
	char	**tmp_arg;
	t_r_in	*r_in;

	tmp_arg = NULL;
	if (!minishell || !node || \
		((!node->path || !node->content) && !node->built_in))
	{
		r_in = get_last_redir(node);
		if (r_in && r_in->fd)
			close(r_in->fd);
		(exit_minishell(minishell), exit(127));
	}
	if (check_cmd(node->content) == -1 \
	|| is_a_directory(node->content))
		(print_error_d(node->content), exit_minishell(minishell), exit(126));
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, sig_ctrl_c_catcher2);
	if (!node->built_in)
		(ft_lstadd_front(&node->arg, ft_create_node(ft_strndup(node->content, \
			ft_strlen(node->content)))), \
		tmp_arg = get_split_from_list(node->arg));
	return (tmp_arg);
}

void	close_r_in(t_tree *tree)
{
	t_list	*lst_r;
	t_r_in	*redir;

	if (!tree)
		return ;
	if (tree->left)
		close_r_in(tree->left);
	if (tree->right)
		close_r_in(tree->right);
	lst_r = tree->redir_in;
	while (lst_r)
	{
		redir = lst_r->data;
		if (redir && redir->type == 2)
		{
			close(redir->fdi);
			close(redir->fd);
		}
		lst_r = lst_r->next;
	}
}

char	*get_env_str(t_var *var)
{
	char	*str;
	int		len[3];

	if (!var || !var->val)
		return (NULL);
	len[0] = ft_strlen(var->key);
	len[1] = ft_strlen(var->val);
	len[2] = len[0] + len[1];
	if (len[2] <= 0)
		return (NULL);
	str = malloc(sizeof(char) * (len[2] + 2));
	if (!str)
		return (NULL);
	ft_memset(str, 0, len[2] + 2);
	ft_strlcat(str, var->key, len[0] + 1);
	str[len[0]] = '=';
	ft_strlcat(&str[len[0] + 1], var->val, len[1] + 1);
	return (str);
}

char	**get_env(t_list *env_lst, int lst_size)
{
	char	**env;
	char	*tmp;
	int		i;

	if (!env_lst)
		return (NULL);
	lst_size = ft_lstsize(env_lst);
	if (lst_size <= 0)
		return (NULL);
	env = malloc(sizeof(char *) * (lst_size + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (env_lst && i < lst_size)
	{
		tmp = get_env_str((t_var *)env_lst->data);
		if (tmp)
			env[i++] = tmp;
		env_lst = env_lst->next;
	}
	env[lst_size] = NULL;
	return (env);
}

void	exec_cmd(t_env *minishell, t_tree *node, int code)
{
	char	**tmp_arg;
	void	*arg;
	t_r_in	*r_in;

	tmp_arg = exec_part1(minishell, node);
	if (node->redir_out)
		if (use_redir_out(node) != 0)
			(exit_minishell(minishell), free_split(tmp_arg), exit(1));
	r_in = get_last_redir(node);
	if (r_in)
		use_redir_in(node, r_in);
	close_r_in(minishell->tree);
	if (node->built_in)
	{
		arg = get_arg_built_in(minishell, node);
		code = (*node->built_in)(arg);
		exit(code);
	}
	else
		close(minishell->stdin_cpy);
	if (minishell->env_list)
		execve(node->path, tmp_arg, get_env(minishell->env_list, 0));
	else
		execve(node->path, tmp_arg, NULL);
	(exit_minishell(minishell), free_split(tmp_arg), exit(0));
}

static int	manage_fork(t_env *minishell, t_tree *node)
{
	pid_t	pid;
	t_r_in	*last_r;

	pid = fork();
	if (pid < 0)
		return (-2);
	last_r = get_last_redir(node);
	if (pid == 0)
		exec_cmd(minishell, node, 0);
	if (last_r && last_r->fd > 0)
		close(last_r->fd);
	node->pid = pid;
	return (0);
}

// Execute une commande simple
int	exec_one_cmd(t_env *minishell, t_tree *node)
{
	t_arg	*arg;
	t_r_in	*r_in;

	if (!node || !node->content)
	{
		r_in = get_last_redir(node);
		if (r_in && r_in->fd > 0)
			close(r_in->fd);
		return (127);
	}
	if (node->need_parent && node->built_in)
	{
		arg = get_arg_built_in(minishell, node);
		if (arg)
			return ((*node->built_in)(arg));
		else
			return ((*node->built_in)(NULL));
	}
	return (manage_fork(minishell, node));
}
