/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:54:48 by tcros             #+#    #+#             */
/*   Updated: 2026/02/17 18:57:55 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coloring.hpp"

void	magnifText(char c, std::string msg)
{
	std::cout << std::endl;
	switch (c)
	{
		case 'i':
			std::cout << BLUE << "[INFO] " << ITALIC << msg << RESET << std::endl;
			break;
		case 'w':
			std::cout << YELLOW << "[WARNING] " << ITALIC << msg << RESET << std::endl;
			break;
		case 'e':
			std::cerr << RED << "[ERROR]" << ITALIC << " Exception caught : " << msg << RESET << std::endl;
			break;
		default:
			std::cout << GREEN << "[ALL GOOD] " << ITALIC << msg << RESET << std::endl;
			break;
	}
}
