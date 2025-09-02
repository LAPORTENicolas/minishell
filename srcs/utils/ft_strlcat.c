/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 20:58:13 by nlaporte          #+#    #+#             */
/*   Updated: 2024/11/14 20:58:21 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_l;
	size_t	src_l;

	i = 0;
	dest_l = ft_strlen(dest);
	src_l = ft_strlen((char *)src);
	if (size <= dest_l)
		return (size + src_l);
	if (size > 0 && dest_l < size)
	{
		while (src[i] != '\0' && i < size - dest_l - 1)
		{
			dest[dest_l + i] = src[i];
			i++;
		}
		dest[dest_l + i] = '\0';
		return (src_l + dest_l);
	}
	return (size);
}
