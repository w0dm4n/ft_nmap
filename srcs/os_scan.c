/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os_scan.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/20 01:46:03 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/20 01:46:09 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void		init_os_scan(t_nmap *nmap)
{
	t_host		*hosts = nmap->hosts;

	while (hosts)
	{
		printf("Host: %s\n", hosts->address);
		hosts = hosts->next;
	}
}
