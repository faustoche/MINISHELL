/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:59:17 by fcrocq            #+#    #+#             */
/*   Updated: 2025/04/11 21:55:00 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_atoll(const char *str, int *error)
{
	long long	result;
	int			i;
	int			sign;
	
	*error = 0;
	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (ft_isdigit(str[i] == 1))
	{
		*error = 1;
		return (0);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((result > LLONG_MAX / 10) || (result == LLONG_MAX / 10
			&& (str[i] - '0') > LLONG_MAX % 10))
		{
			*error = 1;
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		result = result * 10 + (str[i++] - '0');
	}
	if (str[i] != '\0')
	{
		*error = 1;
		return (0);
	}
	return (result * sign);
}
