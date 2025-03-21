/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faustoche <faustoche@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:50:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/21 17:46:26 by faustoche        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Step 2 : Gérer la commande export */

// Parser l'entrée utilisateur 
// Vérifier si l'utilisateur tape export sans argument : afficher toutes les variables triées
// Sinon extraire key=value des arguments fournis

// Ajouter ou mettre à jour une variable
// Si la variable existe déjà : mettre à jour sa valeur
// Sinon : créer une nouvelle variable et l'ajouter à la liste

// Gérer les cas spécifiques
// export VAR= définir VAR avec une valeur vide

/* Step 3 : Afficher une nouvelle ligne (export sans ARGUMENT)*/

// Afficher une nouvelle ligne
// Ne pas afficher les variables supprimées (si unset a été utilisé)
