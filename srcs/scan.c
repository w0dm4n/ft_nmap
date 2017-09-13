/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 21:31:08 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/13 21:31:13 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void		start_scans(t_thread_handler *thread_handler)
{
	while (thread_handler->ports_len)
	{
		printf("%d\n", thread_handler->nmap->port[thread_handler->start++]);
		thread_handler->ports_len--;
	}
}
