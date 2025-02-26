/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/26 11:45:39 by fcrocq           ###   ########.fr       */
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
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->nb_arg = 0;
	cmd->max_arg = 0;
	cmd->heredoc = -1;
	return (cmd);
}

/* Convertiti la liste de tokens en structure de commande */

t_cmd	*parse_commands(t_token *token_list)
{
	t_cmd	*head;
	t_cmd	*current ;
	t_token	*token;

	head = NULL;
	current = NULL;
	token = token_list;
	while (token)
	{
		if (process_token(&token, &current, &head))
			return (NULL);
	}
	if (head == NULL)
		printf(ERR_SYNTAX);
	return (head);
}

int	process_token(t_token **token, t_cmd **current, t_cmd **head)
{
	if (redirection_token(*token) || (*token)->type == HEREDOC)
	{
		if (redirection_process(token, current, head))
			return (-1);
	}
	else if ((*token)->type == TOKEN_PIPE)
	{
		if (handle_pipe(current) == -1)
			return (-1);
	}
	else if ((*token)->type == TOKEN_SEPARATOR)
		*current = NULL;
	else
	{
		if (handle_standard_token(current, head, (*token)->value))
			return (-1);
	}
	if (*token)
		*token = (*token)->next;
	return (0);
}

int	redirection_token(t_token *token)
{
	return (token->type == REDIR_OUT || token->type == REDIR_APPEND
		|| token->type == REDIR_IN || token->type == HEREDOC);
}

int	redirection_process(t_token **token, t_cmd **current, t_cmd **head)
{
	if (!*current)
	{
		printf(ERR_SYNTAX);
		free_commands(*head);
		return (-1);
	}
	if (handle_redirection(*current, *token, *head) == -1)
		return (-1);
	*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
	return (0);
}

/*
Dans process token

if ((*token)->type == TOKEN_OPEN_PAREN)
    {
        // Traitement de l'ouverture de parenthèse
        // Stockage du contexte actuel et création d'un nouveau contexte
    }
    else if ((*token)->type == TOKEN_CLOSE_PAREN)
    {
        // Traitement de la fermeture de parenthèse
        // Retour au contexte précédent
    }

*/