/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:22:38 by tcros             #+#    #+#             */
/*   Updated: 2026/03/18 18:45:53 by tcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PMergeMe.hpp"

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

size_t	jacobsthal(size_t k)
{
	return (round((pow(2, k + 1) + pow(-1, k)) / 3));
}

std::vector<size_t>	get_jacobsthal_vec(size_t size)
{
	size_t	nb = 3;
	size_t	nbb;
	std::vector<size_t>	res;

	if (size < 2)
		return (res);
	
	for(size_t i = 2; nb < size; ++i)
	{
		nb = jacobsthal(i);
		nbb = jacobsthal(i - 1) + 1;

		size_t	j = nb;
		if (j > size)
			j = size;
		while (j >= nbb)
			res.push_back(j--);
	}
	return (res);
}

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

static size_t	find_order(size_t size)
{
	size_t	order = 1;

	while (order < size / 2)
	{
		order *= 2;
	}
	return (order);
}

// static void	sorting_deque(std::deque<int>& deckList)
// {
// 	(void)deckList;
// }

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

static void	generate_pend_main_vector(std::vector<int>& vecList, std::vector<int>& main, std::vector<int>& pend, size_t order)
{
	// add first pending element to main.
	main.insert(main.begin(), vecList.begin(), vecList.begin() + order);

	//add all bigger to main numbers (winners) and all pending numbers (losers)
	std::vector<int>::iterator	it;
	size_t	win = 1;
	for (it = vecList.begin() + order; it != vecList.end(); std::advance(it, order))
	{
		if (static_cast<size_t>(std::distance(it, vecList.end())) < order)
			break ;
		
		if (win % 2 == 1)
			main.insert(main.end(), it, it + order);
		else
			pend.insert(pend.end(), it, it + order);
		win++;
	}
	//il faudra peut etre gerer le cas impaire dans une autre section afin de le mettre directement apres le sort_and_insert_vector
}

static void	sort_and_insert_vector(std::vector<int>& pend, std::vector<int>& main, size_t order)
{
	// peut etre un prob avec pend.size() / order
	std::vector<size_t>	j_id = get_jacobsthal_vec(pend.size() / order);

	for (size_t i = 0; i < j_id.size(); ++i)
	{
		size_t	id_win = (j_id[i] - 1) * 2 * order + (2 * order) - 1;
		std::vector<int>::iterator	limit;

		if (id_win >= main.size())
			limit = main.end();
		else
		{
			int	limValue = main[id_win];
			limit = std::lower_bound(main.begin(), main.end(), limValue);
		}

		int	insertValue = pend[(j_id[i] - 1) * order + (order - 1)];
		std::vector<int>::iterator	insertId = std::lower_bound(main.begin(), limit, insertValue);

		size_t	start_id = (j_id[i] - 1) * order;
		main.insert(insertId, pend.begin() + start_id, pend.begin() + start_id + order);
	}
}

static void	add_leftover_vector(std::vector<int>& vecList, std::vector<int>& pend, size_t order);

static void	inserting_vector(std::vector<int>& vecList, size_t order)
{
	std::vector<int>	main;
	std::vector<int>	pend;

	generate_pend_main_vector(vecList, main, pend, order);

	while (order >= 1)
	{
		sort_and_insert_vector(pend, main, order);

		order /= 2;

		add_leftover_vector(vecList, pend, order);
	}

	vecList = main;
}

static void	sorting_vector(std::vector<int>& vecList)
{
	merging_vector(vecList, 1);

	int	order = find_order(vecList.size());

	inserting_vector(vecList, order / 2);
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
