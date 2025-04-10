/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:28:18 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/10 19:44:54 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env **env)
{
	t_env	*tmp;

	if (!env || !(*env))
		return ;
	while (*env)
	{
		tmp = (*env)->next;
		free((*env)->name);
		free((*env)->value);
		free(*env);
		*env = tmp;
	}
	*env = NULL;
}

void	free_elements(t_env *element)
{
	if (element)
	{
		free(element->name);
		free(element->value);
		free(element);
	}
}

void	free_token_list(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

void _ft_memdel(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void free_files(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in)
		_ft_memdel(cmd->in);
	if (cmd->out)
		_ft_memdel(cmd->out);
	if (cmd->heredoc_eof)
		_ft_memdel(cmd->heredoc_eof);
}

void	free_pipe_redir(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_cmd	*next;
	int		i;

	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		i = 0;
		next = tmp_cmd->next;
		free_files(tmp_cmd);
		if (tmp_cmd->args)
		{
			while (tmp_cmd->args[i])
			{
				_ft_memdel(tmp_cmd->args[i]);
				i++;
			}
			_ft_memdel(tmp_cmd->args);
		}
		_ft_memdel(tmp_cmd);
		tmp_cmd = next;
	}
	close_all_fd(3);
}
