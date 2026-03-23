/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcros <tcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:22:38 by tcros             #+#    #+#             */
/*   Updated: 2026/03/23 12:54:23 by tcros            ###   ########.fr       */
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

static void	merging_deque(std::deque<int>& deqList, size_t order)
{
	if (deqList.size() / 2 < order)
		return ;

	std::deque<int>::iterator	it;
	for (it = deqList.begin(); it != deqList.end(); std::advance(it, order * 2))
	{
		if (static_cast<size_t>(std::distance(it, deqList.end())) < (order * 2))
			break ;

		if (*(it + (order - 1)) > *(it + (2 * order) - 1))
		{
			std::swap_ranges(it, it + order, it + order);
		}
	}
	merging_deque(deqList, order * 2);
}

static void	generate_pend_main_deque(std::deque<int>& deqList, std::deque<int>& main, std::deque<int>& pend, std::deque<int>& leftover, std::deque<size_t>& m_id, std::deque<size_t>& p_id, size_t order)
{
	std::deque<int>::iterator	it;
	size_t	win = 0;
	for (it = deqList.begin(); it + order * 2 <= deqList.end(); std::advance(it, order * 2))
	{
		if (it == deqList.begin())
		{
			main.insert(main.end(), it, it + order);
			m_id.push_back(win);
			win++;
		}
		else
		{
			pend.insert(pend.end(), it, it + order);
			p_id.push_back(win);
		}
		main.insert(main.end(), it + order, it + order * 2);
		m_id.push_back(win);
		win++;
	}
	if (it + order <= deqList.end())
	{
		pend.insert(pend.end(), it, it + order);
		p_id.push_back(win);
	}
	else
		leftover.insert(leftover.end(), it, deqList.end());

	if (it + order + 1 <= deqList.end())
	{
		main.insert(main.end(), it + order, deqList.end());
		m_id.push_back(win);
	}
}

static size_t	dichotomie_deque(std::deque<int>& pend, std::deque<int>& main, std::deque<size_t>& m_id, std::deque<size_t>& p_id, int gap, size_t order)
{
	int	begin = 0;
	int	end = 0;

	size_t	i = 0;
	while (i < m_id.size() && p_id[gap] != m_id[i])
	{
		end++;
		i++;
	}
	//borne superieur exclue
	end--;

	while (begin <= end)
	{
		size_t	mid = (begin + end) / 2;
		if (main[mid * order + order - 1] < pend[gap * order + (order - 1)])
			begin = mid + 1;
		else
			end = mid - 1;
	}
	m_id.insert(m_id.begin() + begin, p_id[gap]);
	p_id.erase(p_id.begin() + gap);
	return (begin * order);
}

static void	sort_and_insert_deque(std::deque<int>& pend, std::deque<int>& main, std::deque<size_t>& m_id, std::deque<size_t>& p_id, size_t order)
{
	size_t	cpt = 1;
	// ///////////////////////// Affichage pour voir l'algo
	// std::deque<int>::iterator	it;
	// int							j = 1;
	// std::cout << "==> pend " << order << " =";
	// for (it = pend.begin(); it < pend.end(); it++)
	// {
	// 	std::cout << " " << *it;
	// 	if (j % order == 0)
	// 		std::cout << ",";
	// 	j++;
	// }
	// std::cout << "\n" << std::endl;
	// j = 1;
	// std::cout << "==> main " << order << " =";
	// for (it = main.begin(); it < main.end(); it++)
	// {
	// 	std::cout << " " << *it;
	// 	if (j % order == 0)
	// 		std::cout << ",";
	// 	j++;
	// }
	// std::cout << "\n\n" << std::endl;
	
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
			size_t	insert = dichotomie_deque(pend, main, m_id, p_id, gap, order);
			main.insert(main.begin() + insert, pend.begin() + (order * gap), pend.begin() + (order) + (order * gap));
			pend.erase(pend.begin() + (order * gap), pend.begin() + order + (order * gap));
			// /////////////////////// Affichage pour voir l'algo
			// j = 1;
			// std::cout << "==> pend " << order << " =";
			// for (it = pend.begin(); it < pend.end(); it++)
			// {
			// 	std::cout << " " << *it;
			// 	if (j % order == 0)
			// 		std::cout << ",";
			// 	j++;
			// }
			// std::cout << "\n" << std::endl;
			// j = 1;
			// std::cout << "==> main " << order << " =";
			// for (it = main.begin(); it < main.end(); it++)
			// {
			// 	std::cout << " " << *it;
			// 	if (j % order == 0)
			// 		std::cout << ",";
			// 	j++;
			// }
			// std::cout << "\n\n" << std::endl;
			gap--;
		}
		cpt++;
	}
}

