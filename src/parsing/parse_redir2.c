/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/06 18:20:36 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redir_token(t_token **tok, t_cmd **curr, t_cmd **head, t_env *env)
{
	if (!(*curr))
	{
		*curr = init_command();
		if (!(*curr))
			return (-1);
		(*curr)->env_list = env;
		if (!(*head))
			*head = *curr;
		if (init_args(*curr) == -1)
			return (-1);
	}
	if (!(*tok)->next || ((*tok)->next->type != TOKEN_ARGUMENT
			&& (*tok)->next->type != TOKEN_COMMAND))
	{
		printf(ERR_SYNTAX);
		free_commands(*head);
		return (-1);
	}
	if (handle_redirection(*tok, *curr, *head) == -1)
		return (-1);
	*tok = (*tok)->next;
	if (*tok)
		*tok = (*tok)->next;
	return (0);
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
