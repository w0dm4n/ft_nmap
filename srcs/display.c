/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/19 07:01:52 by frmarinh          #+#    #+#             */
/*   Updated: 2017/10/07 19:53:06 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"
#include <stdio.h>

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

static int		count_scan_type()
{
	t_scan_type		*scans	= globals->nmap->scans;
	int 			i		= 0;
	while (scans)
	{
		i++;
		scans = scans->next;
	}
	return i;
}

static char		*leaks_free_strjoin(char *s1, char *s2)
{
	char		*ret;
	size_t		i = 0;
	size_t		j = 0;

	while (s1 && s1[i])
		i++;
	while (s2 && s2[j])
		j++;
	if ((ret = (char *)malloc(sizeof(*ret) * (i + j + 1))) == NULL)
		return (NULL);
	i = 0;
	while (s1 && s1[i]) {
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j]) {
		ret[i + j] = s2[j];
		j++;
	}
	ret[i + j] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (ret);
}

static void		my_asprintf(char **str, char *dup, int space)
{
	char		*ptr;
	size_t		len = ft_strlen(dup) + space + 1;

	*str = (char *)malloc(sizeof(**str) * len);
	if (*str) {
		ptr = *str;
		if (space)
			*ptr++ = ' ';
		while (*dup)
			*ptr++ = *dup++;
	}
	*ptr = '\0';
}

static int		calc_underscore_length(int count, char *text)
{
	int			underscore;

	underscore = (TAB_WIDTH > 4) ? 5 + TAB_WIDTH % 5 : 5 + 5 % TAB_WIDTH;
	printf("%d ", underscore);
	underscore += (10 + TAB_WIDTH - 10 / TAB_WIDTH);
	printf("%d ", underscore);
	underscore += (count > 1) ? ((count * 15 + TAB_WIDTH - (count * 15) / TAB_WIDTH)) : (17 + TAB_WIDTH - 17 / TAB_WIDTH);
	printf("%d ", underscore);
	underscore += (15 - ft_strlen(text));
	printf("%d ", underscore);
	underscore /= 2;
	return (underscore);
}

static void		display_ports(t_queue *queues, char *text)
{
	static char	*scan_name[] = { "SYN", "NULL", "FIN", "XMAS", "ACK", "UDP", NULL };
	static int	scan_value[] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0000 };
	t_queue		*ptr		= NULL;
	char		*proto		= NULL;
	int			scan_types	= 0;
	int			opcl		= 0;
	int			filtered	= 0;
	int			count		= count_scan_type();
	int			underscore	= calc_underscore_length(count, text);

	printf("\n");
	print_char(underscore, '_');
	printf("%s", text);
	print_char(underscore, '_');
	printf("\n");
	printf("%-5s\t%-10s\t%-17s", "PORT", "SERVICE", "SCAN_TYPE(STATUS)");
	print_char(count * 1.5, '\t');
	printf("%-15s\n", "HOST");
	while (queues)
	{
		ptr = NULL;
		printf("%-5d\t", queues->port);
		printf("%-10s\t", queues->service);

		ptr = queues;
		scan_types = 0;
		opcl = 0;
		int i = 0;
		while (ptr && ptr->port == queues->port && !ft_strcmp(ptr->host, queues->host)) {
			while (scan_name[i]) {
				if (!ft_strcmp(ptr->scan, scan_name[i])) {
					scan_types |= scan_value[i];
					if (ptr->filtered)
						filtered |= scan_value[i];
					else if (ptr->open)
						opcl |= scan_value[i];
					break ;
				}
				i++;
			}
			i = 0;
			ptr = ptr->next;
		}

		int			pos = 0;
		char		*p1 = NULL;
		char		*p2 = NULL;
		char		*p3 = NULL;
		char		*ret = NULL;
		i = 0;
		while (scan_name[i]) {
			if (scan_types & scan_value[i]) {
				my_asprintf(&p1, scan_name[i], pos);
				my_asprintf(&p2, (opcl & scan_value[i] ? "(Open)" :
								  (filtered & scan_value[i] ? "(Filtered)" : "(Closed)")), 0);
				pos = 1;
				p3 = leaks_free_strjoin(p1, p2);
				p1 = ret;
				ret = leaks_free_strjoin(p1, p3);
			}
			i++;
		}

		printf("%-17s\t", ret);
		printf("%-15s\n", queues->host);
		if (ret)
			free(ret);
		queues = ptr;
	}
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

static t_queue	*sort_by_port(t_queue *head)
{
	t_queue		*first	= head;
	t_queue		*ptr	= NULL;

	while (head)
	{
		if (head->next && head->next->port < head->port)
		{
			/* If the next port is lower than the first one */
			if (head->next->port < first->port) {
				ptr = head->next;
				head->next = ptr->next;
				ptr->next = first;
				head = ptr;
				first = ptr;
			} else {
				/* Else we check from the first */
				ptr = first;
				while (ptr->next && ptr != head->next) {
					if (head->next->port < ptr->next->port) {
						t_queue *swap = head->next;
						head->next = swap->next;
						swap->next = ptr->next;
						ptr->next = swap;
						break ;
					}
					ptr = ptr->next;
				}
			}
		}
		else
			head = head->next;
	}
	return first;
}

