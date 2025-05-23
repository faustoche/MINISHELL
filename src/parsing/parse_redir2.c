/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 08:03:34 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 10:22:59 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_for_current(t_token *token, t_cmd *current, t_cmd *head)
{
	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	return (0);
}

static int	process_handle_redirection_type(t_token *token, t_cmd *current)
{
	if (token->type == REDIR_OUT)
		return (redirection(&current, token->next->value, 1, 0));
	else if (token->type == REDIR_IN)
		return (redirection(&current, token->next->value, 0, 0));
	else if (token->type == REDIR_APPEND)
		return (redirection(&current, token->next->value, 1, 1));
	else if (token->type == HEREDOC)
	{
		if (!token->next)
			return (print_error_message("Error: expected heredoc delimiter\n"));
		if (current->heredoc_eof)
			free(current->heredoc_eof);
		current->heredoc_eof = remove_quotes(token->next->value);
		if (!current->heredoc_eof)
			return (print_error_message("Error: malloc heredoc_delim\n"));
		return (0);
	}
	return (0);
}

int	handle_redirection(t_token *token, t_cmd *current, t_cmd *head)
{
	int	result;

	result = 0;
	if (check_for_current(token, current, head) == -1)
		return (-1);
	result = process_handle_redirection_type(token, current);
	if (result == -1)
		free_commands(head);
	return (result);
}

int	process_tok(t_token **token, t_cmd **current, t_cmd **head, t_env *env)
{
	if (!(*token))
		return (0);
	if ((*token)->type == TOKEN_PIPE)
		return (process_pipe_token(token, current, head));
	else if ((*token)->type == REDIR_OUT || (*token)->type == REDIR_APPEND
		|| (*token)->type == REDIR_IN || (*token)->type == HEREDOC)
		return (process_redir_token(token, current, head, env));
	else
		return (process_other_tok(token, current, head, env));
}
