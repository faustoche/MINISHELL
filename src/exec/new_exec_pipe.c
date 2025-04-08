/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exec_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/08 16:48:55 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_pipes(t_cmd *cmd)
{
	if (cmd && cmd->next)
		return (1);
	return (0);
}

void execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current;
	int		fd;
	char	*binary_path;
	pid_t	pid;
	int		pipe_fd[2];
	int		input_fd;
	t_cmd	*temp_cmd;
	t_cmd	*next;
	int		i;
	char	**split_path = NULL;
	int		status;
	pid_t	wait_pid;

	input_fd = STDIN_FILENO;
	current = cmd;
	while (current)
	{
		if (current->next)
		{
			if (pipe(pipe_fd) == -1)
				print_error_message("Pipe failed\n");
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Fork creation failed\n");
			if (current->next)
				(close(pipe_fd[0]), close(pipe_fd[1]));
			return ;
		}
		else if (pid == 0)
		{
			if (input_fd != STDIN_FILENO)
				(dup2(input_fd, STDIN_FILENO), close(input_fd));
			if (current->in)
			{
				fd = open_file(current->in, REDIR_IN);
				if (fd != -1)
					(dup2(fd, STDIN_FILENO), close (fd));
			}
			if (current->heredoc != -1)
				(dup2(current->heredoc, STDIN_FILENO), close(current->heredoc));
			if (current->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO),
				close(pipe_fd[1]);
			}
			if (current->out)
			{
				if (current->append)
					fd = open_file(current->out, REDIR_APPEND);
				else
					fd = open_file(current->out, REDIR_OUT);
				if (fd != -1)
				{
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
			}
			if (is_builtins(current->args[0]))
			{
				if (current->next)
					(dup2(pipe_fd[1], STDOUT_FILENO), close(pipe_fd[1]));
				builtins_execution(current, &env_list);
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
				char **env = env_list_to_array(env_list);
				binary_path = find_binary_path(current->args[0]);
				if (!binary_path)
				{
					(printf(ERR_CMD, current->args[0]), free_env_array(env));
					(free_env_list(&env_list), free_commands(cmd), exit(127));
				}
				if (execve(binary_path, current->args, env) == -1)
				{
					(free(binary_path), free_env_array(env));
					(free_env_list(&env_list), free_commands(cmd), exit(1));
				}
			}
			exit(1);
		}
		else
		{
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			if (current->next)
			{
				close(pipe_fd[1]);
				input_fd = pipe_fd[0];
			}
			if (current->heredoc != -1)
				close(current->heredoc);
			current = current->next;
		}
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (split_path)
		free_split(split_path);
	while ((wait_pid = waitpid(-1, &status, 0)) > 0);
	close_all_fd(3);
}

// void execute_pipeline(t_cmd *cmd, t_env *env_list)
// {
//     t_cmd *current;
//     int pipe_fd[2];
//     int input_fd;
//     pid_t pid;
//     current = cmd;
//     while (current) {
//         if (current->heredoc != -1) {
//             int tmp_fd = open("/tmp/heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//             if (tmp_fd != -1) {
//                 char buffer[4096];
//                 ssize_t bytes_read;
//                 while ((bytes_read = read(current->heredoc, buffer, sizeof(buffer))) > 0) {
//                     write(tmp_fd, buffer, bytes_read);
//                 }
//                 close(tmp_fd);
//                 close(current->heredoc);
//                 // Réouvrir le fichier temporaire
//                 current->heredoc = open("/tmp/heredoc_tmp", O_RDONLY);
//             }
//         }
//         current = current->next;
//     }
//     input_fd = STDIN_FILENO;
//     current = cmd;
//     while (current)
//     {
//         // Créer un pipe si nécessaire
//         if (current->next) {
//             if (pipe(pipe_fd) == -1) {
//                 perror("pipe failed");
//                 return;
//             }
//         }
//         // Créer un processus
//         pid = fork();
//         if (pid == -1) {
//             perror("fork failed");
//             if (current->next) {
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }
//             return;
//         }
//         if (pid == 0) { // Processus enfant
//             // Configurer l'entrée
//             if (input_fd != STDIN_FILENO) {
//                 dup2(input_fd, STDIN_FILENO);
//                 close(input_fd);
//             }
//             // Redirection d'entrée
//             if (current->heredoc != -1) {
//                 dup2(current->heredoc, STDIN_FILENO);
//                 close(current->heredoc);
//             } else if (current->in) {
//                 int fd = open_file(current->in, REDIR_IN);
//                 if (fd != -1) {
//                     dup2(fd, STDIN_FILENO);
//                     close(fd);
//                 }
//             }
//             // Configurer la sortie
//             if (current->next) {
//                 close(pipe_fd[0]);
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[1]);
//             }
//             // Redirection de sortie
//             if (current->out) {
//                 int fd;
//                 if (current->append)
//                     fd = open_file(current->out, REDIR_APPEND);
//                 else
//                     fd = open_file(current->out, REDIR_OUT);
//                 if (fd != -1) {
//                     dup2(fd, STDOUT_FILENO);
//                     close(fd);
//                 }
//             }
//             // Exécuter la commande
//             if (is_builtins(current->args[0])) {
//                 builtins_execution(current, &env_list);
//                 exit(0);
//             } else {
//                 char **env = env_list_to_array(env_list);
//                 char *binary_path = find_binary_path(current->args[0]);
//                 if (!binary_path) {
//                     printf(ERR_CMD, current->args[0]);
//                     if (env) free_env_array(env);
//                     exit(127);
//                 }
//                 execve(binary_path, current->args, env);
//                 perror("execve failed");
//                 free(binary_path);
//                 if (env) free_env_array(env);
//                 exit(1);
//             }
//         } else { // Processus parent
//             // Fermer les descripteurs de fichiers
//             if (input_fd != STDIN_FILENO)
//                 close(input_fd);
//             if (current->heredoc != -1) {
//                 close(current->heredoc);
//                 current->heredoc = -1;
//             }
//             if (current->next) {
//                 close(pipe_fd[1]);
//                 input_fd = pipe_fd[0];
//             }
//             // Passer à la commande suivante
//             current = current->next;
//         }
//     }
//     // Attendre que tous les processus se terminent
//     while (wait(NULL) > 0);
//     // Nettoyer
//     unlink("/tmp/heredoc_tmp");
//     close_all_fd(3);
// }