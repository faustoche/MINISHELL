/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:41:42 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// expoand variable
// si expand pas null (donc $)
// alors je write avec expanded imput
// sinon entree originale

static int	read_heredoc_content(char *delimiter, int write_fd)
{
	char	*input;

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
		if (input)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
			free(input);
		}
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
		if (head && head->exit_status)
			*(head->exit_status) = 1; // TBC
		free_commands(head);
		return (print_error_message("Error: invalid heredoc\n"));
	}
	if (pipe(pipe_fd) == -1)
	{
		if (cmd->exit_status)
			*(cmd->exit_status) = 1; // TBC
		return (print_error_message("Error: pipe creation failed\n"));
	}
	handle_signals(SIGINT, CLOSE_IN);
	stat = read_heredoc_content(delimiter, pipe_fd[1]);
	if (stat == -1)
	{
		*(cmd->exit_status) = 130; // TBC // askip sigint
		return (dup2(fd, STDIN_FILENO), close(pipe_fd[1]), -1);
	}
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

// static int	read_heredoc_content(char *delimiter, int write_fd, t_env *env_list, int *code)
// {
// 	char	*input;
// 	char	*input_expand;

// 	while (1)
// 	{
// 		input = readline("heredoc> ");
// 		if (!input)
// 		{
// 			if (g_received_signal == 130)
// 				return (close(write_fd), -1);
// 			printf("minislay: warning: here-document delimited by eof\n");
// 			close(write_fd);
// 			return (0);
// 		}
// 		if (ft_strcmp(input, delimiter) == 0)
// 		{
// 			free(input);
// 			break ;
// 		}
// 		input_expand = expand_variable(env_list, input, 0, code); // POURQUOI CA EXPAND PAS HAAAAAAA
// 		if (input_expand)
// 		{
// 			write(write_fd, input_expand, ft_strlen(input_expand));
// 			write(write_fd, "\n", 1);
// 			free(input_expand);
// 		}
// 		else
// 		{
// 			write(write_fd, input, ft_strlen(input));
// 			write(write_fd, "\n", 1);
// 		}
// 		free(input); // ici ou en deouus ? a voir si leaks
// 	}
// 	close(write_fd);
// 	return (0);
// }
