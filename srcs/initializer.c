/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 15:34:01 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/19 01:22:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void		print_error(char *msg, t_nmap *nmap)
{
	printf("ft_mmap: %s\n", msg);
	free_datas(nmap);
	exit(-1);
}

static t_nmap	*init_nmap()
{
	t_nmap		*nmap = NULL;

	if (!(nmap = (t_nmap*)malloc(sizeof(struct s_nmap))))
		return NULL;
	nmap->hosts = NULL;
	nmap->port = NULL;
	nmap->threads = DEFAULT_THREADS;
	nmap->ports_index = 0;
	nmap->scans = NULL;
	return (nmap);
}

void			initializer()
{
	t_flag		*host = get_flag("host");
	t_flag		*file = get_flag("file");
	t_flag		*speed = get_flag("speedup");
	t_nmap		*nmap = NULL;
	bool		multiple_host = false;

	if (host != NULL && file != NULL) {
		print_error("you cant set an adress as host and a file container", nmap);
	}
	if (host && host->value) {
		multiple_host = false;
	} else if (file && file->value) {
		multiple_host = true;
	}
	else {
		print_error("no host has been set (missing flag --host or --file)", nmap);
	}
	if ((nmap = init_nmap()) != NULL) {
		if (speed) {
			if (speed->value) {
				int threads = ft_atoi(speed->value);

				if (threads >= 1 && threads <= 250) {
					nmap->threads = threads;
				}
				else
					print_error("invalid speedup number [1 - 250]", nmap);
			}
			else
				print_error("invalid speedup number [1 - 250]", nmap);
		}
		if (load_hosts(multiple_host, nmap) && load_ports(nmap) && load_scans_type(nmap))
			instantiate_threads(nmap);
	}
	free_datas(nmap);
}
