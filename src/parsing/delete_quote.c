/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:08:14 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/30 09:31:57 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	flag_is_in_quote(t_token *token)
{
	int		len;
	char	*str;

	if (!token && !token->data)
		return ;
	str = token->data;
	len = ft_strlen(str);
	if (str[0] == '"' && str[len -1] == '"')
		token->is_in_quote = 1;
	else if (str[0] == '\'' && str[len - 1] == '\'')
		token->is_in_quote = 1;
	else
		token->is_in_quote = 0;
}

/*************************************************
* logique pour supprimer les quotes d'une liste  * 
**************************************************/
t_list	*remove_list_quotes(t_list *token_list)
{
	t_list	*current;
	t_token	*token;
	char	*tmp;

	if (token_list == NULL)
		return (token_list);
	current = token_list;
	while (current)
	{
		token = (t_token *)current->data;
		flag_is_in_quote(token);
		tmp = remove_str_quotes(token->data);
		if (ft_strncmp(tmp, token->data, ft_strlen(tmp)) \
			!= 0 && ft_strlen(tmp) != ft_strlen(token->data))
		{
			free(token->data);
			token->data = tmp;
		}
		else
			free (tmp);
		current = current->next;
	}
	return (token_list);
}

/*************************************************
* logique pour supprimer les quotes d'une string * 
**************************************************/
char	*remove_str_quotes(char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*new_str;

	i = 0;
	j = 0;
	quote = 0;
	new_str = str;
	while (str && str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote && (str[i] == quote))
			quote = 0;
		else
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (ft_strdup(new_str));
}
