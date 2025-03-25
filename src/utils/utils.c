/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/25 18:40:55 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(int c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';'
		|| c == '&' || c == '(' || c == ')');
}

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	skip_space(t_lexer *lexer)
{
	while (lexer->input[lexer->pos] && is_space(lexer->input[lexer->pos]))
		lexer->pos++;
}

int is_redirection(t_cmd *cmd)
{
    return ((cmd->in || cmd->out || cmd->heredoc != -1) ? 1 : 0);
}

int is_numeric(char *str)
{
    int i;
    
    if (!str)
        return (0);
    if (str[0] == '-' || str[0] == '+')
        i = 1;
    else
        i = 0;
    
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}