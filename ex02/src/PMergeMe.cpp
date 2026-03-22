/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:22:38 by tcros             #+#    #+#             */
/*   Updated: 2026/03/22 19:22:22 by tcros            ###   ########.fr       */
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

int	diff_jacobsthal(size_t cpt)
{
	return (jacobsthal(cpt + 1) - jacobsthal(cpt));
}

static void	parse(std::string str)
{
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

static void	generate_pend_main_vector(std::vector<int>& vecList, std::vector<int>& main, std::vector<int>& pend, std::vector<int>& leftover, std::vector<size_t>& m_id, std::vector<size_t>& p_id, size_t order)
{
	// add first pending element to main.
	main.insert(main.begin(), vecList.begin(), vecList.begin() + order * 2);
	m_id.push_back(0);
	m_id.push_back(1);

	//add all bigger to main numbers (winners) and all pending numbers (losers)
	std::vector<int>::iterator	it;
	size_t	win = 2;
	for (it = vecList.begin() + order * 2; it + order * 2 <= vecList.end(); std::advance(it, order * 2))
	{
		main.insert(main.end(), it + order, it + order * 2);
		pend.insert(pend.end(), it, it + order);
		p_id.push_back(win);
		m_id.push_back(win);
		win++;
	}
	if (it < vecList.end())
		leftover.insert(leftover.end(), it, vecList.end());
	if (leftover.size() == order)
	{
		pend.insert(pend.end(), it, vecList.end());
		p_id.push_back(win - 1);
		leftover.clear();
	}
}

static size_t	dichotomie_vector(std::vector<int>& pend, std::vector<int>& main, std::vector<size_t>& m_id, std::vector<size_t>& p_id, int gap, size_t order)
{
	int	begin = 0;
	int	end = 0;

	size_t	i = 0;
	while (i < m_id.size() && p_id[gap] != m_id[i])
	{
		end++;
		i++;
	}
	while (begin <= end)
	{
		size_t	mid = (begin + end) / 2;
		if (main[mid * order + order - 1] < pend[gap * order + (order - 1)])
			begin = mid + 1;
		else
			end = mid - 1;
	}
	for (size_t j = 0; j < m_id.size(); ++j)
    {
        if (m_id[j] >= (size_t)begin)
            m_id[j] += 1;
    }
	m_id.insert(m_id.begin() + begin, (size_t)begin);
	p_id.erase(p_id.begin() + gap);
	return (begin * order);
}

static void	sort_and_insert_vector(std::vector<int>& pend, std::vector<int>& main, std::vector<size_t>& m_id, std::vector<size_t>& p_id, size_t order)
{
	size_t	cpt = 1;
	while(!pend.empty())
	{
		int	gap = diff_jacobsthal(cpt) - 1;
		while (gap >= 0)
		{
			if (gap * order >= pend.size())
			{
				gap--;
				continue ;
			}
			size_t	insert = dichotomie_vector(pend, main, m_id, p_id, gap, order);
			main.insert(main.begin() + insert, pend.begin() + (order * gap), pend.begin() + (order) + (order * gap));
			pend.erase(pend.begin() + (order * gap), pend.begin() + order + (order * gap));
			gap--;
		}
		cpt++;
	}
}

static void	inserting_vector(std::vector<int>& vecList, size_t order)
{
	while (order >= 1)
	{
		std::vector<int>	main;
		std::vector<int>	pend;
		std::vector<int>	leftover;
		std::vector<size_t>	m_id;
		std::vector<size_t>	p_id;

		generate_pend_main_vector(vecList, main, pend, leftover, m_id, p_id, order);

		sort_and_insert_vector(pend, main, m_id, p_id, order);

		main.insert(main.end(), leftover.begin(), leftover.end());
		vecList = main;
		order /= 2;
	}
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
			std::string	str(av[i]);

			parse(str);

			int	j = i + 1;
			while (j < ac)
			{
				std::string	st2(av[j++]);
				if (str == st2)
					throw NoDuplicateAllowed();
			}

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
		//check if sorted

		std::vector<int>::iterator iV;
		for (iV = vecList.begin(); iV < vecList.end() - 1; iV++)
		{
			if (*iV > *(iV + 1))
			{
				std::cout << CYAN << "Result vector : " << RESET << "Not sorted 😔" << std::endl;
				break ;
			}
		}
		if (iV == vecList.end() - 1)
			std::cout << CYAN << "Result vector : " << RESET << "Sorted 👍" << std::endl;
		
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
}
