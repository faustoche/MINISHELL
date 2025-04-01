/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/01 16:29:16 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Define token type for each split part of the input according to the macros */

int	get_token_type(char *token, int *command)
{
	if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(token, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(token, ";") == 0)
		return (TOKEN_SEPARATOR);
	if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(token, "(") == 0)
		return (TOKEN_OPEN_PARENT);
	if (ft_strcmp(token, ")") == 0)
		return (TOKEN_CLOSE_PARENT);
	if (*command)
	{
		*command = 0;
		return (TOKEN_COMMAND);
	}
	return (TOKEN_ARGUMENT);
}

int	handle_std_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
{
	t_cmd	*new_cmd;

	if (!(*curr))
	{
		new_cmd = init_command();
		if (!new_cmd)
		{
			free_commands(*head);
			free(new_cmd);
			return (-1);
		}
		new_cmd->env_list = env;
		if (!(*head))
			*head = new_cmd;
		else
			(*curr)->next = new_cmd;
		*curr = new_cmd;
	}
	if (add_args(*tok, *curr) == -1)
	{
		free_commands(*head);
		return (-1);
	}
	return (0);
}

int	process_other_token(t_token **token, t_cmd **curr, t_cmd **head, t_env *env)
{
	if ((*token)->type == TOKEN_SEPARATOR)
		*curr = NULL;
	else
	{
		if (handle_std_token(token, curr, head, env))
			return (-1);
	}
	if (*token)
		*token = (*token)->next;
	return (0);
}

/* Selon le token concerné, j'appelle les fonctions correspondantes */

int	process_token(t_token **token, t_cmd **current, t_cmd **head, t_env *env)
{
	if ((*token)->type == TOKEN_PIPE)
		return (process_pipe_token(token, current, head));
	else if (redirection_token(*token))
		return (process_redirection_token(token, current, head));
	else
		return (process_other_token(token, current, head, env));
}

int	process_pipe_token(t_token **token, t_cmd **current, t_cmd **head)
{
	t_cmd	*new_cmd;

	if (!*current)
	{
		printf(ERR_SYNTAX);
		free_commands(*head);
		return (-1);
	}
	new_cmd = init_command();
	if (!new_cmd)
	{
		free_commands(*head);
		return (-1);
	}
	(*current)->next = new_cmd;
	*current = new_cmd;
	*token = (*token)->next;
	return (0);
}
