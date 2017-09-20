/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 12:51:06 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/18 23:56:54 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int		main_error(char *str, char *arg)
{
	printf("ft_nmap: ");
	printf(str, arg);
	printf("\n");
	if (arg)
		ft_strdel(&arg);
	free_datas(NULL);
	return 2;
}

static void		print_help()
{
	printf("ft_nmap [OPTIONS]\n\n"
		   "--help Print this help screen\n"
		   "--ports ports to scan (eg: 1-10 or 1,2,3 or 1,5-15)\n"
		   "--host ip address to scan in dot format\n"
		   "--file File name containing IP addresses to scan,\n"
		   "--speedup [250 max] number of parallel threads to use\n"
		   "--scan SYN/NULL/FIN/XMAS/ACK/UDP\n"
		   "--deep For scanning all addresses from a given host\n"
		   "--src Spoof the src address\n"
		   "--timeout Wait time for host answer (seconds or ms)\n"
		   "--osscan Scan the host single threaded for guessing the OS\n"
		   "--closed Show closed ports too on display\n");
}

void		print_start(t_nmap *nmap)
{
	t_flag		*spoof = get_flag("spoof");
	t_scan_type	*scans = nmap->scans;

	if (spoof && spoof->value) {
		printf("\nStarting ft_nmap ( http://nmap.org ) with spoof address %s\n", spoof->value);
	} else {
		printf("\nStarting ft_nmap ( http://nmap.org )\n");
	}
	if (!get_flag("osscan")) {
		printf("Scan flags: ");
		while (scans)
		{
			printf("%s ", scans->name);
			scans = scans->next;
		}
		printf("\n");
	}
}

static void		check_help(char **argv)
{
	int			i		= 1;
	char		*option	= NULL;
	char		*arg	= NULL;

	while (argv[i])
	{
		if ((option = ft_substr(argv[i], 0, 2)) != NULL) {
			if ((arg = ft_substr(argv[i], 2, ft_strlen(argv[i]))) != NULL) {
				if (ft_strlen(argv[i]) > 2 &&
					!ft_strcmp(option, FLAG_SEPARATOR) &&
					!ft_strcmp(arg, "help")) {
					print_help();
					ft_strdel(&arg);
					ft_strdel(&option);
					exit (0);
				}
				ft_strdel(&arg);
			}
			ft_strdel(&option);
		}
		i++;
	}
}

bool		single_flag(char *flag, bool sub)
{
	char	*tmp = (sub) ? ft_substr(flag, 2, ft_strlen(flag)) : flag;
	if (!tmp) {
		return (false);
	}
	if (!ft_strcmp(tmp, "osscan") || !ft_strcmp(tmp, "help")
		|| !ft_strcmp(tmp, "deep") || !ft_strcmp(tmp, "closed")) {
			if (sub) ft_strdel(&tmp);
			return (true);
		}
	if (sub) {
		ft_strdel(&tmp);
	}
	return (false);
}

int				main(int argc, char **argv)
{
	int			i		= 1;
	bool		value	= false;
	char		*option	= NULL;

	if (argc == 1) {
		print_help();
		return (0);
	}
	check_help(argv);
	while (i < argc)
	{
		if ((option = ft_substr(argv[i], 0, 2)) != NULL) {
			if (ft_strlen(argv[i]) > 2 && !ft_strcmp(option, FLAG_SEPARATOR)) {
				if (value) {
					return main_error("Syntax error !", option);
				}
				ft_strdel(&option);
				if ((option = ft_substr(argv[i], 2, ft_strlen(argv[i]))) != NULL) {
					if (!get_flag(option)) {
						add_flag(option);
						if (!single_flag(argv[i], true)) {
							value = true;
						}
					}
					else
						return main_error("flag %s already set !", option);
				}
			} else {
				if (!add_value(argv[i])) {
					return main_error("Syntax error !", option);
				}
				value = false;
			}
			ft_strdel(&option);
		}
		i++;
	}
	initializer();
	return (0);
}
