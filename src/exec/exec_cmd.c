/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/03/08 15:32:19 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_pathname(char *arg)
{
	size_t	len;
	char 	*pathname;

	pathname = malloc(sizeof(char) * 6);
	if (!pathname)
		return (NULL);
	ft_strcpy(pathname, "/bin/");
	len = 6 + ft_strlen(arg);
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
				if (execve(pathname, current->args, NULL) == -1)
					perror("execve failed");
				exit(EXIT_FAILURE);
			}
			else //dans process parent, verifier ensuite comment s'est termine le fils
				waitpid(pid, NULL, 0);
		}
		current = current->next;
	}
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
