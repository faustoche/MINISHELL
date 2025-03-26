/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/26 17:51:05 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->in = 0;
	cmd->out = 0;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->nb_arg = 0;
	cmd->max_arg = 0;
	cmd->heredoc = -1;
	cmd->env_list = NULL;
	return (cmd);
}

t_cmd	*parse_commands(t_token *token_list, t_env *env_list)
{
	t_cmd	*head;
	t_cmd	*current ;
	t_token	*token;

	head = NULL;
	current = NULL;
	token = token_list;
	while (token)
	{
		if (process_token(&token, &current, &head, env_list))
			return (NULL);
	}
	if (head == NULL)
		printf(ERR_SYNTAX);
	return (head);
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

int	process_redirection_token(t_token **token, t_cmd **current, t_cmd **head)
{
	if (!(*token)->next || (*token)->next->type != TOKEN_ARGUMENT)
	{
		printf(ERR_SYNTAX);
		free_commands(*head);
		return (-1);
	}
	if (handle_redirection(*token, *current, *head) == -1)
		return (-1);
	*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
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

int	redirection_token(t_token *token)
{
	return (token->type == REDIR_OUT || token->type == REDIR_APPEND
		|| token->type == REDIR_IN || token->type == HEREDOC
		|| token->type == TOKEN_PIPE);
}

int	redirection_process(t_token **token, t_cmd **current, t_cmd **head)
{
	if (!*current)
	{
		printf(ERR_SYNTAX);
		free_commands(*head);
		return (-1);
	}
	if (handle_redirection(*token, *current, *head) == -1)
		return (-1);
	*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
	return (0);
}
