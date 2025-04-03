/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/03 18:18:12 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// lancement de ma boucle principale
// si je constTE qu'il y a d'autres commandes (ex env | grep) je cree un pipe
// je cree ensuite un enfant
// si c pas la premiere commande je reidigire l'entree
// close
// si j'ai une autre commande, je redirige
// close encore
// execution builtins
// execution command si pas builtins
// gestion du parent, je close quand j;ai fini
// je stock ce que je viens de lire pour la prochaine commande
// j'attend que tout le monde ai fini sqa popote waitpid
/*
À chaque itération, temp_cmd avance à temp_cmd->next avant que l'élément courant soit libéré.
Sans cette variable temporaire, si tu faisais cmd = cmd->next en essayant de parcourir la liste avec cmd directement,
tu perdrais l'adresse du début de la liste et ne pourrais pas la libérer correctement.
*/


int	has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

void execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current; 
	char	*binary_path;
	pid_t	pid;
	int		pipe_fd[2];
	int		input_fd;
	t_env	*old_env; // a voir
	t_cmd	*temp_cmd;
	t_cmd	*next;
	int		i;
	char	**split_path = NULL;

	input_fd = STDIN_FILENO;
	current = cmd;
	while (current)
	{
		if (current->next)
		{
			if (pipe(pipe_fd) == -1) // vu que j'ai une commande suivante, je crée un pipe
				print_error_message("Pipe failed\n");
		}
		pid = fork(); // création de mon enfant
		if (pid == -1)
		{
			perror("Fork creation failed\n");
			if (current->next)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			return ;
		}
		else if (pid == 0) // confirmation qu;on est bien dans l'enfant
		{
			if (input_fd != STDIN_FILENO) // si ce n'est pas la premiere commande
			{
				dup2(input_fd, STDIN_FILENO); // redirection de l'entree
				close(input_fd);
			}
			if (current->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO); // s'il y a une autre commande, reidrection
				close(pipe_fd[1]);
			}
			if (is_builtins(current->args[0]))
			{
				old_env = env_list;
				builtins_execution(current, &env_list);
				if (old_env != env_list)
					free_env_list(&old_env);
				free_env_list(&env_list);
				temp_cmd = cmd;
				while (temp_cmd) 
				{
					i = 0;
					next = temp_cmd->next;
					if (temp_cmd->args) 
					{
						while(temp_cmd->args[i])
						{
							free(temp_cmd->args[i]);
							i++;
						}
						free(temp_cmd->args);
					}
					free(temp_cmd);
					temp_cmd = next;
					close_all_fd(3);
				}
				exit(0);
			}
			else
			{
				binary_path = find_binary_path(current->args[0]);
				if (!binary_path)
				{
					printf(ERR_CMD, current->args[0]);
					free_env_list(&env_list);
					free_commands(cmd);
					exit(127);
				}
				if (execve(binary_path, current->args, NULL) == -1)
				{
					free(binary_path);
					free_env_list(&env_list);
					free_commands(cmd);
					exit(1);
				}
			}
			exit(1);
		}
		else
		{
			if (input_fd != STDIN_FILENO)
				close(input_fd); // on 
			if (current->next)
			{
				close(pipe_fd[1]);
				input_fd = pipe_fd[0];
			}
			current = current->next;
		}
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (split_path)
		free_split(split_path);
	while (wait(NULL) > 0);
	close_all_fd(3);
}
