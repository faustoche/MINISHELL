/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:14:36 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/13 20:46:29 by faustoche        ###   ########.fr       */
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
	int 		error;

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
			return -1;
		}
		else
			return process_exit_code(exit_code);
	}
	return *(cmd->exit_status);
}

void	ft_exit(t_cmd *cmd)
{
	long long	exit_code;

	printf("exit\n");
	exit_code = handle_exit_arguments(cmd);
	if (exit_code == -1)
		return;
	*(cmd->exit_status) = exit_code;
	quit_minislay(NULL, cmd, NULL, NULL);
	exit(exit_code);
}

// void	ft_exit(t_cmd *cmd)
// {
// 	long long		exit_code;
// 	int				error;
	
// 	exit_code = 0;
// 	printf("exit\n");
// 	if (cmd->args[1])
// 	{
// 		exit_code = ft_atoll(cmd->args[1], &error);
// 		if (error)
// 		{
// 			printf("minislay : exit: %s: numeric argument required\n", cmd->args[1]);
// 			*(cmd->exit_status) = 2; // verifier si 2
// 			exit(2);
// 		}
// 		else if (cmd->args[2])
// 		{
// 			printf("minislay : exit: too many arguments\n");
// 			*(cmd->exit_status) = 1; // verifier 1
// 			return ;
// 		}
// 		else
// 		{
// 			exit_code = exit_code % 256;
// 			if (exit_code < 0)
// 				exit_code += 256;
// 		}
// 	}
// 	else
// 		exit_code = *(cmd->exit_status); // ici necessaire ?
// 	*(cmd->exit_status) = exit_code;
// 	quit_minislay(NULL, cmd, NULL, NULL);
// 	exit(exit_code);
// }



// long long	ft_atoll(const char *str, int *error)
// {
// 	long long	result;
// 	int			i;
// 	int			sign;
	
// 	*error = 0;
// 	i = 0;
// 	result = 0;
// 	sign = 1;
// 	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
// 		i++;
// 	if (str[i] == '-')
// 		sign = -1;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	if (ft_isdigit(str[i] == 1))
// 	{
// 		*error = 1;
// 		return (0);
// 	}
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		if ((result > LLONG_MAX / 10) || (result == LLONG_MAX / 10
// 			&& (str[i] - '0') > LLONG_MAX % 10))
// 		{
// 			*error = 1;
// 			if (sign == 1)
// 				return (LLONG_MAX);
// 			else
// 				return (LLONG_MIN);
// 		}
// 		result = result * 10 + (str[i++] - '0');
// 	}
// 	if (str[i] != '\0')
// 	{
// 		*error = 1;
// 		return (0);
// 	}
// 	return (result * sign);
// }
