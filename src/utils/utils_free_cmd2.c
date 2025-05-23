/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_cmd2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 08:01:57 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/17 10:24:13 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	free_pipe_redir(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_cmd	*next;
	int		*last_exit_status;

	tmp_cmd = cmd;
	last_exit_status = NULL;
	while (tmp_cmd)
	{
		next = tmp_cmd->next;
		free_single_command(tmp_cmd, &last_exit_status);
		tmp_cmd = next;
	}
	close_all_fd(3);
}
