/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 11:27:06 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 14:52:23 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	handle_special_cases(char *tmp, int *code)
{
	if (ft_strncmp(tmp, "=", 2) == 0)
	{
		print_export_error("=");
		*code = 1;
		return (1);
	}
	if (tmp[0] == 0)
	{
		print_export_error(tmp);
		*code = 1;
		return (1);
	}
	return (0);
}

static int	create_and_add_var(t_arg *arg, char *tmp, char *sep)
{
	t_var	*var;

	if (sep)
	{
		var = create_var(tmp, sep);
		if (!var)
			return (-1);
	}
	else
	{
		var = create_var_no_value(tmp);
		if (!var)
			return (-1);
	}
	safe_add_env_list(arg->minishell->env_list, var);
	return (0);
}

int	export_while(t_arg *arg, t_list	*list_arg, int *code)
{
	char	*sep;
	char	*tmp;
	int		local_code;

	tmp = (char *)list_arg->data;
	local_code = 0;
	if (handle_special_cases(tmp, code))
		return (0);
	sep = export_part1(&local_code, tmp);
	if (local_code == 1)
	{
		print_export_error(tmp);
		*code = 1;
		return (0);
	}
	return (create_and_add_var(arg, tmp, sep));
}
