/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:44:58 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 16:00:32 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*********************************************
*   compte le nombre de char du futur token  *
**********************************************/
//@Calculer la longueur d’un bloc homogène : une suite de caractères qui sont
//	tous soit dans quote, soit tous hors quote
// @param s La chaîne de caractères à analyser
// @param charset La chaîne de caractères définissant le charset
// @return La longueur du bloc homogène
static size_t	handle_redir(char *s, int len)
{
	size_t	j;

	j = 0;
	while (is_in_charset(s[j], "0123456789"))
		j++;
	if (len - j >= 2 && ((s[j] == '>' && s[j + 1] == '>')
			|| (s[j] == '<' && s[j + 1] == '<')))
		return (j + 2);
	if (len - j >= 1 && (s[j] == '>' || s[j] == '<'))
		return (j + 1);
	return (j);
}

static size_t	handle_quotes(char *s, size_t j)
{
	char	quote_char;

	quote_char = s[j];
	j++;
	while (s[j] && s[j] != quote_char)
		j++;
	if (s[j] == quote_char)
		j++;
	return (j);
}

static size_t	handle_charset(char *s, size_t j, int len)
{
	if (j > 0)
		return (j);
	if (len - j >= 3 && s[j] == '<' && s[j + 1] == '<' && s[j + 2] == '<')
		return (3);
	if (len - j >= 2 && ((s[j] == '<' && s[j + 1] == '<')
			|| (s[j] == '>' && s[j + 1] == '>')))
		return (2);
	return (1);
}

static int	is_numeric_redirection(char *s)
{
	size_t	j;

	j = 0;
	if (!is_in_charset(s[j], "0123456789"))
		return (0);
	while (is_in_charset(s[j], "0123456789"))
		j++;
	if (s[j] == '>' || s[j] == '<')
		return (1);
	return (0);
}

size_t	ft_token_len(char *s, char *charset, t_meta *meta)
{
	size_t	j;
	int		len;

	j = 0;
	len = ft_strlen(s);
	if (is_numeric_redirection(s))
		return (handle_redir(s, len));
	j = 0;
	while (s[j])
	{
		meta->last_quote_is = s[j];
		if (is_in_charset(s[j], charset))
			return (handle_charset(s, j, len));
		else if (s[j] == '\'' || s[j] == '\"')
			j = handle_quotes(s, j);
		else
			j++;
	}
	return (j);
}
