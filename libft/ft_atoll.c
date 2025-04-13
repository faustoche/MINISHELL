/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 19:25:51 by faustoche         #+#    #+#             */
/*   Updated: 2025/04/13 19:27:44 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_and_skip(const char *str, int *error)
{
	int i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (!ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')
	{
		*error = 1;
		return (0);
	}
	return (i);
}

static long long	parse_sign(const char *str, int *index)
{
	int	sign;

	sign = 1;
	if (str[*index] == '-')
		sign = -1;
	if (str[*index] == '-' || str[*index] == '+')
		(*index)++;
	return (sign);
}

static long long	handle_overflow(long long result, char digit, int sign, int *error)
{
	if ((result > LLONG_MAX / 10) || 
		(result == LLONG_MAX / 10 && (digit - '0') > LLONG_MAX % 10))
	{
		*error = 1;
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	return (-1);
}

long long	parse_digits(const char *str, int *i, int sign, int *error)
{
	long long	result;
	int			check;
	
	result = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		check = handle_overflow(result, str[*i], sign, error);
		if (*error == 1)
			return ((long long)check);
		result = result * 10 + (str[(*i)++] - '0');
	}
	return (result);
}

long long	ft_atoll(const char *str, int *error)
{
	long long	result = 0;
	int			i = 0;
	int			sign;

	*error = 0;
	i = check_and_skip(str, error);
	if (*error == 1)
		return (0);
	sign = parse_sign(str, &i);
	if (!ft_isdigit(str[i]))
	{
		*error = 1;
		return (0);
	}
	result = parse_digits(str, &i, sign, error);
	if (*error == 1)
		return result;
	if (str[i] != '\0')
	{
		*error = 1;
		return (0);
	}
	return (result * sign);
}
