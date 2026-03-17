/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:22:38 by tcros             #+#    #+#             */
/*   Updated: 2026/03/17 18:56:45 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PMergeMe.hpp"

static void	parse(char *av)
{
	std::string	str(av);

	if (str.empty())
		throw BadInput();

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
			throw NotADigit();
	}

	std::stringstream	ss(str);
	long	num;
	ss >> num;

	if (num > INT_MAX)
		throw TooLargeNumber();
}


// static void	sorting_deque(std::deque<int>& deckList)
// {
// 	(void)deckList;
// }

static void	printSequence(const std::string& type, std::vector<int> vecList)
{
	std::cout << type;

	for (std::vector<int>::iterator it = vecList.begin(); it != vecList.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
}

static void	printDuration(int ac, const std::string& type, std::clock_t start, std::clock_t end)
{
	double	duration_ms = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000.0;

	std::cout << "Time to process a range of "
		<< ac - 1 <<  " elements with std::" << type << " : "
		<< duration_ms << " ms." << std::endl;
}

static void	merging_vector(std::vector<int>& vecList, size_t order)
{
	if (vecList.size() / 2 < order)
		return ;

	std::vector<int>::iterator	it;
	for (it = vecList.begin(); it != vecList.end(); std::advance(it, order * 2))
	{
		if (static_cast<size_t>(std::distance(it, vecList.end())) < (order * 2))
			break ;

		if (*(it + (order - 1)) > *(it + (2 * order) - 1))
		{
			std::swap_ranges(it, it + order, it + order);
		}
	}
	merging_vector(vecList, order * 2);
}

static void	sorting_vector(std::vector<int>& vecList)
{
	merging_vector(vecList, 1);
}

void	PMergeMe(int ac, char *av[])
{
	std::deque<int>		deckList;
	std::vector<int>	vecList;

	try
	{
		int	i = 1;

		while (i < ac)
		{
			parse(av[i]);

			int	n = std::atoi(av[i]);
			deckList.push_back(n);
			vecList.push_back(n);
			i++;
		}

		printSequence("Before : ", vecList);
		std::clock_t	start_vec = std::clock();

		sorting_vector(vecList);

		std::clock_t	end_vec = std::clock();
		std::clock_t	start_deq = std::clock();

		// sorting_deque(deckList);

		std::clock_t	end_deq = std::clock();

		printSequence("After:  ", vecList);
		printDuration(ac, "vector", start_vec, end_vec);
		printDuration(ac, "deque", start_deq, end_deq);
		
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
}
