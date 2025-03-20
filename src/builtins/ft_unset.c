/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:04:05 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/20 15:04:16 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Step 4 : Gérer la suppression avec unset */

// Parcourir la liste pour trouver la variable à supprimé
// Retirer la variable de la liste et libérer la mémoire