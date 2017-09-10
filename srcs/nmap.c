/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 12:51:06 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/10 12:51:43 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void		print_help()
{
	printf("ft_nmap [OPTIONS]\n\n--help Print this help screen\n--ports ports to scan (eg: 1-10 or 1,2,3 or 1,5-15)\n--host ip address to scan in dot format\n--file File name containing IP addresses to scan,\n--speedup [250 max] number of parallel threads to use\n--scan SYN/NULL/FIN/XMAS/ACK/UDP\n");
}

static void		check_help(char **argv)
{
	int i = 1;
	while (argv[i])
	{
		if (ft_strlen(argv[i]) > 2 && !ft_strcmp(ft_substr(argv[i], 0, 2), FLAG_SEPARATOR)
			&& !ft_strcmp(ft_substr(argv[i], 2, ft_strlen(argv[i])), "help")) {
			print_help();
			exit (0);
		}
		i++;
	}
}

int				main(int argc, char **argv)
{
	int			i		= 1;
	bool		value	= false;

	if (argc == 1) {
		print_help();
		return (0);
	}
	check_help(argv);
	while (i < argc)
	{
		if (ft_strlen(argv[i]) > 2 && !ft_strcmp(ft_substr(argv[i], 0, 2), FLAG_SEPARATOR)) {
			if (value) {
				printf("ft_nmap: Syntax error !\n");
				return (-1);
			}
			if (!get_flag(ft_substr(argv[i], 2, ft_strlen(argv[i])))) {
				add_flag(ft_substr(argv[i], 2, ft_strlen(argv[i])));
				value = true;
			} else {
				printf("ft_nmap: flag %s already set !\n", ft_substr(argv[i], 2, ft_strlen(argv[i])));
				return (-1);
			}
		} else {
			if (!add_value(argv[i])) {
				printf("ft_nmap: Syntax error !\n");
				return (-1);
			}
			value = false;
		}
		i++;
	}
	initializer();
	return (0);
}
