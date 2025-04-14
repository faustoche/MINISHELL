/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:20:54 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/14 14:34:00 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	int_to_str(int n, char *str)
{
	int	len;
	int	tmp;

	len = 0;
	tmp = n;
	if (n == 0)
	{
		ft_strcpy(str, "0");
		return ;
	}
	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
	str[len] = '\0';
	while (len > 0)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
}

int	handle_exit_status(t_expand *exp, int *code)
{
	char	exit_status_str[16];
	int		exit_code;

	if (code)
		exit_code = *code;
	else
		exit_code = 0;
	int_to_str(exit_code, exit_status_str);
	if (!copy_str_to_result(exp, exit_status_str, ft_strlen(exit_status_str)))
		return (0);
	exp->i++;
	return (1);
}

int	is_valid_variable_char(char c)
{
	return (isalnum(c) || c == '_' || c == '{'
		|| c == '"' || c == '\'' || c == '`');
}

int	process_variable_part5(t_expand *exp, int *code)
{
	exp->i++;
	if (exp->str[exp->i] == '?')
		return (handle_exit_status(exp, code));
	else if (!is_valid_variable_char(exp->str[exp->i]))
	{
		exp->result[exp->j++] = '$';
		return (1);
	}
	return (0);
}

// int	process_variable_part5(t_expand *exp, int *code)
// {
// 	char	exit_status_str[16];
// 	int		exit_code;
// 	int		tmp;
// 	int		len;

// 	exp->i++;
// 	exit_code = 0;
// 	if (exp->str[exp->i] == '?')
// 	{	
// 		if (code)
// 			exit_code = *code;
// 		if (exit_code == 0)
// 			ft_strcpy(exit_status_str, "0");
// 		else
// 		{
// 			tmp = exit_code;
// 			len = 0;
// 			while (tmp > 0)
// 			{
// 				tmp /= 10;
// 				len++;
// 			}
// 			exit_status_str[len] = '\0';
// 			tmp = exit_code;
// 			while (len > 0)
// 			{
// 				exit_status_str[--len] = (tmp % 10) + '0';
// 				tmp /= 10;
// 			}
// 		}
// 		if (!copy_str_to_result(exp, exit_status_str,strlen(exit_status_str)))
// 			return (0);
// 		exp->i++;
// 		return (1);
// 	}

// 	if (exp->str[exp->i] != '{' && !isalnum(exp->str[exp->i])
// 		&& exp->str[exp->i] != '_'
// 		&& exp->str[exp->i] != '"' && exp->str[exp->i] != '\''
// 		&& exp->str[exp->i] != '`')
// 	{
// 		exp->result[exp->j++] = '$';
// 		return (1);
// 	}
// 	return (0);
// }
