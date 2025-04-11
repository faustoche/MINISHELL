/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 20:05:00 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/11 18:49:12 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

static t_pipe	init_pipe_struct(t_cmd *cmd, t_env **env_list)
{
	t_pipe	pipe_data;

	pipe_data.cmd = cmd;
	pipe_data.env_list = env_list;
	pipe_data.input_fd = STDIN_FILENO;
	pipe_data.pipe_fd[0] = -1;
	pipe_data.pipe_fd[1] = -1;
	return (pipe_data);
}

static void	pipe_child_process(t_cmd *current, t_pipe *pipe_data)
{
	if (handle_signals(SIGINT, DEFAULT) == -1)
		return ;
	if (handle_signals(SIGQUIT, DEFAULT) == -1)
		return ;
	in_redirection(current, pipe_data->input_fd);
	out_redirection(current, pipe_data);
	if (current->args)
	{
		if (is_builtins(current->args[0]))
			pipe_builtin(current, pipe_data);
		else
			pipe_execve(current, pipe_data);
	}
	else
		free_pipe(pipe_data->cmd, *(pipe_data->env_list), NULL);
	exit(1);
}

static void	pipe_parent_process(t_cmd **current, t_pipe *pipe_data)
{
	if (handle_signals(SIGINT, WESH) == -1)
		return ;
	if (pipe_data->input_fd != STDIN_FILENO)
		close(pipe_data->input_fd);
	if ((*current)->next)
	{
		close(pipe_data->pipe_fd[1]);
		pipe_data->input_fd = pipe_data->pipe_fd[0];
	}
	if ((*current)->heredoc != -1)
		close((*current)->heredoc);
	*current = (*current)->next;
}

static void	cleanup_and_wait(t_pipe *pipe_data, char **split_path)
{
	int		status;
	pid_t	wait_pid;
	t_cmd	*last_cmd;
	int		last_status = 0;

	last_cmd = get_last_cmd(pipe_data->cmd);
	pid_t	last_pid = (last_cmd) ? last_cmd->pid : -1;
	status = 0;
	if (pipe_data->input_fd != STDIN_FILENO)
		close(pipe_data->input_fd);
	if (split_path) // inutile?
		free_split(split_path);
	while ((wait_pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		if (wait_pid == last_pid && last_cmd && last_cmd->exit_status)
			*(last_cmd->exit_status) = last_status;
	}
	if (last_cmd && last_cmd->exit_status && *(last_cmd->exit_status) == 0)
		*(last_cmd->exit_status) = last_status;
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		printf("Quit (core dumped)\n");
	}
	close_all_fd(3);
}

void	execute_pipeline(t_cmd *cmd, t_env *env_list)
{
	t_cmd	*current;
	pid_t	pid;
	t_pipe	pipe_data;
	char	**split_path; // inutile?


	split_path = NULL;
	pipe_data = init_pipe_struct(cmd, &env_list);
	current = cmd;
	while (current)
	{
		if (current->next && !setup_pipe(&pipe_data))
			return ;
		pid = fork();
		if (pid == -1)
		{
			perror("Fork creation failed\n");
			if (current->next)
				(close(pipe_data.pipe_fd[0]), close(pipe_data.pipe_fd[1]));
			return ;
		}
		else if (pid == 0)
			pipe_child_process(current, &pipe_data);
		else
		{
			current->pid = pid;
			pipe_parent_process(&current, &pipe_data);
		}
	}
	cleanup_and_wait(&pipe_data, split_path);
}

// void execute_pipeline(t_cmd *cmd, t_env *env_list)
// {
// 	t_cmd	*current;
// 	int		fd;
// 	char	*binary_path;
// 	pid_t	pid;
// 	int		pipe_fd[2];
// 	int		input_fd;
// 	t_cmd	*temp_cmd;
// 	t_cmd	*next;
// 	int		i;
// 	char	**split_path = NULL;
// 	int		status;
// 	pid_t	wait_pid;

// 	input_fd = STDIN_FILENO;
// 	current = cmd;
// 	while (current)
// 	{
// 		if (current->next)
// 		{
// 			if (pipe(pipe_fd) == -1)
// 				print_error_message("Pipe failed\n");
// 		}
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("Fork creation failed\n");
// 			if (current->next)
// 				(close(pipe_fd[0]), close(pipe_fd[1]));
// 			return ;
// 		}
// 		else if (pid == 0)
// 		{
// 			if (input_fd != STDIN_FILENO)
// 				(dup2(input_fd, STDIN_FILENO), close(input_fd));
// 			if (current->in)
// 			{
// 				fd = open_file(current->in, REDIR_IN);
// 				if (fd != -1)
// 					(dup2(fd, STDIN_FILENO), close (fd));
// 			}
// 			if (current->heredoc != -1)
// 				(dup2(current->heredoc, STDIN_FILENO), close(current->heredoc));
// 			if (current->next)
// 			{
// 				close(pipe_fd[0]);
// 				dup2(pipe_fd[1], STDOUT_FILENO),
// 				close(pipe_fd[1]);
// 			}
// 			if (current->out)
// 			{
// 				if (current->append)
// 					fd = open_file(current->out, REDIR_APPEND);
// 				else
// 					fd = open_file(current->out, REDIR_OUT);
// 				if (fd != -1)
// 				{
// 					dup2(fd, STDOUT_FILENO);
// 					close(fd);
// 				}
// 			}
// 			if (is_builtins(current->args[0]))
// 			{
// 				if (current->next)
// 					(dup2(pipe_fd[1], STDOUT_FILENO), close(pipe_fd[1]));
// 				builtins_execution(current, &env_list);
// 				free_env_list(&env_list);
// 				temp_cmd = cmd;
// 				while (temp_cmd) 
// 				{
// 					i = 0;
// 					next = temp_cmd->next;
// 					if (temp_cmd->args) 
// 					{
// 						while(temp_cmd->args[i])
// 						{
// 							free(temp_cmd->args[i]);
// 							i++;
// 						}
// 						free(temp_cmd->args);
// 					}
// 					free(temp_cmd);
// 					temp_cmd = next;
// 					close_all_fd(3);
// 				}
// 				exit(0);
// 			}
// 			else
// 			{
// 				char **env = env_list_to_array(env_list);
// 				binary_path = find_binary_path(current->args[0]);
// 				if (!binary_path)
// 				{
// 					(printf(ERR_CMD, current->args[0]), free_env_array(env));
// 					(free_env_list(&env_list), free_commands(cmd), exit(127));
// 				}
// 				if (execve(binary_path, current->args, env) == -1)
// 				{
// 					(free(binary_path), free_env_array(env));
// 					(free_env_list(&env_list), free_commands(cmd), exit(1));
// 				}
// 			}
// 			exit(1);
// 		}
// 		else
// 		{
// 			if (input_fd != STDIN_FILENO)
// 				close(input_fd);
// 			if (current->next)
// 			{
// 				close(pipe_fd[1]);
// 				input_fd = pipe_fd[0];
// 			}
// 			if (current->heredoc != -1)
// 				close(current->heredoc);
// 			current = current->next;
// 		}
// 	}
// 	if (input_fd != STDIN_FILENO)
// 		close(input_fd);
// 	if (split_path)
// 		free_split(split_path);
// 	while ((wait_pid = waitpid(-1, &status, 0)) > 0);
// 	close_all_fd(3);
// 	set_signal_handlers();
// }
