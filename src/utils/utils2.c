/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:55:47 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/06 09:19:18 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strcpy(char *dest, char *src)
{
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return (dest);
}

int	is_separator(int c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';'
		|| c == '&' || c == '(' || c == ')');
}

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

void	skip_space(t_lexer *lexer)
{
	while (is_space(lexer->input[lexer->pos]))
		lexer->pos++;
}