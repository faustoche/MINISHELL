/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:19:10 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 17:50:42 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_bin_path(char **split_path, char *arg)
{
	int		i;
	char	*binary_path;
	char	*path_copy;

	i = 0;
	binary_path = NULL;
	while (split_path[i])
	{
		path_copy = ft_strdup(split_path[i]);
		if (!path_copy)
			return (NULL);
		binary_path = build_pathname(path_copy, arg);
		if (!access(binary_path, F_OK))
			return (binary_path);
		free(binary_path);
		i++;
	}
	return (binary_path);
}
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

char	*find_bin_path(char *arg, t_env *env_list)
{
	char	*path_env;
	char	**split_path;
	char	*binary_path;

	binary_path = NULL;
	path_env = find_var_value(env_list, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	split_path = ft_split(path_env, ':');
	if (!split_path)
		return (NULL);
	binary_path = create_bin_path(split_path, arg);
	if (binary_path == NULL)
	{
		free(split_path);
		free_split(split_path);
		return (NULL);
	}
	free_split(split_path);
	return (binary_path);
}

// char	*build_pathname(char *directory, char *arg)
// {
// 	char	*binary_path;
// 	char	*tmp;
// 	size_t	len;

// 	directory = ft_realloc(directory, ft_strlen(directory) + 2);
// 	if (!directory)
// 		return (NULL);
// 	if (directory[ft_strlen(directory) - 1] != '/')
// 	{
// 		tmp = directory;
// 		directory = ft_strjoin(directory, "/");
// 		free(tmp);
// 	}
// 	len = ft_strlen(directory) + ft_strlen(arg);
// 	binary_path = malloc(sizeof(char) * len + 1);
// 	if (!binary_path)
// 	{
// 		free(directory);
// 		return (NULL);
// 	}
// 	tmp = binary_path;
// 	binary_path = ft_strjoin(directory, arg);
// 	free(tmp);
// 	free(directory);
// 	return (binary_path);
// }

// /* Check each directory in PATH to find binary_path */

// char	*find_bin_path(char *arg, t_env *env_list)
// {
// 	char	*path_env;
// 	char	**split_path;
// 	char	*binary_path;
// 	char	*path_copy;
// 	int		i;

// 	path_env = find_var_value(env_list, "PATH");
// 	if (!path_env || path_env[0] == '\0')
// 		return (NULL);
// 	split_path = ft_split(path_env, ':');
// 	if (!split_path)
// 		return (NULL);
// 	i = 0;
// 	binary_path = NULL;
// 	while (split_path[i])
// 	{
// 		path_copy = ft_strdup(split_path[i]);
// 		if (!path_copy)
// 		{
// 			free_split(split_path);
// 			return (NULL);
// 		}
// 		binary_path = build_pathname(path_copy, arg);
// 		if (!access(binary_path, F_OK))
// 		{
// 			free_split(split_path);
// 			return (binary_path);
// 		}
// 		free(binary_path);
// 		i++;
// 	}
// 	free_split(split_path);
// 	return (NULL);
// }
