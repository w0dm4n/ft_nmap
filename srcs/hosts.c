/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hosts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 16:08:20 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/10 16:08:26 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static t_host	*alloc_host(t_host *to_alloc, char *addr)
{
	if (!(to_alloc = (t_host*)malloc(sizeof(struct s_host))))
		return ;
	to_alloc->address = ft_strdup(addr);
	to_alloc->next = NULL;
	to_alloc->addresses = NULL;
	return (to_alloc);
}

static void		add_host(t_nmap *nmap, char *addr)
{
	t_host		*new = NULL;
	t_host		*hosts = nmap->hosts;

	if (hosts) {
		if (!(new = alloc_host(new, addr)))
			return ;
		while (hosts->next)
			hosts = hosts->next;
		hosts->next = new;
	} else {
		if (!(nmap->hosts = alloc_host(nmap->hosts, addr)))
			return ;
	}
}

void			load_hosts(bool multiple_host, t_nmap *nmap)
{
	t_flag		*flag	= NULL;
	int			fd		= 0;
	char		*hosts	= NULL;

	if (!multiple_host) {
		flag = get_flag("host");
		if (!(nmap->hosts = (t_host*)malloc(sizeof(struct s_host))))
			return ;
		nmap->hosts->next = NULL;
		nmap->hosts->address = flag->value;
	} else {
		flag = get_flag("file");
		if ((fd = open(flag->value, O_RDONLY)) != -1) {
			while ((get_next_line(fd, &hosts)) > 0)
				add_host(nmap, hosts);
		} else {
			perror("ft_mmap");
		}
	}
}
