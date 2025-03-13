/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/03/12 18:10:41 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// recuperer PATH (getenv)
// split la chaine
// while tab non vide, parcourir le tableau et recup la chaine puis concatener avec arg
// checker si executable trouve (if access == 0)
// si trouve, sort de la boucle et continue avec execve
// si trouve nulle part, print ERR_CMD

// char	*find_pathname(char *arg)
// {
// 	char	*path_str;
// 	char	**split_path;
// 	char	*pathname;
// 	char	*truc;
// 	int		i;
// 	size_t	len_one;
// 	size_t	len_two;

// 	i = 0;
// 	truc = NULL;
// 	path_str = getenv("PATH");
// 	split_path = ft_split(path_str, ':');
// 	if (!split_path)
// 		return (NULL);
// 	while (split_path && split_path[i])
// 	{
// 		len_two = ft_strlen(arg) + ft_strlen(split_path[i]) + 3;
// 		pathname = malloc(sizeof(char) * len_two);
// 		if (!pathname)
// 			return (NULL);
// 		ft_strcpy(pathname, split_path[i]);
// 		ft_strcpy(truc, "/");
// 		len_one = ft_strlen(arg) + ft_strlen(truc) + 1;
// 		ft_strlcat(arg, truc, len_one);
// 		ft_strlcat(pathname, arg, len_two);
// 		if (!access(pathname, F_OK))
// 			break ;
// 		i++;
// 	}
// 	free_tokens(split_path);
// 	return (pathname);
// }

char	*find_pathname(char *arg)
{
	size_t	len;
	char 	*pathname;

	len = 6 + ft_strlen(arg);
	pathname = malloc(sizeof(char) * len);
	if (!pathname)
		return (NULL);
	ft_strcpy(pathname, "/bin/");
	ft_strlcat(pathname, arg, len);
	return (pathname);
}

void	execute_commands(t_cmd *cmd)
{
	t_cmd	*current;
	int		pid;
	char	*pathname;

	current = cmd;
	pathname = find_pathname(current->args[0]);
	printf("%s\n", pathname);
	while (current)
	{
		if (current->args && current->args[0] && is_builtins(current->args[0]))
			builtins_execution(current);
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("Fork failed\n");
				exit(EXIT_FAILURE); //free et return
			}
         	if (pid == 0) //dans process fils
			{
				if (access(pathname, X_OK) == -1)
					printf(ERR_CMD, current->args[0]);
				else if (execve(pathname, current->args, NULL) == -1)
				{
					perror("execve failed"); // quelle gestion erreur ici?
					exit(EXIT_FAILURE);
				}
			}
			else //dans process parent, verifier ensuite comment s'est termine le fils
				waitpid(pid, NULL, 0);
		}
		current = current->next;
	}
	free(pathname);
	free(current);
}

// void	execute_commands(t_cmd *cmd)
// {
// 	t_cmd	*current;

// 	current = cmd;
// 	while (current)
// 	{
// 		if (current->args && current->args[0] && is_builtins(current->args[0]))
// 			builtins_execution(current);
// 		else
// 		{
// 			if (current->args && current->args[0]) // ici execve a la place du if else
// 				printf(ERR_CMD, current->args[0]);
// 			else
// 				printf("Empty command\n");
//                 printf(ERR_CMD, current->args[0]);
// 		}
// 		current = current->next;
// 	}
// }

