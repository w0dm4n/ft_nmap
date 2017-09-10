/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 15:34:01 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/10 15:34:07 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void			print_error(char *msg)
{
	printf("ft_mmap: %s\n", msg);
	exit(-1);
}

static t_nmap	*init_nmap()
{
	t_nmap		*nmap = NULL;

	if (!(nmap = (t_nmap*)malloc(sizeof(struct s_nmap))))
		return;
	nmap->hosts = NULL;
	nmap->port = NULL;
	nmap->threads = DEFAULT_THREADS;
	nmap->ports_index = 0;
	return (nmap);
}

void			initializer()
{
	t_flag		*flag = NULL;
	t_nmap		*nmap = NULL;
	bool		multiple_host = false;

	if (((flag = get_flag("host")) != NULL) && ((flag = get_flag("file")) != NULL)) {
		print_error("you cant set an adress as host and a file container");
	}
	if ((flag = get_flag("host")) && flag->value) {
		multiple_host = false;
	} else if ((flag = get_flag("file")) && flag->value) {
		multiple_host = true;
	}
	else {
		print_error("no host has been set (missing flag --host or --file)");
	}
	if ((nmap = init_nmap())) {
		if ((flag = get_flag("speedup")) && flag->value) {
			int threads = ft_atoi(flag->value);
			if (threads >= 1 && threads <= 250) {
				nmap->threads = threads;
			} else { print_error("invalid speedup number [1 - 250]"); }
		}
		load_hosts(multiple_host, nmap);
		load_ports(nmap);
	}
}
