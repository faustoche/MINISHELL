/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcrocq <fcrocq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:50:22 by fcrocq            #+#    #+#             */
/*   Updated: 2025/03/20 15:03:57 by fcrocq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Step 1 : Stocker les variables d'environnnement */

// Créer une structure pour stocker les variables (exp->env_list)
// Chaque variable contient un nom et une valeur + un pointeur vers l'élément suivant

// Initialiser la liste des variables d'environnement depuis envp
// Parcourir envp et extraire le nom et la valeur
// Insérer chaque variable dans la liste chaînée

// DONE DANS EXPAND


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
