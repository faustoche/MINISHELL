/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:28:18 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/15 08:26:09 by fcrocq           ###   ########.fr       */
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

// void	free_pipe_redir(t_cmd *cmd)
// {
// 	t_cmd	*tmp_cmd;
// 	t_cmd	*next;
// 	int		i;
// 	int		*last_exit_status;

// 	tmp_cmd = cmd;
// 	last_exit_status = NULL;
// 	while (tmp_cmd)
// 	{
// 		i = 0;
// 		next = tmp_cmd->next;
// 		free_files(tmp_cmd);
// 		if (tmp_cmd->args)
// 		{
// 			while (tmp_cmd->args[i])
// 			{
// 				ft_memdel(tmp_cmd->args[i]);
// 				i++;
// 			}
// 			ft_memdel(tmp_cmd->args);
// 		}
// 		if (tmp_cmd->exit_status)
// 		{
// 			if (!last_exit_status || last_exit_status != tmp_cmd->exit_status)
// 			{
// 				last_exit_status = tmp_cmd->exit_status;
// 				free(tmp_cmd->exit_status);
// 			}
// 		}
// 		ft_memdel(tmp_cmd);
// 		tmp_cmd = next;
// 	}
// 	close_all_fd(3);
// }

// void	free_pipe_redir(t_cmd *cmd)
// {
// 	t_cmd	*tmp_cmd;
// 	t_cmd	*next;
// 	int		i;

// 	tmp_cmd = cmd;
// 	while (tmp_cmd)
// 	{
// 		i = 0;
// 		next = tmp_cmd->next;
// 		free_files(tmp_cmd);
// 		if (tmp_cmd->args)
// 		{
// 			while (tmp_cmd->args[i])
// 			{
// 				ft_memdel(tmp_cmd->args[i]);
// 				i++;
// 			}
// 			ft_memdel(tmp_cmd->args);
// 		}
// 		ft_memdel(tmp_cmd);
// 		tmp_cmd = next;
// 	}
// 	close_all_fd(3);
// }
