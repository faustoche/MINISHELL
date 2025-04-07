/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/05 15:47:01 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Complete pathname by adding '/' and name of binary */

char	*build_pathname(char *directory, char *arg)
{
	char	*binary_path;
	char	*tmp;
	size_t	len;

	directory = ft_realloc(directory, ft_strlen(directory) + 2);
	if (!directory)
		return (NULL);
	if (directory[ft_strlen(directory) - 1] != '/')
	{
		tmp = directory;
		directory = ft_strjoin(directory, "/");
		free(tmp);
	}
	len = ft_strlen(directory) + ft_strlen(arg);
	binary_path = malloc(sizeof(char) * len + 1);
	if (!binary_path)
	{
		free(directory);
		return (NULL);
	}
	tmp = binary_path;
	binary_path = ft_strjoin(directory, arg);
	free(tmp);
	free(directory);
	return (binary_path);
}

/* Check each directory in PATH to find binary_path */

char	*find_binary_path(char *arg)
{
	char	*path_env;
	char	**split_path;
	char	*binary_path;
	char	*path_copy;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	split_path = ft_split(path_env, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	binary_path = NULL;
	while (split_path[i])
	{
		path_copy = ft_strdup(split_path[i]);
		if (!path_copy)
		{
			free_split(split_path);
			return (NULL);
		}
		binary_path = build_pathname(path_copy, arg);
		if (!access(binary_path, F_OK))
		{
			free_split(split_path);
			return (binary_path);
		}
		free(binary_path);
		i++;
	}
	free_split(split_path);
	return (NULL);
}

static void	execute_child_process(char **args, char *binary_path)
{
	if (access(binary_path, X_OK) == -1)
		printf(ERR_CMD, args[0]);
	else if (execve(binary_path, args, NULL) == -1)
	{
		close_all_fd(3);
		exit(EXIT_FAILURE);
	}
	close_all_fd(3);
}

/* Create child process and execute */

static void	create_child_process(char **args, char *binary_path)
{
	pid_t	pid;
	pid_t	result;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed\n");
		return ;
	}
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		execute_child_process(args, binary_path);
	}
	else if (pid > 0)
	{
		signal(SIGQUIT, sigquit_handler);
		result = waitpid(pid, &status, 0);
		close_all_fd(3);
		if (result == -1)
		{
			perror("waitpid failed");
			return ;
		}
		else if (!WIFEXITED(status))
			return ;	
	}
	close_all_fd(3);
}

void	execute_commands(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current;
	char	*binary_path;

	current = cmd;
	while (current)
	{
		current->processed = 1;
		if (current->args && current->args[0] && is_builtins(current->args[0]))
			builtins_execution(current, &env_list);
		else if (current->args && current->args[0])
		{
			if ((current->args[0][0] == '/' || current->args[0][0] == '.')
				&& (current->args[0][1] == '/' || current->args[0][1] == '.'))
				printf(ERR_DIR, current->args[0]);
			else
			{
				binary_path = find_binary_path(current->args[0]);
				if (binary_path == NULL)
					printf(ERR_CMD, current->args[0]);
				else
				{
					create_child_process(current->args, binary_path);
					free(binary_path);
				}
			}
		}
		current = current->next;
	}
}
