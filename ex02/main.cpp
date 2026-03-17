/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:20:57 by tcros             #+#    #+#             */
/*   Updated: 2026/03/17 17:39:18 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PMergeMe.hpp"

int	main(int ac, char *av[])
{
	if (ac < 2)
	{
		std::cerr << "Usage: ./PMergeMe \"<sequence>\"" << std::endl;
		return (1);
	}
	else if (ac == 2)
	{
		std::cout << "One number is already sorted !" << std::endl;
		return (0);
	}
	else
	{
		PMergeMe(ac, av);
	}
	return (0);
}