static t_queue	*sort_by_address(t_queue *queue)
{
	t_queue		*tail	= queue;
	t_queue		*end	= NULL;
	t_queue		*head	= NULL;
	t_queue		*save	= NULL;
	t_queue		*ptr	= NULL;

	while (tail && tail->next)
	{
		/* We get the range (tail to end) of the queues who got the same port */
		end = tail;
		while (end->next && end->next->port == tail->port)
			end = end->next;
		/* if we got a range > 1 */
		if (end != tail) {
			/* We get the range (tail to head) of the queue who got the same host */
			head = tail;
			while (head->next != end->next && !ft_strcmp(head->next->host, tail->host))
				head = head->next;
			/* We now loop througth the port range */
			ptr = head;
			while (ptr->next && ptr->next != end->next)
			{
				/* If our pointeur host equals our tail host, */
				if (!ft_strcmp(ptr->next->host, tail->host))
				{
					/* We swap pointeurs so the same hosts are next to each others */
					save = ptr->next;
					ptr->next = save->next;
					save->next = head->next;
					head->next = save;
					head = save;
				}
				else
					ptr = ptr->next;
			}
			tail = head;
		}
		tail = tail->next;
	}
	return (queue);
}

void		display_handler()
{
	t_queue		*queues		= globals->all_queues;
	t_queue		*open_q		= NULL;
	t_queue		*close_q	= NULL;
	t_flag		*closed		= get_flag("closed");
	float		ms_time		= 0;

	pthread_mutex_lock(&globals->id_lock);
	queues = sort_by_port(queues);
	queues = sort_by_address(queues);
	parse_not_done(queues);
	sort_open_close(queues, &open_q, &close_q);
	if (open_q)
		display_ports(open_q, "OPENED_PORTS");
	if (close_q && closed)
		display_ports(close_q, "CLOSED_PORTS");
	ms_time = ((globals->end_time.tv_sec - globals->start_time.tv_sec) * 1000000 + globals->end_time.tv_usec) - globals->start_time.tv_usec;
	float time_value = ms_time / 1000;
	printf ("Execution time: %.3fms\n", time_value);
	free_datas(globals->nmap);
	pthread_mutex_unlock(&globals->id_lock);
	exit(0);
}

void		init_display(t_nmap *nmap)
{
	struct sigaction	act;
	t_flag				*timeout = get_flag("timeout");

	gettimeofday (&globals->end_time, NULL);
	globals->nmap = nmap;
	ft_memset (&act, 0, sizeof(struct sigaction));
	act.sa_sigaction = &display_handler;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGALRM, &act, NULL) < 0) {
		exit(0);
	}
	printf("Sniffing network for %d ms.. and waiting for answers (%ds)\n",
			(timeout && timeout->value) ? ft_atoi(timeout->value) : DEFAULT_TIMEOUT,
			EXECUTION_TIME);
	alarm(EXECUTION_TIME);
}
