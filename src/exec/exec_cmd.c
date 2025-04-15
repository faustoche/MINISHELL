/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:03 by ghieong           #+#    #+#             */
/*   Updated: 2025/04/15 09:28:25 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child_process(char **args, char *binary_path, t_env *env)
{
	char	**env_arr;

	env_arr = env_list_to_array(env);
	if (access(binary_path, X_OK) == -1)
	{
		printf(ERR_CMD, args[0]);
		exit(127);
	}
	else if (execve(binary_path, args, env_arr) == -1)
	{
		close_all_fd(3);
		free_env_array(env_arr);
		exit(127);
	}
	free_env_array(env_arr);
	close_all_fd(3);
}

/* Create child process and execute */

static void	create_child_process(char **args, char *bin, t_env *env, int *code)
{
	pid_t	pid;
	int		status;
	t_cmd	*last_cmd;
	int		last_status;
	pid_t	wait_pid;
	pid_t	last_pid;

	last_status = 0;
	last_cmd = get_last_cmd(env->cmd);
	if (last_cmd)
		last_pid = last_cmd->pid;
	else
		last_pid = -1;
	g_received_signal = 0;
	status = 0;
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
	{
		handle_signals(SIGINT, DEFAULT);
		handle_signals(SIGQUIT, DEFAULT);
		execute_child_process(args, bin, env);
	}
	else
	{
		handle_signals(SIGINT, IGNORE);
		while ((wait_pid = waitpid(pid, &status, WNOHANG)) == 0)
		{
		}
		handle_signals(SIGINT, PROMPT);
		if (wait_pid > 0)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			if (code)
				*code = last_status;
			if (wait_pid == last_pid && last_cmd && last_cmd->exit_status)
				*(last_cmd->exit_status) = last_status;
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			else if (WTERMSIG(status) == SIGINT)
				printf("\n");
		}
	}
	close_all_fd(3);
}

void	execute_commands(t_cmd *cmd, t_env *env)
{
	t_cmd	*cur;
	char	*bin;

	cur = cmd;
	while (cur)
	{
		if (is_builtins(cur->args[0]) && cur->args && cur->args[0])
			builtins_execution(cur, &env);
		else if (cur->args && cur->args[0])
		{
			if (cur->args[0][0] == '/')
			{
				bin = ft_strdup(cur->args[0]);
				if (!bin)
					return ;
				if (access(bin, F_OK) == -1)
				{
					*cur->exit_status = 127;
					printf(ERR_CMD, cur->args[0]);
					free(bin);
				}
				else
				{
					create_child_process(cur->args, bin, env, cur->exit_status);
					free(bin);
				}
			}
			else if ((cur->args[0][0] == '/') || (cur->args[0][0] == '.'
				&& cur->args[0][1] == '/'))
			{
				bin = ft_strdup(cur->args[0]);
				if (!bin)
					return ;
				if (access(bin, F_OK) == -1)
				{
					*cur->exit_status = 127;
					printf(ERR_DIR, cur->args[0]);
					free(bin);
				}
				else
				{
					create_child_process(cur->args, bin, env, cur->exit_status);
					free(bin);
				}
			}
			else
			{
				bin = find_bin_path(cur->args[0], env);
				if (bin == NULL)
				{
					printf(ERR_CMD, cur->args[0]);
					if (cur->exit_status)
						*(cur->exit_status) = 127;
					close_all_fd(3);
				}
				else
				{
					create_child_process(cur->args, bin, env, cur->exit_status);
					free(bin);
				}
			}
		}
		cur = cur->next;
	}
}

// void	create_child_process(char **args, char *binary, t_env *env, int *code)
// {
// 	pid_t				pid;
// 	int					status;
// 	t_cmd	*last_cmd;
// 	int		last_status = 0;
// 	pid_t	wait_pid;

// 	last_cmd = get_last_cmd(env->cmd);
// 	pid_t	last_pid = (last_cmd) ? last_cmd->pid : -1;
// 	g_received_signal = 0;
// 	status = 0;
// 	pid = fork();
// 	if (pid == -1)
// 		return ;
// 	else if (pid == 0)
// 	{
// 		dprintf(2, "line = %d, file %s\n", __LINE__, __FILE__);
// 		if (handle_signals(SIGINT, DEFAULT) == -1)
// 			return ;
// 		if (handle_signals(SIGQUIT, DEFAULT) == -1)
// 			return ;
// 		execute_child_process(args, binary_path, env);
// 	}
// 	else if (pid > 0)
// 	{
// 		dprintf(2, "line = %d, file %s\n", __LINE__, __FILE__);
// 		if (handle_signals(SIGINT, CHILD_PROMPT) == -1)
// 			return ;
// 		while ((wait_pid = waitpid(pid, &status, 0)) > 0)
// 		{
// 			if (WIFEXITED(status))
// 				last_status = WEXITSTATUS(status);
// 			else if (WIFSIGNALED(status))
// 				last_status = 128 + WTERMSIG(status);
// 			if (code)
// 				*code = last_status;
// 			if (wait_pid == last_pid && last_cmd && last_cmd->exit_status)
// 				*(last_cmd->exit_status) = last_status;
// 		}
// 		if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == SIGQUIT)
// 			printf("Quit (core dumped)\n");
// 		}
// 	}
// 	close_all_fd(3);
// }
