/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:47:52 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/15 10:09:18 by fcrocq           ###   ########.fr       */
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
		exit(1);
	}
	close(heredoc_fd);
}

void	redir_input(char *input_file, int *code)
{
	int	fd;

	fd = open_file(input_file, REDIR_IN, code);
	if (fd == -1)
	{
		printf("minislay: %s: Permission denied\n", input_file);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	redir_output(char *output_file, int append_mode, int *code)
{
	int	fd;

	if (append_mode)
		fd = open_file(output_file, REDIR_APPEND, code);
	else
		fd = open_file(output_file, REDIR_OUT, code);
	if (fd == -1)
	{
		printf("fd == -1\n");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd);
		exit(1);
	}
	close(fd);
}

int	redir_execute(t_cmd *cmd, t_env *env_list)
{
	char	**env;
	char	*binary_path;

	env = env_list_to_array(env_list);
	binary_path = find_bin_path(cmd->args[0], env_list);
	if (!binary_path)
	{
		*(cmd->exit_status) = 127;
		fprintf(stderr, ERR_CMD, cmd->args[0]);
		free_pipe(cmd, env_list, env);
		exit(127);
	}
	execve(binary_path, cmd->args, env);
	perror("execve");
	free(binary_path);
	free_pipe(cmd, env_list, env);
	exit(126);
}
