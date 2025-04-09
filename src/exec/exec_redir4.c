/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:48:26 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/09 08:58:47 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_cmd *cmd, t_env *env, int pipefd[2], int fd)
{
	if (fd != -1)
		redir_heredoc(fd);
	else if (cmd->in)
		redir_input(cmd->in);
	if (cmd->out)
		redir_output(cmd->out, cmd->append);
	(close(pipefd[0]), close(pipefd[1]));
	if (cmd->args && cmd->args[0])
		redir_execute(cmd, env);
	exit(EXIT_FAILURE);
}

void	handle_pipe_redirection(t_cmd *cmd, t_env *env_list)
{
	int		pipefd[2];
	pid_t	pid;
	int		heredoc_fd;

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
		(close(pipefd[1]), waitpid(pid, NULL, 0));
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

static void	init_original_std(int *og_stdin, int *og_stdout)
{
	*og_stdin = -1;
	*og_stdout = -1;
}

void	execute_only_redirections(t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;

	init_original_std(&original_stdin, &original_stdout);
	if (cmd->in || cmd->heredoc != -1)
		original_stdin = handle_input_redirection(cmd);
	if (cmd->out)
		original_stdout = handle_output_redirection(cmd);
	if (original_stdin == -1 || original_stdout == -1)
	{
		if (original_stdin != -1)
			(dup2(original_stdin, STDIN_FILENO), close(original_stdin));
		if (original_stdout != -1)
			(dup2(original_stdout, STDOUT_FILENO), close(original_stdout));
		return ;
	}
	if (original_stdin != -1)
		(dup2(original_stdin, STDIN_FILENO), close(original_stdin));
	if (original_stdout != -1)
		(dup2(original_stdout, STDOUT_FILENO), close(original_stdout));
	if (cmd->heredoc != -1)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
	}
}
