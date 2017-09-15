/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/15 06:31:30 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/15 06:31:35 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void		get_tcp_flags(struct tcphdr *header, char *scan)
{
	if (!ft_strcmp(scan, "SYN")) {
		header->syn = 1;
	} else if (!ft_strcmp(scan, "FIN")) {
		header->fin = 1;
	}
}
