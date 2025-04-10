/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:47:52 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/10 08:47:52 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe(t_cmd *cmd, t_env *env_list, char **env)
{
	free_commands(cmd);
	free_env_list(&env_list);
	free_env_array(env);
	exit(EXIT_FAILURE);
}

void	redir_heredoc(int heredoc_fd)
{
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed for heredoc");
		exit(EXIT_FAILURE);
	}
	close(heredoc_fd);
}

void	redir_input(char *input_file)
{
	int	fd;

	fd = open_file(input_file, REDIR_IN);
	if (fd == -1)
		exit(EXIT_FAILURE);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redir_output(char *output_file, int append_mode)
{
	int	fd;

	if (append_mode)
		fd = open_file(output_file, REDIR_APPEND);
	else
		fd = open_file(output_file, REDIR_OUT);
	if (fd == -1)
	{
		printf("fd == -1\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redir_execute(t_cmd *cmd, t_env *env_list)
{
	char	**env;
	char	*binary_path;

	env = env_list_to_array(env_list);
	binary_path = find_binary_path(cmd->args[0], env_list);
	if (!binary_path)
	{
		printf(ERR_CMD, cmd->args[0]);
		free_pipe(cmd, env_list, env);
	}
	execve(binary_path, cmd->args, env);
	free(binary_path);
	free_pipe(cmd, env_list, env);
}