static void	inserting_deque(std::deque<int>& deqList, size_t order)
{
	while (order >= 1)
	{
		std::deque<int>	main;
		std::deque<int>	pend;
		std::deque<int>	leftover;
		std::deque<size_t>	m_id;
		std::deque<size_t>	p_id;

		generate_pend_main_deque(deqList, main, pend, leftover, m_id, p_id, order);

		sort_and_insert_deque(pend, main, m_id, p_id, order);

		main.insert(main.end(), leftover.begin(), leftover.end());
		deqList = main;
		order /= 2;
	}
}

static void	sorting_deque(std::deque<int>& deqList)
{
	merging_deque(deqList, 1);

	int	order = find_order(deqList.size());

	inserting_deque(deqList, order / 2);
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
	std::vector<int>::iterator	it;
	size_t	win = 0;
	for (it = vecList.begin(); it + order * 2 <= vecList.end(); std::advance(it, order * 2))
	{
		if (it == vecList.begin())
		{
			main.insert(main.end(), it, it + order);
			m_id.push_back(win);
			win++;
		}
		else
		{
			pend.insert(pend.end(), it, it + order);
			p_id.push_back(win);
		}
		main.insert(main.end(), it + order, it + order * 2);
		m_id.push_back(win);
		win++;
	}
	if (it + order <= vecList.end())
	{
		pend.insert(pend.end(), it, it + order);
		p_id.push_back(win);
	}
	else
		leftover.insert(leftover.end(), it, vecList.end());

	if (it + order + 1 <= vecList.end())
	{
		main.insert(main.end(), it + order, vecList.end());
		m_id.push_back(win);
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
	//borne superieur exclue
	end--;

	while (begin <= end)
	{
		size_t	mid = (begin + end) / 2;
		if (main[mid * order + order - 1] < pend[gap * order + (order - 1)])
			begin = mid + 1;
		else
			end = mid - 1;
	}
	m_id.insert(m_id.begin() + begin, p_id[gap]);
	p_id.erase(p_id.begin() + gap);
	return (begin * order);
}

static void	sort_and_insert_vector(std::vector<int>& pend, std::vector<int>& main, std::vector<size_t>& m_id, std::vector<size_t>& p_id, size_t order)
{
	size_t	cpt = 1;
	// ///////////////////////// Affichage pour voir l'algo
	// std::vector<int>::iterator	it;
	// int							j = 1;
	// std::cout << "==> pend " << order << " =";
	// for (it = pend.begin(); it < pend.end(); it++)
	// {
	// 	std::cout << " " << *it;
	// 	if (j % order == 0)
	// 		std::cout << ",";
	// 	j++;
	// }
	// std::cout << "\n" << std::endl;
	// j = 1;
	// std::cout << "==> main " << order << " =";
	// for (it = main.begin(); it < main.end(); it++)
	// {
	// 	std::cout << " " << *it;
	// 	if (j % order == 0)
	// 		std::cout << ",";
	// 	j++;
	// }
	// std::cout << "\n\n" << std::endl;
	
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
			// /////////////////////// Affichage pour voir l'algo
			// j = 1;
			// std::cout << "==> pend " << order << " =";
			// for (it = pend.begin(); it < pend.end(); it++)
			// {
			// 	std::cout << " " << *it;
			// 	if (j % order == 0)
			// 		std::cout << ",";
			// 	j++;
			// }
			// std::cout << "\n" << std::endl;
			// j = 1;
			// std::cout << "==> main " << order << " =";
			// for (it = main.begin(); it < main.end(); it++)
			// {
			// 	std::cout << " " << *it;
			// 	if (j % order == 0)
			// 		std::cout << ",";
			// 	j++;
			// }
			// std::cout << "\n\n" << std::endl;
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
	std::deque<int>		deqList;
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
			deqList.push_back(n);
			vecList.push_back(n);
			i++;
		}

		printSequence("Before : ", vecList);
		std::clock_t	start_vec = std::clock();

		sorting_vector(vecList);

		std::clock_t	end_vec = std::clock();
		std::clock_t	start_deq = std::clock();

		sorting_deque(deqList);

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

		std::deque<int>::iterator iD;
		for (iD = deqList.begin(); iD < deqList.end() - 1; iD++)
		{
			if (*iD > *(iD + 1))
			{
				std::cout << CYAN << "Result deque : " << RESET << "Not sorted 😔" << std::endl;
				break ;
			}
		}
		if (iD == deqList.end() - 1)
			std::cout << CYAN << "Result deque : " << RESET << "Sorted 👍" << std::endl;
		
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
	}
}
