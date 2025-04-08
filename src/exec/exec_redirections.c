/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:30:49 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 09:23:19 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_input_redirection(t_cmd *cmd)
{
	int fd_in;
	int original_stdin;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);

	if (cmd->heredoc != -1)
	{
		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		{
			close(original_stdin);
			return (-1);
		}
	}
	else if (cmd->in)
	{
		fd_in = open_file(cmd->in, REDIR_IN);
		if (fd_in == -1)
		{
			close(original_stdin);
			return (-1);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			close(fd_in);
			close(original_stdin);
			return (-1);
		}
		close(fd_in);
	}
	return (original_stdin);
}

int	handle_output_redirection(t_cmd *cmd)
{
	int	fd_out;
	int	original_stdout;

	fd_out = STDOUT_FILENO;
	original_stdout = dup(STDOUT_FILENO);
	if (cmd->out)
	{
		if (cmd->append)
			fd_out = open_file(cmd->out, REDIR_APPEND);
		else
			fd_out = open_file(cmd->out, REDIR_OUT);
		if (fd_out == -1)
			return (-1);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	return (original_stdout);
}

void handle_builtin_redirection(t_cmd *cmd, t_env **env_list)
{
	int original_stdin;
	int original_stdout;
	int heredoc_fd;
	
	heredoc_fd = -1;
	if (cmd->heredoc != -1)
		heredoc_fd = cmd->heredoc;
	original_stdin = handle_input_redirection(cmd);
	if (original_stdin == -1)
		return ;
	original_stdout = handle_output_redirection(cmd);
	if (original_stdout == -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		return ;
	}
	builtins_execution(cmd, env_list);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	if (heredoc_fd != -1)
	{
		close(heredoc_fd);
		cmd->heredoc = -1;
	}
}

static void handle_pipe_redirection(t_cmd *cmd, t_env *env_list)
{
	int     pipefd[2];
	pid_t   pid;
	int     heredoc_fd = -1;
	
	if (cmd->heredoc != -1)
		heredoc_fd = cmd->heredoc;
	pid = create_pipe_and_fork(pipefd);
	if (pid == -1)
		return;
	if (pid == 0)
	{
		if (heredoc_fd != -1)
		{
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 failed for heredoc");
				exit(EXIT_FAILURE);
			}
		}
		else if (cmd->in)
		{
			int fd = open_file(cmd->in, REDIR_IN);
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
		if (cmd->out)
		{
			int fd;
			if (cmd->append)
				fd = open_file(cmd->out, REDIR_APPEND);
			else
				fd = open_file(cmd->out, REDIR_OUT);
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
		close(pipefd[0]);
		close(pipefd[1]);
		if (cmd->args && cmd->args[0])
		{
			char **env = env_list_to_array(env_list);
			char *binary_path = find_binary_path(cmd->args[0]);
			if (!binary_path)
			{
				printf(ERR_CMD, cmd->args[0]);
				free_commands(cmd);
				free_env_list(&env_list);
				free_env_array(env);
				exit(EXIT_FAILURE);
			}
			execve(binary_path, cmd->args, env);
			free(binary_path);
			free_env_array(env);
			free_commands(cmd);
			free(binary_path);
			free_env_list(&env_list);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		if (heredoc_fd != -1)
		{
			close(heredoc_fd);
			cmd->heredoc = -1;
		}
	}
}


void execute_redirection(t_cmd *cmd, t_env *env_list)
{
	if (!cmd)
		return;
	
	if (is_builtins(cmd->args[0]))
		handle_builtin_redirection(cmd, &env_list);
	else
		handle_pipe_redirection(cmd, env_list);
}

void execute_only_redirections(t_cmd *cmd)
{
	int original_stdin = -1;
	int original_stdout = -1;
	
	if (cmd->in || cmd->heredoc != -1)
		original_stdin = handle_input_redirection(cmd);
	if (cmd->out)
		original_stdout = handle_output_redirection(cmd);
	if (original_stdin == -1 || original_stdout == -1)
	{
		if (original_stdin != -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdin);
		}
		if (original_stdout != -1)
		{
			dup2(original_stdout, STDOUT_FILENO);
			close(original_stdout);
		}
		return;
	}
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
	if (cmd->heredoc != -1)
	{
		close(cmd->heredoc);
		cmd->heredoc = -1;
	}
}

char **env_list_to_array(t_env *env_list)
{
	int count = 0;
	t_env *current = env_list;

	while (current)
	{
		count++;
		current = current->next;
	}
	char **result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;
	current = env_list;
	count = 0;
	while (current)
	{
		char *env_str = malloc(strlen(current->name) + strlen(current->value) + 2);
		if (!env_str)
		{
			while (--count >= 0)
				free(result[count]);
			free(result);
			return NULL;
		}
		strcpy(env_str, current->name);
		strcat(env_str, "=");
		strcat(env_str, current->value);
		result[count++] = env_str;
		current = current->next;
	}
	result[count] = NULL;
	return result;
}