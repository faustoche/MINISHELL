/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/15 18:31:32 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
- Why atoi ? Exit codes in linux are limited to 8 bits (0-255)
- If the user put a value that's out of range (ex: exit 1000)
- Shell has to bring back the value in the 0-255 range
- 1000 % 256 = 232 thus exit 1000 = exit 232
*/

static int	parse_exit_code(char *arg, long long *exit_code)
{
	int	error;

	error = 0;
	*exit_code = ft_atoll(arg, &error);
	if (error == 1)
		return (2);
	else
		return (0);
}

static void	exit_error_message(int error, char *arg)
{
	if (error == 1)
		printf("minislay : exit: too many arguments\n");
	else if (error == 2)
		printf("minislay : exit: %s: numeric argument required\n", arg);
}

static long long	process_exit_code(long long exit_code)
{
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

static long long	handle_exit_arguments(t_cmd *cmd)
{
	long long	exit_code;
	int			error;

	exit_code = 0;
	error = 0;
	if (cmd->args[1])
	{
		error = parse_exit_code(cmd->args[1], &exit_code);
		if (error == 2)
		{
			exit_error_message(error, cmd->args[1]);
			*(cmd->exit_status) = 2;
			exit(2);
		}
		else if (cmd->args[2])
		{
			exit_error_message(1, NULL);
			*(cmd->exit_status) = 1;
			return (-1);
		}
		else
			return (process_exit_code(exit_code));
	}
	return (*(cmd->exit_status));
}

void	ft_exit(t_cmd *cmd)
{
	long long	exit_code;

	printf("exit\n");
	exit_code = handle_exit_arguments(cmd);
	if (exit_code == -1)
		return ;
	*(cmd->exit_status) = exit_code;
	if (cmd->env_list)
		free_env_list(&cmd->env_list);
	quit_minislay(NULL, cmd, NULL, NULL);
	exit(exit_code);
}
