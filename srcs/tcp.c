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

void			set_tcp_header(BYTE *buffer_raw, int port, int raw_len, char *scan)
{
	struct tcphdr			header;
	struct pseudo_header	psh;
	struct ip				*ip_header = (struct ip*)((void*)buffer_raw);
	int						len = sizeof(struct pseudo_header) + sizeof(struct tcphdr);
	char					*pseudogram = NULL;
	t_flag					*spoof = get_flag("spoof");

	if (!(pseudogram = (char*)malloc(len)))
		return ;
	header.source = htons(port);
	header.dest = htons(port); //16 bit in nbp format of destination port
	header.seq = htonl(1); //32 bit sequence number, initially set to zero
	header.ack_seq = 0; //32 bit ack sequence number, depends whether ACK is set or not
	header.doff = 5; //4 bits: 5 x 32-bit words on tcp header
	header.res1 = 0; //4 bits: Not used
	header.cwr = 0; //Congestion control mechanism
	header.ece = 0; //Congestion control mechanism
	header.urg = 0; //Urgent flag
	header.ack = 0; //Acknownledge
	header.psh = 0; //Push data immediately
	header.rst = 0; //RST flag
	header.syn = 0; //SYN flag
	header.fin = 0; //Terminates the connection
	header.window = htons(5840);//0xFFFF; //16 bit max number of databytes
	header.urg_ptr = 0; //16 bit indicate the urgent data. Only if URG flag is set
	header.check = 0; //16 bit check sum. Can't calculate at this point
	get_tcp_flags(&header, scan);
	psh.source_address = (spoof && spoof->value) ? inet_addr(spoof->value) : inet_addr(get_default_interface_host());
    psh.dest_address = ip_header->ip_dst.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr));

	ft_memset(pseudogram, 0, len);
	ft_memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
	ft_memcpy(pseudogram + sizeof(struct pseudo_header), &header, sizeof(struct tcphdr));

	header.check = checksum((unsigned short *)pseudogram, len);
	ft_memcpy((void*)buffer_raw + sizeof(struct ip), &header, sizeof(struct tcphdr));
	ft_strdel(&pseudogram);
}

void			wait_answer(t_thread_handler *thread_handler)
{
	char				buffer[ANSWER_BUFFER];
	struct				sockaddr_in from;
	socklen_t			len = sizeof(from);
	int					received = 0;

	received = recvfrom(thread_handler->fd, buffer, (ANSWER_BUFFER - 1), 0, (struct sockaddr*)&from, &len);
	if (received != SOCKET_ERROR) {
		printf("Received an answer of len %d\n", received);
	}
}

void			tcp_handler(t_thread_handler *thread_handler, char *scan, char *host)
{
	int		payload			= ft_strlen(PAYLOAD);
	int		raw_len 		= sizeof(struct ip) + sizeof(struct tcphdr) + payload;
	int		ports_len 		= thread_handler->ports_len;
	int		start_index 	= thread_handler->start;

	if ((thread_handler->fd = init_socket(IPPROTO_TCP)) != SOCKET_ERROR) {
		if (!(thread_handler->buffer_raw = (BYTE*)malloc(raw_len))) {
			return;
		}
		ft_memset(thread_handler->buffer_raw, 0, raw_len);
		set_ipv4_header(thread_handler->buffer_raw, raw_len, host, IPPROTO_TCP);
		while (ports_len)
		{
			if (ft_strlen(PAYLOAD) > 0) {
				char	*data_payload = (char*)(thread_handler->buffer_raw + sizeof(struct ip) + sizeof(struct tcphdr));
				ft_strcpy(data_payload, PAYLOAD);
			}
			set_tcp_header(thread_handler->buffer_raw, thread_handler->nmap->port[start_index], raw_len, scan);
			if ((send_socket_raw(thread_handler, raw_len, thread_handler->nmap->port[start_index])) > 0) {
				printf("Started scan for port %d on host %s with type %s\n", thread_handler->nmap->port[start_index],
				host, scan);
				wait_answer(thread_handler);
			}
			ports_len--;
			start_index++;
		}
		close (thread_handler->fd);
	}
}
