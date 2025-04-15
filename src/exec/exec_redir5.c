/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:07:24 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 18:29:41 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_redirection(t_cmd *cmd, t_env *env_list)
{
	int		pipefd[2];
	pid_t	pid;
	int		heredoc_fd;
	int		status;

	heredoc_fd = -1;
	if (cmd->heredoc != -1)
		heredoc_fd = cmd->heredoc;
	pid = create_pipe_and_fork(pipefd);
	if (pid == -1)
		return ;
	if (pid == 0)
		handle_child_process(cmd, env_list, pipefd, heredoc_fd);
	else
	{
		handle_signals(SIGINT, CLOSE_IN);
		(close(pipefd[1]), waitpid(pid, &status, 0));
		handle_exit_status_signals(status, cmd);
		if (heredoc_fd != -1)
		{
			close(heredoc_fd);
			cmd->heredoc = -1;
		}
	}
}

void	execute_redirection(t_cmd *cmd, t_env *env_list)
{
	if (!cmd)
		return ;
	if (is_builtins(cmd->args[0]))
		handle_builtin_redirection(cmd, &env_list);
	else
		handle_pipe_redirection(cmd, env_list);
}
