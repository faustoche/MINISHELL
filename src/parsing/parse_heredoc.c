/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/10 13:52:47 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	read_heredoc_content(char *delimiter, int write_fd)
{
	char	*input;
	
	//g_received_signal = 0;
	while (1)
	{
		input = readline("heredoc> ");
		if (!input)
		{
			if (g_received_signal == 130)
				return (close(write_fd), -1);
			printf("minislay: warning: here-document delimited by eof\n");
			close(write_fd);
			return (0);
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		write(write_fd, input, ft_strlen(input));
		write(write_fd, "\n", 1);
		free(input);
	}
	close(write_fd);
	return (0);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head)
{
	int	pipe_fd[2];
	int	fd;
	int stat = 0;

	fd = dup(STDIN_FILENO); 
	if (!cmd || !delimiter)
	{
		free_commands(head);
		return (print_error_message("Error: invalid heredoc\n"));
	}
	if (pipe(pipe_fd) == -1)
		return (print_error_message("Error: pipe creation failed\n"));
	handle_signals(SIGINT, CLOSE_IN);
	stat = read_heredoc_content(delimiter, pipe_fd[1]);
	if (stat == -1)
		return (dup2(fd, STDIN_FILENO), close(pipe_fd[1]), -1);
	close(pipe_fd[1]);
	if (cmd->heredoc != -1)
		close(cmd->heredoc);
	cmd->heredoc = pipe_fd[0];
	return (0);
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
	return (0);
}
