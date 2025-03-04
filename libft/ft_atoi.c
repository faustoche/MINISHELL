/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:55:47 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:45:12 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La fonction atoi() convertit le début de la chaîne pointée par nptr 
en entier de type int . Le résultat est identique à un appel */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		sign;
	long	r;

	r = 0;
	sign = 1;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
	{
		nptr++;
	}
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
		{
			sign = -1;
		}
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		r = r * 10 + *nptr - '0';
		nptr++;
	}
	return (sign * (int)r);
}
/*
int	main(void)
{
	char a[] = "\n\n\v\f\r\t-5234AAAAAvfvjdfi]\[;vodfjv";
	printf("%d\n", ft_atoi(a));
}*/
