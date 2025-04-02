/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/02 10:20:41 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Complete pathname by adding '/' and name of binary */

/* Ici à modifier car // ou n'importe quoi devrait envoyer ERR SYNTAX*/

char	*build_pathname(char *directory, char *arg)
{
	char	*binary_path;
	int		j;
	size_t	len;

	j = 0;
	directory = ft_realloc(directory, ft_strlen(directory) + 2);
	while (directory[j])
		j++;
	if (directory[j] == '\0')
	{
		directory[j] = '/';
		directory[j + 1] = '\0';
	}
	len = ft_strlen(directory) + ft_strlen(arg);
	binary_path = malloc(sizeof(char) * len + 1); // ici leaks
	if (!binary_path)
	{
		free(directory);
		free(binary_path);
		return (NULL);
	}
	binary_path = ft_strjoin(directory, arg); // ici leaks
	free(directory);
	return (binary_path);
}

/* Check each directory in PATH to find binary_path */

char	*find_binary_path(char *arg)
{
	char	*path_env;
	char	**split_path;
	char	*binary_path;
	int		i;

	path_env = getenv("PATH");
	split_path = ft_split(path_env, ':'); // ici leaks
	if (!split_path)
	{
		free(split_path);
		return (NULL);
	}
	i = 0;
	while (split_path && split_path[i])
	{
		binary_path = build_pathname(split_path[i], arg); // ici leak
		if (!access(binary_path, F_OK))
		{
			free(split_path);
			return (binary_path);
		}
		i++;
	}
	return (NULL);
}

static void	execute_child_process(char **args, char *binary_path)
{
	if (access(binary_path, X_OK) == -1)
		printf(ERR_CMD, args[0]);
	else if (execve(binary_path, args, NULL) == -1)
	{
		printf("Execve failed\n");
		exit(EXIT_FAILURE);
	}
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
		execute_child_process(args, binary_path);
	else if (pid > 0)
	{
		result = waitpid(pid, &status, 0);
		if (result == -1)
		{
			perror("waitpid failed");
			return ;
		}
		else if (!WIFEXITED(status))
		{
			printf("Error: child process ended weirdly\n"); // pas besoin du message d'erreur non ? 
			return ;	
		}
	}
}

void	execute_commands(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current;
	char	*binary_path;

	current = cmd;
	while (current)
	{
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
