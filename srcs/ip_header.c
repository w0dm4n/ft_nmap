/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip_header.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 20:58:15 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/14 20:58:22 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void		set_ipv4_header(BYTE *buffer_raw, int raw_len, char *host, char *proto)
{
	struct ip	header;

	header.ip_src.s_addr = INADDR_ANY; // spoof flag
	if (!(inet_pton(AF_INET, host, &(header.ip_dst))))
	{
		printf("Can't set destination network address for %s\n", host);
		return ;
	}
	header.ip_ttl = DEFAULT_TTL; // Time to live
	header.ip_p = (!ft_strcmp(proto, "tcp")) ? IPPROTO_TCP : IPPROTO_UDP; //  Protocol
	header.ip_v = 4; // IPv4
	header.ip_hl = sizeof(struct ip) >> 2; // IPv4 header length (4 bits)
	header.ip_tos = 0;
	header.ip_len = htons(raw_len);
	header.ip_off = 0;
	header.ip_sum = 0; // set checksum to zero to calculate
	header.ip_sum = checksum((uint16_t *) &header, sizeof(struct ip)); // calculate checksum
	ft_memcpy((void*)buffer_raw, &header, sizeof(struct ip));
}