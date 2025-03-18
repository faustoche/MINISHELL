/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 09:51:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/18 16:48:38 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void execute_redirections(t_cmd *cmd)
// {
//     pid_t pid;
//     int status;
//     int fd_in = -1;
//     int fd_out = -1;
//     char *binary_path = NULL;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("Fork failed");
//         return;
//     }
    
//     if (pid == 0) // Dans le processus fils
//     {
//         // Gérer les redirections d'entrée
//         if (cmd->in)
//         {
//             fd_in = open_file(cmd->in, REDIR_IN);
//             redirect_in(fd_in);
//         }
//         else if (cmd->heredoc != -1)
//         {
//             redirect_in(cmd->heredoc);
//         }
        
//         // Gérer les redirections de sortie
//         if (cmd->out)
//         {
//             if (cmd->append)
//                 fd_out = open_file(cmd->out, REDIR_APPEND);
//             else
//                 fd_out = open_file(cmd->out, REDIR_OUT);
//             redirect_out(fd_out);
//         }
        
//         // Exécuter la commande
//         if (cmd->args && cmd->args[0])
//         {
//             if (is_builtins(cmd->args[0]))
//             {
//                 builtins_execution(cmd);
//                 exit(EXIT_SUCCESS);
//             }
//             else
//             {
//                 binary_path = find_binary_path(cmd->args[0]);
//                 if (!binary_path)
//                 {
//                     printf(ERR_CMD, cmd->args[0]);
//                     exit(EXIT_FAILURE);
//                 }
                
//                 if (access(binary_path, X_OK) == -1)
//                 {
//                     printf(ERR_CMD, cmd->args[0]);
//                     free(binary_path);
//                     exit(EXIT_FAILURE);
//                 }
                
//                 if (execve(binary_path, cmd->args, NULL) == -1)
//                 {
//                     perror("Execve failed");
//                     free(binary_path);
//                     exit(EXIT_FAILURE);
//                 }
//             }
//         }
//         exit(EXIT_SUCCESS);
//     }
//     else // Dans le processus parent
//     {
//         if (waitpid(pid, &status, 0) == -1)
//         {
//             perror("waitpid failed");
//             return;
//         }
//     }
// }

/* Ouvre le fichier en fonction de la redirection */

int	open_file(char *filename, int token)
{
	int	fd;

	fd = 0;
	if (token == 4)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (token == 6)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (token == 5)
		fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("No such or directory : %s\n", filename);
		exit (EXIT_FAILURE);
	}
	return (fd);
}

void	redirect(int fd, int std_fd)
{
	if (dup2(fd, std_fd) == -1)
		perror("error : dup2 failed\n");
	close(fd);
}

// /* RRedirige la sortie standard vers un fichier */

// void	redirect_out(int fd)
// {
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 		perror("dup2 failed for out");
// 	close(fd);
// }

// /* Redirige l'entrée standard depuis un fichier */

// void	redirect_in(int fd)
// {
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 		perror("dup2 failed for in");
// 	close(fd);
// }

/* According to the token, redirect in the right direction */

int	handle_redirection(t_token *token, t_cmd *current, t_cmd *head)
{
	if (!current || !token->next)
	{
		printf("Error: invalid redirection\n");
		free_commands(head);
		return (-1);
	}
	if (token->type == REDIR_OUT)
		return (redirection(current, token->next->value, 1, 0));
	else if (token->type == REDIR_IN)
		return (redirection(current, token->next->value, 0, 0));
	else if (token->type == REDIR_APPEND)
		return (redirection(current, token->next->value, 1, 1));
	else if (token->type == HEREDOC)
		return (handle_heredoc(current, token->next->value, head));
	return (0);
}

int	redirection(t_cmd *cmd, char *file, int out, int append)
{
	if (!cmd || !file)
		return (-1);
	if (out)
	{
		if (cmd->out)
			free(cmd->out);
		cmd->out = ft_strdup(file);
		if (!cmd->out)
			return (-1);
		cmd->append = append;
	}
	else
	{
		if (cmd->in)
			free(cmd->in);
		cmd->in = ft_strdup(file);
		if (!cmd->in)
			return (-1);
	}
	return (0);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter, t_cmd *head)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd || !delimiter)
	{
		printf("Error : invalid heredoc\n");
		free_commands(head);
		return (-1);
	}
	if (pipe(pipe_fd) == -1)
	{
		printf("Error : pipe creation failed\n");
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	cmd->heredoc = pipe_fd[0];
	return (0);
}
