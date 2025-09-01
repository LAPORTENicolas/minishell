/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:19:56 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/13 18:27:20 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// creer un tableau de str depuis une liste
char	**get_split_from_list(t_list *lst)
{
	char	**split;
	int		i;

	if (!lst)
		return (NULL);
	split = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!split)
		return (NULL);
	i = 0;
	while (lst)
	{
		split[i] = ft_strndup((char *)lst->data, ft_strlen((char *)lst->data));
		if (!split[i])
		{
			split[i] = 0;
			free_split(split);
			return (NULL);
		}
		lst = lst->next;
		i++;
	}
	split[i] = 0;
	return (split);
}

int	print_error_pipeline(char *s, char *err, int code)
{
	ft_putstr_fd(s, 2);
	ft_putendl_fd(err, 2);
	return (code);
}

int	use_redir_in_here_doc2(t_tree *node, pid_t fork_pid, char *path_p)
{
	int	code;

	if (!node)
		return (-1);
	if (fork_pid == 0)
	{
		close(node->here_doc_fd[0]);
		code = write(node->here_doc_fd[1], path_p, 1000);
		while (code == 1000)
		{
			path_p += 1000;
			code = write(node->here_doc_fd[1], path_p, 1000);
		}
		if (code < 0)
			return (print_error_pipeline("can't write: %s", \
				strerror(errno), -4));
		close(node->here_doc_fd[1]);
		exit(0);
	}
	return (0);
}
