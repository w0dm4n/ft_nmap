/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:54:02 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/19 01:20:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void		my_strdel(char **str)
{
	if (!str || !*str)
		return ;
	free(*str);
	*str = NULL;
}

static void		free_hosts(t_host *host)
{
	if (host->next)
		free_hosts(host->next);
	if (host->addresses)
		ft_free_array((void **)host->addresses);
	if (host->address)
		my_strdel(&host->address);
	free(host);
}

static void		free_scans(t_scan_type *scan)
{
	if (scan->next)
		free_scans(scan->next);
	if (scan->name)
		my_strdel(&scan->name);
	free(scan);
}

static void		free_flags(t_flag *flags)
{
	if (!flags)
		return ;
	if (flags->next)
		free_flags(flags->next);
	if (flags->flag)
		my_strdel(&flags->flag);
	if (flags->value)
		my_strdel(&flags->value);
	free(flags);
}

void			free_datas(t_nmap *nmap)
{
	if (nmap)
	{
		if (nmap->hosts)
			free_hosts(nmap->hosts);
		if (nmap->scans)
			free_scans(nmap->scans);
		if (nmap->port)
			free(nmap->port);
		free(nmap);
	}
	free_flags(get_flags());
}
