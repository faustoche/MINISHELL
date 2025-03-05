/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:25:23 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/05 16:31:48 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *expand_tilde(char *input)
{
    char    *home;
    char    *result;

    if (!input || input[0] != '~')
        return (ft_strdup(input));
    home = getenv("HOME");
    if (!home)
        return (ft_strdup(input));
    result = malloc(ft_strlen(home) + ft_strlen(input));
    if (!result)
        return (NULL);
    ft_strcpy(result, home);
    strcat(result, input + 1); // mettre le vrai depuis la libft
    return (result);
}


/*
À prendre en compte selon bash : 

> $USER : renvois fcrocq
> '$USER' : renvois $USER
> "$USER" : renvois fcrocq
> '"$USER"' : renvois "$USER"
> "'$USER'" : renvois 'fcrocq'


*/