/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 17:54:02 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/12 17:54:08 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void			free_hosts(t_nmap *nmap)
{
	t_host		*tmp_host = NULL;

	while (nmap->hosts){
		ft_strdel(&nmap->hosts->address);
		tmp_host = nmap->hosts->next;
		free(nmap->hosts);
		nmap->hosts = tmp_host;
	}
}

static void			free_flags()
{
	t_flag		*tmp_flag = NULL;
	t_flag		*flags = get_flags();

	while (flags) {
		ft_strdel(&flags->flag);
		if (flags->value) {
			ft_strdel(&flags->value);
		}
		tmp_flag = flags->next;
		free(flags);
		flags = tmp_flag;
	}
}

static void			free_scans(t_nmap *nmap)
{
	t_scan_type		*tmp_type = NULL;

	while (nmap->scans) {
		ft_strdel(&nmap->scans->name);
		tmp_type = nmap->scans->next;
		free(nmap->scans);
		nmap->scans = tmp_type;
	}
}

void				free_datas(t_nmap *nmap)
{
	free_hosts(nmap);
	free_scans(nmap);
	free_flags();

	free(nmap->port);
	free(nmap);
}
