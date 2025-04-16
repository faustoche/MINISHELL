/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:44:08 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/16 16:28:07 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	init_pipe_struct(t_cmd *cmd, t_env **env_list)
{
	t_pipe	pipe_data;

	pipe_data.cmd = cmd;
	pipe_data.env_list = env_list;
	pipe_data.input_fd = STDIN_FILENO;
	pipe_data.pipe_fd[0] = -1;
	pipe_data.pipe_fd[1] = -1;
	return (pipe_data);
}

void	handle_command_not_found(t_pipe *pipe_data, char **env)
{
	ft_putstr_fd("minislay: command not found\n", 2);
	free_env_array(env);
	free_env_list(pipe_data->env_list);
	free_commands(pipe_data->cmd);
	exit(127);
}
