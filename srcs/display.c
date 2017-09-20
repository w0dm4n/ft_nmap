/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/19 07:01:52 by frmarinh          #+#    #+#             */
/*   Updated: 2017/09/20 04:05:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static void		print_char(int i, char c)
{
	while (i--)
		printf("%c", c);
}

static bool		check_if_open(t_queue *head)
{
	t_queue		*tail	= head;
	bool		is_open	= false;

	while (is_open == false && tail && tail->port == head->port)
	{
		is_open = tail->open && tail->filtered == false;
		tail = tail->next;
	}
	return (is_open);
}

static void		display_ports(t_queue *queues, bool open)
{
	t_queue		*ptr		= NULL;
	char		*proto		= NULL;
	int			scan_types	= 0;
	int			opcl		= 0;
	int			filtered	= 0;

	print_char(45, '_');
	if (open) {
		printf("OPENED PORTS");
	} else {
		printf("CLOSED PORTS");
	}
	print_char(45, '_');
	printf("\n");
	printf("PORT\tSERVICE\t\tSCAN TYPE(STATUS)\t\t\tHOST\n");
	while (queues)
	{
		ptr = NULL;
		printf("%d", queues->port);
		print_char(1, '\t');
		printf("Unassigned");
		print_char(1, '\t');

		ptr = queues;
		scan_types = 0;
		opcl = 0;
		while (ptr && ptr->port == queues->port) {
			if (!ft_strcmp(ptr->scan, "SYN")) {
				scan_types |= 0x0001;
				if (ptr->filtered)
					filtered |= 0x0001;
				else if (ptr->open)
					opcl |= 0x0001;
			}
			else if (!ft_strcmp(ptr->scan, "NULL")) {
				scan_types |= 0x0002;
				if (ptr->open)
					opcl |= 0x0002;
			}
			else if (!ft_strcmp(ptr->scan, "FIN")) {
				scan_types |= 0x0004;
				if (ptr->open)
					opcl |= 0x0004;
			}
			else if (!ft_strcmp(ptr->scan, "XMAS")) {
				scan_types |= 0x0008;
				if (ptr->open)
					opcl |= 0x0008;
			}
			else if (!ft_strcmp(ptr->scan, "ACK")) {
				scan_types |= 0x0010;
				if (ptr->open)
					opcl |= 0x0010;
			}
			else if (!ft_strcmp(ptr->scan, "UDP")) {
				scan_types |= 0x0020;
				if (ptr->open)
					opcl |= 0x0020;
			}
			ptr = ptr->next;
		}

		int i = 0;
		if (scan_types & 0x0001) {
			printf(((i == 0) ? "%s" : " %s"), "SYN");
			printf("(%s)", (opcl & 0x0001 ? "Open" : (filtered & 0x0001 ? "Filtered" : "Closed"))); i++; }
		if (scan_types & 0x0002) {
			printf(((i == 0) ? "%s" : " %s"), "NULL");
			printf("(%s)", ((opcl & 0x0002) ? "Open" : "Closed")); i++; }
		if (scan_types & 0x0004) {
			printf(((i == 0) ? "%s" : " %s"), "FIN");
			printf("(%s)", ((opcl & 0x0004) ? "Open" : "Closed")); i++; }
		if (scan_types & 0x0008) {
			printf(((i == 0) ? "%s" : " %s"), "XMAS");
			printf("(%s)", ((opcl & 0x0008) ? "Open" : "Closed")); i++; }
		if (scan_types & 0x0010) {
			printf(((i == 0) ? "%s" : " %s"), "ACK");
			printf("(%s)", ((opcl & 0x0010) ? "Filtered" : "Unfiltered")); i++; }
		if (scan_types & 0x0020) {
			printf(((i == 0) ? "%s" : " %s"), "UDP");
			printf("(%s)", ((opcl & 0x0020) ? "Open|Filtered" : "Closed")); i++; }

		print_char((i < 4) ? 4 - i : 1, '\t');
		printf("%s\n", queues->host);
		queues = ptr;
}

static void		parse_not_done(t_queue *queues)
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
			else { // other type are all opened when there is no answer (at least we think)
				queues->open = true;
			}
		}
		queues = queues->next;
	}
}

static t_queue	*sort_by_port(t_queue *head)
{
	t_queue		*tail;
	t_queue		*next;
	t_queue		*ptr;

	tail = head;	
	while (tail)
	{
		next = tail->next;
		if (next && next->port < tail->port)
		{
			if (next->port < head->port) {
				tail->next = next->next;
				next->next = head;
				head = next;
			} else {
				ptr = head;
				while (ptr->next && ptr != next) {
					if (next->port < ptr->next->port) {
						tail->next = next->next;
						next->next = ptr->next;
						ptr->next = next;
						break ;
					}
					ptr = ptr->next;
				}
			}
		}
		else
			tail = tail->next;
	}
	return head;
}

static void		sort_open_close(t_queue *head, t_queue **open, t_queue **close)
{
	t_queue		*tail = head;
	t_queue		*ptr;
	bool		is_open = false;

	while (head)
	{
		is_open = head->open && !head->filtered;
		tail = head;
		while (tail->next && tail->next->port == head->port)
		{
			if (!is_open)
				is_open = tail->next->open && !tail->next->filtered;
			tail = tail->next;
		}
		ptr = tail->next;
		if (is_open)
		{
			tail->next = *open;
			*open = head;
		}
		else
		{
			tail->next = *close;
			*close = head;
		}
		head = ptr;
	}
}

static t_queue	*sort_by_address(t_queue *queue)
{
	t_queue		*head	= queue;
	t_queue		*tail	= NULL;

	while (head)
	{
		tail = head;
		while (tail->next && tail->next->port == head->port)
			tail = tail->next;
		while (head != tail)
		{
			t_queue		*end	= head;
			while (end->next && end->next != tail->next && && !ft_strcmp(end->next->host, head->host))
				end = end->next;
			t_queue		*ptr	= end;
			while (ptr->next != tail->next)
			{
				if (!ft_strcmp(ptr->next->host, head->host))
				{
					
					
				}
				ptr = ptr->next;
			}
			head = end;
		}
	}
}

static void		display_handler()
{
	t_queue		*queues		= all_queues;
	t_queue		*open_q		= NULL;
	t_queue		*close_q	= NULL;

	queues = sort_by_port(queues);
	queues = sort_by_address(queues);
	parse_not_done(queues);
	sort_open_close(queues, &open_q, &close_q);
	if (open_q) {
		display_ports(open_q, true);
		printf("\n");
	}
	if (close_q)
		display_ports(close_q, false);
	exit(0);
}

void			init_display()
{
	signal(SIGALRM, display_handler);
	alarm(EXECUTION_TIME);
}
