/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pcap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/16 04:38:55 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/19 01:41:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void			pcap_dump(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
    static int count = 1;
	int i = 0;
	bytes += 14; // link layer header
	struct ip	*header = (struct ip*)bytes;
	if (header->ip_p == IPPROTO_TCP) {
		struct tcphdr			*tcp_header		= (struct tcphdr*)((void*)bytes + sizeof(struct ip));
		int						id				= ntohs(tcp_header->dest);
		t_queue *queue = find_queue(IPPROTO_TCP, id);
		if (queue) {
			if (!ft_strcmp(queue->scan, "SYN") && tcp_header->syn && tcp_header->ack) {
				printf("SYN SCAN: %d/tcp open (%d)\n", queue->port, ntohs(tcp_header->source));
			} else if (!ft_strcmp(queue->scan, "ACK") && tcp_header->rst) {
				printf("ACK SCAN: %d/tcp unfiltered (%d)\n", queue->port, ntohs(tcp_header->source));
			} else if (!ft_strcmp(queue->scan, "FIN") && tcp_header->rst) {
				printf("FIN SCAN: %d/tcp closed (%d)\n", queue->port, ntohs(tcp_header->source));
			} else if (!ft_strcmp(queue->scan, "NULL") && tcp_header->rst && tcp_header->ack) {
				printf("NULL SCAN: %d/tcp closed (%d)\n", queue->port, ntohs(tcp_header->source));
			} else if (!ft_strcmp(queue->scan, "XMAS") && tcp_header->rst && tcp_header->ack) {
				printf("XMAS SCAN: %d/tcp closed (%d)\n", queue->port, ntohs(tcp_header->source));
			}
			queue->done = true;
		}
	}
	else if (header->ip_p == IPPROTO_ICMP)
	{
		struct ip				*ip_header		= (struct ip*)((void*)bytes);
		t_queue *queue = find_queue(IPPROTO_UDP, -1);
		if (queue) {
			printf("UDP SCAN: %d/udp closed\n", queue->port);
		}
	}
    fflush(stdout);
    count++;
}

void				*init_pcap(void *h, t_thread_handler *t)
{
	char				*dev = DEFAULT_INTERFACE;
	char				errbuf[PCAP_ERRBUF_SIZE];
	pcap_t				*handle;
	struct bpf_program	fp;					/* The compiled filter expression */
	char				host[256], netstr[INET_ADDRSTRLEN], maskstr[INET_ADDRSTRLEN];
	char				filter_exp[256];	/* The filter expression */
	bpf_u_int32			mask;				/* The netmask of our sniffing device */
	bpf_u_int32			net;				/* The IP of our sniffing device */
	int					pcount;
	t_flag				*timeout = get_flag("timeout");

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
	{
		printf("ft_nmap: Can't get net/mask for device %s\n", dev);
		net = 0;
		mask = 0;
	}
	if (timeout && timeout->value) {
		handle = pcap_open_live(dev, 4096, 1, ft_atoi(timeout->value), errbuf);
	} else {
		handle = pcap_open_live(dev, 4096, 1, DEFAULT_TIMEOUT, errbuf);
	}
	if (handle == NULL) {
		printf("ft_nmap: Couldn't open device %s: %s\n", dev, errbuf);
		return (NULL);
	}
	inet_ntop(AF_INET, (char*) &net, netstr, sizeof (netstr));
	inet_ntop(AF_INET, (char*) &mask, maskstr, sizeof (maskstr));
	sprintf(filter_exp, "tcp or udp");
	if (pcap_compile(handle, &fp, filter_exp, 1, mask) == -1)
	{
		printf("ft_nmap: couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return (NULL);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		printf("ft_nmap: couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return (NULL);
	}
	pcount = pcap_dispatch(handle, -1, pcap_dump, NULL);
	if (pcount < 0)
	{
		printf("ft_nmap: error reading packets from interface %s: %s", dev, pcap_geterr(handle));
		return (NULL);
	}
	pcap_close(handle);
	return (NULL);
}
