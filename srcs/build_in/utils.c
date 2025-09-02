/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:34:04 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 11:50:50 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_arg	*get_arg_built_in(t_env *minishell, t_tree *node)
{
	t_arg	*arg;

	if (!minishell || !node)
		return (NULL);
	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->minishell = minishell;
	arg->node = node;
	return (arg);
}

int	give_file_name_size(char *buf)
{
	int	i[2];

	if (!buf)
		return (0);
	i[0] = 0;
	i[1] = 0;
	while (buf[i[0]] && i[0] < 999 && i[1] < 29)
	{
		if (ft_isalnum(buf[i[0]]))
			i[1]++;
		i[0]++;
	}
	return (i[1]);
}
