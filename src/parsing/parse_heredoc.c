/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 18:33:04 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_content(char *delimiter, int fd)
{
	char	*in;

	while (1)
	{
		in = readline("heredoc> ");
		if (!in)
		{
			if (g_received_signal == SIGINT)
			{
				g_received_signal = 0;
				return (close(fd), -1);
			}
			(printf(ERR_HERE), close(fd));
			return (0);
		}
		if (ft_strcmp(in, delimiter) == 0)
		{
			free(in);
			break ;
		}
		if (in)
			(write(fd, in, ft_strlen(in)), write(fd, "\n", 1), free(in));
	}
	close(fd);
	return (0);
}

static int	init_heredoc(t_cmd *cmd, char *del, int *fd, int *pipe_fd)
{
	*fd = dup(STDIN_FILENO);
	if (!cmd || !del)
	{
		if (cmd && cmd->exit_status)
			*(cmd->exit_status) = 1;
		free_commands(cmd);
		return (print_error_message("Error: invalid heredoc\n"));
	}
	if (pipe(pipe_fd) == -1)
	{
		if (cmd->exit_status)
			*(cmd->exit_status) = 1;
		return (print_error_message("Error: pipe creation failed\n"));
	}
	return (0);
}

static int	handle_heredoc_content(t_cmd *cmd, char *del, int fd, int *pipe_fd)
{
	int	stat;

	handle_signals(SIGINT, CLOSE_IN);
	stat = read_heredoc_content(del, pipe_fd[1]);
	if (stat == -1)
	{
		if (cmd->exit_status)
			*(cmd->exit_status) = 130;
		g_received_signal = 0;
		dup2(fd, STDIN_FILENO);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	if (cmd->heredoc != -1)
		close(cmd->heredoc);
	cmd->heredoc = pipe_fd[0];
	return (0);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head)
{
	int	pipe_fd[2];
	int	fd;
	int	stat;

	stat = init_heredoc(cmd, delimiter, &fd, pipe_fd);
	if (stat != 0)
	{
		if (head && head->exit_status)
			*(head->exit_status) = 1;
		free_commands(head);
		return (stat);
	}
	return (handle_heredoc_content(cmd, delimiter, fd, pipe_fd));
}

int	handle_all_heredocs(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc_eof)
		{
			if (handle_heredoc(cmd, cmd->heredoc_eof, cmd) == -1)
				return (-1);
			free(cmd->heredoc_eof);
			cmd->heredoc_eof = NULL;
		}
		cmd = cmd->next;
	}
	handle_signals(SIGINT, PROMPT);
	handle_signals(SIGQUIT, IGNORE);
	return (0);
}
