/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:30:49 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/09 09:00:50 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_variable(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_var)
{
	char	*env_str;

	env_str = malloc(ft_strlen(env_var->name) + ft_strlen(env_var->value) + 2);
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, env_var->name);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, env_var->value);
	return (env_str);
}

static int	fill_env_array(char **result, t_env *env_list)
{
	int		count;
	t_env	*cur;
	char	*env_str;

	cur = env_list;
	count = 0;
	while (cur)
	{
		env_str = create_env_string(cur);
		if (!env_str)
		{
			while (--count >= 0)
				free(result[count]);
			return (-1);
		}
		result[count++] = env_str;
		cur = cur->next;
	}
	result[count] = NULL;
	return (0);
}

char	**env_list_to_array(t_env *env_list)
{
	int		count;
	char	**result;

	count = count_env_variable(env_list);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	if (fill_env_array(result, env_list) == -1)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

// static void	handle_pipe_redirection(t_cmd *cmd, t_env *env_list)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	int		heredoc_fd;
// 	int		fd;
// 	char	**env;
// 	char	*binary_path;
// 	heredoc_fd = -1;
// 	if (cmd->heredoc != -1)
// 	heredoc_fd = cmd->heredoc;
// 	pid = create_pipe_and_fork(pipefd);
// 	if (pid == -1)
// 	return ;
// 	if (pid == 0)
// 	{
// 		if (heredoc_fd != -1)
// 		{
// 			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
// 			(perror("dup2 failed for heredoc"), exit(EXIT_FAILURE));
// 			close(heredoc_fd);
// 		}
// 		else if (cmd->in)
// 		{
// 			fd = open_file(cmd->in, REDIR_IN);
// 			if (fd == -1)
// 			exit(EXIT_FAILURE);
// 			if (dup2(fd, STDIN_FILENO) == -1)
// 			(perror("dup2 failed"), close(fd), exit(EXIT_FAILURE));
// 			close(fd);
// 		}
// 		if (cmd->out)
// 		{
// 			if (cmd->append)
// 				fd = open_file(cmd->out, REDIR_APPEND);
// 				else
// 				fd = open_file(cmd->out, REDIR_OUT);
// 			if (fd == -1)
// 			(printf("fd == -1\n"), exit(EXIT_FAILURE));
// 			if (dup2(fd, STDOUT_FILENO) == -1)
// 			(perror("dup2 failed"), close(fd), exit(EXIT_FAILURE));
// 			close(fd);
// 		}
// 		(close(pipefd[0]), close(pipefd[1]));
// 		if (cmd->args && cmd->args[0])
// 		{
// 			env = env_list_to_array(env_list);
// 			binary_path = find_binary_path(cmd->args[0]);
// 			if (!binary_path)
// 			(printf(ERR_CMD, cmd->args[0]), free_pipe(cmd, env_list, env));
// 			execve(binary_path, cmd->args, env);
// 			(free(binary_path), free_pipe(cmd, env_list, env));
// 		}
// 		exit(EXIT_FAILURE);
// 	}
// 	else
// 	{
// 		(close(pipefd[1]), waitpid(pid, NULL, 0));
// 		if (heredoc_fd != -1)
// 		{
// 			close(heredoc_fd);
// 			cmd->heredoc = -1;
// 		}
// 	}
// }

// int	handle_input_redirection(t_cmd *cmd)
// {
	// 	int	fd_in;
	// 	int	original_stdin;
	// 	original_stdin = dup(STDIN_FILENO);
	// 	if (original_stdin == -1)
	// 		return (-1);
	// 	if (cmd->heredoc != -1)
	// 	{
		// 		if (dup2(cmd->heredoc, STDIN_FILENO) == -1)
		// 		{
			// 			close(original_stdin);
			// 			return (-1);
			// 		}
			// 	}
			// 	else if (cmd->in)
			// 	{
				// 		fd_in = open_file(cmd->in, REDIR_IN);
				// 		if (fd_in == -1)
				// 		{
// 			close(original_stdin);
// 			return (-1);
// 		}
// 		if (dup2(fd_in, STDIN_FILENO) == -1)
// 		{
	// 			close(fd_in);
	// 			close(original_stdin);
	// 			return (-1);
	// 		}
	// 		close(fd_in);
	// 	}
	// 	return (original_stdin);
// }