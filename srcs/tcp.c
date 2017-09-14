/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 20:43:17 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/14 20:43:29 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static SOCKET	init_socket()
{
	SOCKET	sock = NULL;
	bool	opt = true;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) // Raw socket descriptor
	{
		printf("socket() failed : Operation not permitted\n");
		return (SOCKET_ERROR);
	}
	if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt))) < 0) // set flag so socket expects us to provide IPv4 header.
	{
		printf("setsockopt() failed to set IP_HDRINCL\n");
		return (SOCKET_ERROR);
	}
	return (sock);
}

/*
	struct tcphdr {
		u_short	th_sport;		// source port
		u_short	th_dport;		// destination port
		tcp_seq	th_seq;			// sequence number
		tcp_seq	th_ack;			// acknowledgement number
	#if BYTE_ORDER == LITTLE_ENDIAN
		u_char	th_x2:4,		//(unused)
			th_off:4;		//data offset
	#endif
	u_char	th_flags;
	#define	TH_FIN	0x01
	#define	TH_SYN	0x02
	#define	TH_RST	0x04
	#define	TH_PUSH	0x08
	#define	TH_ACK	0x10
	#define	TH_URG	0x20
		u_short	th_win;			// window
		u_short	th_sum;			// checksum
		u_short	th_urp;			// urgent pointer
	};*/

//void			set_tcp_header(BYTE *buffer_raw, )

void			tcp_handler(t_thread_handler *thread_handler, char *scan, char *host)
{
	int		payload = 0;
	int		raw_len = sizeof(struct ip) + sizeof(struct tcphdr) + payload;
	if ((thread_handler->fd = init_socket()) != SOCKET_ERROR) {
		if (!(thread_handler->buffer_raw = (BYTE*)malloc(raw_len))) {
			return;
		}
		set_ipv4_header(thread_handler->buffer_raw, raw_len, host, "tcp");
	}
	/*while (thread_handler->ports_len)
	{
		printf("%d\n", thread_handler->nmap->port[thread_handler->start++]);
		thread_handler->ports_len--;
	}*/
}
