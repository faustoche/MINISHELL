/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 10:23:14 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Define token type for each split part of the input according to the macro */

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
		return (TOKEN_SEP);
	if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	if (*command)
	{
		*command = 0;
		return (TOKEN_COMMAND);
	}
	return (TOKEN_ARGUMENT);
}

int	init_and_add_cmd(t_cmd **curr, t_cmd **head, t_env *env)
{
	t_cmd	*new_cmd;
	t_cmd	*tmp;

	new_cmd = init_command();
	if (!new_cmd)
		return (-1);
	new_cmd->env_list = env;
	if (!(*head))
		*head = new_cmd;
	else
	{
		tmp = *head;
		while (tmp && tmp->next)
			tmp = tmp->next;
		if (tmp)
			tmp->next = new_cmd;
		else
			*head = new_cmd;
	}
	*curr = new_cmd;
	return (0);
}

int	handle_std_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
{
	if (!(*curr))
	{
		if (init_and_add_cmd(curr, head, env) == -1)
			return (-1);
	}
	if (add_args(*tok, *curr) == -1)
		return (-1);
	return (0);
}

int	process_other_tok(t_token **tok, t_cmd **cur, t_cmd **head, t_env *env)
{
	if ((*tok)->type == TOKEN_SEP)
		*cur = NULL;
	else
	{
		if (handle_std_token(tok, cur, head, env))
			return (-1);
	}
	if (*tok)
		*tok = (*tok)->next;
	return (0);
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
	new_cmd->env_list = (*current)->env_list;
	free(new_cmd->exit_status);
	new_cmd->exit_status = (*current)->exit_status;
	(*current)->next = new_cmd;
	*current = new_cmd;
	*token = (*token)->next;
	return (0);
}
