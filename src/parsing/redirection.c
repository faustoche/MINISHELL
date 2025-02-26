/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/02/25 14:08:28 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Ouvre le fichier en fonction de la redirection */

int	open_file(char *filename, int token)
{
	int	fd;

	fd = 0;
	if (token == 4)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (token == 6)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (token == 5)
		fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("No such or directory : %s\n", filename);
		exit (EXIT_FAILURE);
	}
	return (fd);
}

/* RRedirige la sortie standard vers un fichier */

void	redirect_out(int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2 failed for out");
	close(fd);
}

/* Redirige l'entrée standard depuis un fichier */

void	redirect_in(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2 failed for in");
	close(fd);
}

/* According to the token, redirect in the right direction */

int	handle_redirection(t_cmd *current, t_token *token, t_cmd *head)
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

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd || !delimiter)
	{
		printf("Error : invalid heredoc\n");
		free_commands(head);
		return (-1);
	}
	if (pipe(&pipe_fd[1]) == -1)
	{
		printf("Pipe creation failed\n");
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	cmd->heredoc = pipe_fd[0];
	return (0);
}
