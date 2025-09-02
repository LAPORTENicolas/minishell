/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:53:25 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 14:53:26 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	check_out_1_2(t_list *lst, int fd, int not_append)
{
	(void)fd;
	if (!lst)
		return (-1);
	if (((t_token *)lst->data)->data[0] == '>'
		|| ((t_token *)lst->data)->data[0] == '<')
	{
		ft_putstr_fd(ERROR_TOKEN2, 1);
		ft_putstr_fd(((t_token *)lst->data)->data, 1);
		ft_putendl_fd("'", 1);
		return (-1);
	}
	if (!is_an_redir(((t_token *)lst->data)->data[0]))
	{
		if (open_redir(((t_token *)lst->data)->data, not_append) == -1)
			return (-1);
		return (0);
	}
	return (-1);
}

// Verifie les redirections exterieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il affiche une erreur
// si la case du tableau est OK il l'ouvre en creant le fichier
// si il open echou il return une erreur sinon il ferme fd et quit
int	check_out_1(t_list *lst, int not_append)
{
	int	fd;

	if (!lst)
		return (-1);
	fd = -1;
	if (lst->next)
	{
		lst = lst->next;
		if (check_out_1_2(lst, fd, not_append) == 0)
			return (0);
	}
	else
	{
		ft_putendl_fd(ERROR_NEWLINE2, 1);
		return (2);
	}
	return (1);
}

// Verifie les redirections interieur
// Verifie sil y a un operateur dans la prochaine
// case du tableau, si elle nest pas valide il affiche une erreur
// si la case du tableau est une redirection il passe au prochain
// si la case du tableau est OK il se stop
// si il ne trouve pas de "token" valide (que des redirection)
// il affiche une erreur
int	check_redir(t_list *l)
{
	int		code;

	if (!l)
		return (-2);
	while (l)
	{
		if (!((t_token *)l->data)->is_in_quote)
		{
			if (((t_token *)l->data)->data && \
				((t_token *)l->data)->data[0] == '<')
			{
				if (check_in_1(l) != 0)
					return (2);
			}
			if (((t_token *)l->data)->data && \
				((t_token *)l->data)->data[0] == '>')
			{
				code = redir_out_append(((t_token *)l->data)->data, l);
				if (code != 0)
					return (2);
			}
		}
		l = l->next;
	}
	return (0);
}
