/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:43:25 by faustoche         #+#    #+#             */
/*   Updated: 2024/11/27 18:43:27 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
	{
		return (1);
	}
	return (0);
}
/*
int	main(void)
{
	printf("%d\n", ft_isalnum('H'));
	printf("%d\n", ft_isalnum('8'));
	printf("%d\n", ft_isalnum('j'));
	printf("%d\n", ft_isalnum(';'));
	printf("%d\n", ft_isalnum('!'));
}*/