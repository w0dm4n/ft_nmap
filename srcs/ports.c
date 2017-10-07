/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ports.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 16:34:42 by frmarinh          #+#    #+#             */
/*   Updated: 2017/10/07 21:13:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static bool		add_port(t_nmap *nmap, int port)
{
	int			i = 0;
	bool		in_array = false;

	if (port >= 1 && port <= PORTS_SIZE) {
		while (i < nmap->ports_index) {
			if (nmap->port[i] == port)
				in_array = true;
			i++;
		}
		if (!in_array) {
			nmap->port[nmap->ports_index++] = port;
		}
	} else {
		printf("ft_mmap: port %d is invalid ! (1 - %d)\n", port, PORTS_SIZE);
		return (false);
	}
	return (true);
}

bool			load_ports(t_nmap *nmap)
{
	t_flag		*flag = NULL;
	int			i = 1, p = 0, range_start = 0, range_end = 0, diff = 0;
	char		**range = NULL;
	char		**port = NULL;
	bool		ret = true;

	if (!(nmap->port = (int*)malloc(MAX_PORTS_SCAN * sizeof(int))))
		return false;
	if (!(flag = get_flag("ports"))) {
		nmap->ports_index = MAX_PORTS_SCAN;
		while (p < MAX_PORTS_SCAN) {
			nmap->port[p++] = i++;
		}
	} else if (flag->value != NULL) {
		if ((port = ft_strsplit(flag->value, ',')) != NULL) {
			while (ret == true && port[p])
			{
				if (ft_strchr(port[p], '-')) {
					if ((range = ft_strsplit(port[p], '-')) != NULL) {
						if (range[0] && range[1]) {
							range_start = ft_atoi(range[0]);
							range_end = ft_atoi(range[1]);
							diff = range_end - range_start;
							if (diff > 0 && (diff < MAX_PORTS_SCAN)) {
								while (ret == true && range_start <= range_end)
									ret = add_port(nmap, range_start++);
							}
							else
								printf("ft_mmap: ports range %d-%d invalid.\n", range_start, range_end);
						}
						else
							printf("ft_mmap: ports range missing.\n");
						ft_free_array((void **)range);
					}
				}
				else
					ret = add_port(nmap, ft_atoi(port[p]));
				p++;
			}
			ft_free_array((void **)port);
		}
	}
	return ret;
}
