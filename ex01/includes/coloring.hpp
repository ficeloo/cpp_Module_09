/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:50:30 by tcros             #+#    #+#             */
/*   Updated: 2026/02/17 18:57:49 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORING_HPP
# define COLORING_HPP

# include <iostream>

// Réinitialisation
# define RESET       "\033[0m"

// Styles
# define BOLD        "\033[1m"
# define DIM         "\033[2m"
# define ITALIC      "\033[3m"
# define UNDERLINE   "\033[4m"
# define BLINK       "\033[5m"
# define REVERSE     "\033[7m"

// Couleurs de texte
# define BLACK       "\033[30m"
# define RED         "\033[31m"
# define GREEN       "\033[32m"
# define YELLOW      "\033[33m"
# define BLUE        "\033[34m"
# define MAGENTA     "\033[35m"
# define CYAN        "\033[36m"
# define WHITE       "\033[37m"

void	magnifText(char c, std::string msg);

#endif
