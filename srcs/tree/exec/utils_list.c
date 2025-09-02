/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:29:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 22:29:41 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_r_in	*get_last_redir(t_tree *node)
{
	t_list	*lst;

	if (!node || !node->redir_in)
		return (NULL);
	lst = node->redir_in;
	while (lst->next)
		lst = lst->next;
	return (lst->data);
}
