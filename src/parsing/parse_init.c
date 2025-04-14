/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:42:26 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_args(t_cmd *cmd)
{
	cmd->max_arg = 6;
	cmd->args = malloc(sizeof(char *) * (cmd->max_arg + 1));
	if (!cmd->args)
		return (-1);
	cmd->nb_arg = 0;
	cmd->args[0] = NULL;
	return (0);
}

static int	expand_args(t_cmd *cmd)
{
	char	**new_args;
	size_t	i;

	i = 0;
	new_args = malloc(sizeof(char *) * (cmd->max_arg * 2 + 1));
	if (!new_args)
		return (-1);
	while (i <= cmd->nb_arg)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->max_arg *= 2;
	return (0);
}

/* remove quotes a la place de strdup pour gerer l'expansion*/
int	add_args(t_token *token, t_cmd *cmd)
{
	int	i;

	if (!cmd->args)
	{
		if (init_args(cmd) == -1)
			return (-1);
	}
	if (cmd->nb_arg >= cmd->max_arg - 1)
	{
		if (expand_args(cmd) == -1)
			return (-1);
	}
	cmd->args[cmd->nb_arg] = remove_quotes(token->value);
	if (!cmd->args[cmd->nb_arg])
	{
		i = -1;
		while (cmd->args[++i])
			free(cmd->args[i]);
		free(cmd->args);
		cmd->args = NULL;
		return (-1);
	}
	cmd->nb_arg++;
	cmd->args[cmd->nb_arg] = NULL;
	return (0);
}

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
	cmd->heredoc_eof = NULL;
	cmd->pid = -1;
	cmd->env_list = NULL;
	cmd->exit_status = malloc(sizeof(int));
	if (!cmd->exit_status)
	{
		free(cmd);
		return (NULL);
	}
	*(cmd->exit_status) = 0;
	return (cmd);
}

t_cmd	*parse_commands(t_token *token_list, t_env *env_list, int *code)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*token;

	head = NULL;
	current = NULL;
	token = token_list;
	while (token)
	{
		if (process_token(&token, &current, &head, env_list))
		{
			*code = 2;
			if (head)
				free_commands(head);
			return (NULL);
		}
	}
	if (head == NULL)
	{
		printf(ERR_SYNTAX);
		*code = 2;
		if (current != NULL && current != head)
			free_commands(current);
	}
	return (head);
}
