/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/19 07:01:52 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/19 07:01:57 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void				print_char(int i, char c)
{
	while (i--)
		printf("%c", c);
}

static void				display_ports(t_queue *queues, bool open)
{
	print_char(40, '_');
	if (open) {
		printf("OPENED PORTS");
	} else {
		printf("CLOSED PORTS");
	}
	print_char(40, '_');
	printf("\n");
	printf("PORT\t\tPROTOCOL\t\tSCAN TYPE\t\tSERVICE\t\tSTATUS\t\tHOST\n");
	while (queues)
	{
		if ((open && queues->open) || (!open && !queues->open)) {
			printf("%d", queues->port);
			print_char(2, '\t');
			printf("tcp");
			print_char(3, '\t');
			printf("%s", queues->scan);
			print_char(5, '\t');
			if (open) {
				printf("open");
			}
			else {
				printf("close");
			}
			print_char(2, '\t');
			printf("%s\n", queues->host);
		}
		queues = queues->next;
	}
}

static void				parse_not_done(t_queue *queues)
{
	while (queues)
	{
		if (!queues->done) {
			if (!ft_strcmp(queues->scan, "SYN")) {
				queues->open = false;
				queues->filtered = true;
			}
			else if (!ft_strcmp(queues->scan, "ACK")) {
				queues->open = false;
				queues->filtered = true;
			}
			else if (!ft_strcmp(queues->scan, "UDP")) {
				queues->open = true;
			}
		}
		queues = queues->next;
	}
}

static void				display_handler()
{
	t_queue		*queues = all_queues;
	parse_not_done(queues);
	display_ports(queues, true);
	printf("\n");
	display_ports(queues, false);
	exit(0);
}

void			init_display()
{
	signal(SIGALRM, display_handler);
	alarm(EXECUTION_TIME);
}
