/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:53:05 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 15:46:35 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Creer une redirection in  a partir dune liste de tokens
// avant dun token pour aller au fichier, puis le stock
// puis retourne la redir in
static t_r_in	*init_redir_in(t_token *token)
{
	t_r_in	*redir_in;

	redir_in = malloc(sizeof(t_r_in));
	if (!redir_in)
		return (NULL);
	ft_memset(redir_in, 0, sizeof(t_r_in));
	redir_in->type = ft_strlen(token->data);
	return (redir_in);
}

static int	handle_single_redir(t_env *minishell, \
		t_list *head, t_r_in *redir_in)
{
	t_token	*token;
	char	*eof;

	token = consume_token(head);
	if (!token)
		return (-1);
	if (token->type != TOK_WORD)
		return (-1);
	redir_in->is_quote = token->is_quote;
	redir_in->path = ft_strndup((char *)token->data, \
		ft_strlen((char *)token->data));
	if (redir_in->type == 2)
		redir_in->file = get_file_name();
	if (redir_in->type == 2 && token->is_quote == 0)
	{
		eof = expand(minishell, redir_in->path, minishell->meta);
		if (!eof || !*eof)
			return (0);
		free(redir_in->path);
		redir_in->path = eof;
	}
	return (0);
}

t_r_in	*get_redir_in(t_env *minishell, t_list *head, t_tree *node)
{
	t_r_in		*redir_in;
	t_token		*token;

	(void)node;
	(void)minishell;
	if (!head)
		return (NULL);
	token = consume_token(head);
	redir_in = init_redir_in(token);
	if (!redir_in)
		return (NULL);
	if (redir_in->type == 1 || redir_in->type == 2)
	{
		if (handle_single_redir(minishell, head, redir_in) != 0)
		{
			free(redir_in);
			return (NULL);
		}
	}
	else
		NULL;
	return (redir_in);
}

int	file_can_open(char *path)
{
	if (!path)
		return (-1);
	if (access(path, R_OK | W_OK) != 0)
		return (0);
	return (1);
}

char	*get_file_name(void)
{
	int		fd;
	char	*file;
	char	buf[1001];
	int		i[3];

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (ft_memset(buf, 0, 1000) && read(fd, buf, 999) == -1)
		return (NULL);
	buf[999] = 0;
	i[2] = give_file_name_size(buf);
	file = malloc(sizeof(char) * i[2]);
	if (!file)
		return ((close(fd), NULL));
	i[0] = 0;
	i[1] = 0;
	while (buf[i[0]] && i[0] < 999 && i[1] < 29)
	{
		if (ft_isalnum(buf[i[0]]))
			file[i[1]++] = buf[i[0]];
		i[0]++;
	}
	file[i[2] - 1] = 0;
	return (close(fd), (file));
}
