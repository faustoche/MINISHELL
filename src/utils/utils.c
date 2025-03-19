/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/19 08:43:22 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_commands(t_cmd *cmd)
{
	int	i;
	int	cmd_num;

	cmd_num = 0;
	while (cmd)
	{
		printf("\nCommande %d :\n", cmd_num++);
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf("Arg[%d]: %s\n", i, cmd->args[i]);
				i++;
			}
		}
		if (cmd->in)
			printf("Entrée : %s\n", cmd->in);
		if (cmd->out)
			printf("Sortie : %s (append: %d)\n", cmd->out, cmd->append);
		cmd = cmd->next;
	}
}

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

int	is_redirection(t_cmd *cmd)
{
	return (cmd->in || cmd->out || cmd->heredoc);
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