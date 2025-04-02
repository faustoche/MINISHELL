/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/02 16:13:48 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Ouvre le fichier en fonction de la redirection */

int	open_file(char *filename, int token)
{
	int	fd;

	fd = 0;
	if (!filename)
	{
		printf("Error: NULL filename\n");
		return (-1);
	}
	if (token == 4)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (token == 6)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (token == 5)
		fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("No such or directory : %s\n", filename);
		return (-1);
	}
	return (fd);
}

void	redirect(int fd, int std_fd)
{
	if (dup2(fd, std_fd) == -1)
		perror("error : dup2 failed\n");
	close(fd);
}

/* According to the token, redirect in the right direction */

int	handle_redirection(t_token *token, t_cmd *current, t_cmd *head)
{
	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	if (token->type == REDIR_OUT)
		return (redirection(current, token->next->value, 1, 0));
	else if (token->type == REDIR_IN)
		return (redirection(current, token->next->value, 0, 0));
	else if (token->type == REDIR_APPEND)
		return (redirection(current, token->next->value, 1, 1));
	else if (token->type == HEREDOC)
		return (handle_heredoc(current, token->next->value, head));
	return (0);
}

int	redirection(t_cmd *cmd, char *file, int out, int append)
{
	if (!cmd || !file)
		return (-1);
	if (out)
	{
		if (cmd->out)
			free(cmd->out);
		cmd->out = ft_strdup(file);
		if (!cmd->out)
			return (-1);
		cmd->append = append;
	}
	else
	{
		if (cmd->in)
			free(cmd->in);
		cmd->in = ft_strdup(file);
		if (!cmd->in)
			return (-1);
	}
	return (0);
}

/* Plus de fd ouvert ici */

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd || !delimiter)
	{
		free_commands(head);
		return (print_error_message("Error : invalid heredoc\n"));
	}
	if (pipe(pipe_fd) == -1)
		return (print_error_message("error pipe creation failed\n"));
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(pipe_fd[1], line, ft_strlen(line)) == -1
			|| write(pipe_fd[1], "\n", 1) == -1)
		{
			perror("Write function failed\n");
			free(line);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			return (-1);
		}
		free(line);
	}
	close(pipe_fd[1]);
	cmd->heredoc = pipe_fd[0];
	close(pipe_fd[0]);
	return (0);
}
