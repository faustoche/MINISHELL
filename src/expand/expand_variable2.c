/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:54:53 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/11 14:48:50 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_variable_part1(t_expand *exp)
{
	if (exp->i > 0 && exp->str[exp->i - 1] == '"'
		&& exp->str[exp->i] == '$' && exp->str[exp->i + 1] == '"')
	{
		exp->result[exp->j++] = '$';
		exp->i++;
		if (exp->str[exp->i] == '"')
			exp->i++;
		return (1);
	}
	return (0);
}

int	process_variable_part2(t_expand *exp)
{
	int	temp_i;

	if (exp->i > 0 && exp->str[exp->i - 1] == '"' && exp->str[exp->i] == '$')
	{
		temp_i = exp->i + 1;
		while (exp->str[temp_i] && exp->str[temp_i] != ' '
			&& exp->str[temp_i] != '"' && exp->str[temp_i] != '\'')
			temp_i++;
		if (exp->str[temp_i] == '"' && exp->str[temp_i + 1]
			&& !is_space(exp->str[temp_i + 1]) && exp->str[temp_i + 1] != '\0')
		{
			exp->result[exp->j++] = exp->str[exp->i - 1];
			exp->result[exp->j++] = exp->str[exp->i++];
			return (1);
		}
	}
	return (0);
}

int	process_variable_part3(t_expand *exp)
{
	char	quote;
	size_t	start;

	if (exp->str[exp->i] == '$' && exp->i + 1 < ft_strlen(exp->str)
		&& (exp->str[exp->i + 1] == '"' || exp->str[exp->i + 1] == '\''))
	{
		quote = exp->str[exp->i + 1];
		exp->i += 2;
		start = exp->i;
		while (exp->str[exp->i] && exp->str[exp->i] != quote)
			exp->i++;
		if (start < exp->i)
			copy_str_to_result(exp, exp->str + start, exp->i - start);
		if (exp->str[exp->i] == quote)
			exp->i++;
		return (1);
	}
	return (0);
}

int	process_variable_part4(t_expand *exp)
{
	if (!exp->str[exp->i + 1])
	{
		exp->result[exp->j++] = '$';
		exp->i++;
		return (1);
	}
	return (0);
}

int	process_variable_part5(t_expand *exp)
{
	char	exit_status_str[16];
	int		exit_code;
	int		tmp;
	int		len;

	exp->i++;
	// debut
	if (exp->str[exp->i] == '?') // je checke deja $ avant
	{
		exit_code = 0; // 0 par defualt
		if (exp->cmd && exp->cmd->exit_status) // si le status est superieur a 0 (est=ce qu'il y q une limite?)
			exit_code = *(exp->cmd->exit_status);
		if (exit_code == 0)
			ft_strcpy(exit_status_str, "0"); // si je recois 0 alors je defini 0
		else
		{
			tmp = exit_code;
			len = 0;
			while (tmp > 0) // calcul de la longue de la chaine (genre 2 5 6 )
			{
				tmp /= 10;
				len++;
			}
			exit_status_str[len] = '\0'; // nul a la fin
			tmp = exit_code;
			while (len > 0)
			{
				exit_status_str[--len] = (tmp % 10) + '0'; // ti atoi des mif
				tmp /= 10;
			}
		}
		if (!copy_str_to_result(exp, exit_status_str, ft_strlen(exit_status_str))) // je copie le resultat final
			return (0);
		exp->i++; // j'incremonte
		return (1);
		// fin
	}
	if (exp->str[exp->i] != '{' && !isalnum(exp->str[exp->i])
		&& exp->str[exp->i] != '_'
		&& exp->str[exp->i] != '"' && exp->str[exp->i] != '\''
		&& exp->str[exp->i] != '`')
	{
		exp->result[exp->j++] = '$';
		return (1);
	}
	return (0);
}
