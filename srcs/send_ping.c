/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adstuder <adstuder@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 15:16:45 by adstuder          #+#    #+#             */
/*   Updated: 2021/06/23 13:20:09 by adstuder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

void print_line(struct iphdr *ip, struct icmphdr *icmp, float time)
{
  int time_precision;

  if (time < 0.1)
    time_precision = 3;
  else if (time < 10)
    time_precision = 2;
  else
    time_precision = 1;
  if (params.rdns && params.isAdressIpv4 == false)
    printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.*f ms\n", params.rdns, params.ipv4, icmp->un.echo.sequence, ip->ttl, time_precision, time);
  else if (params.rdns && params.isAdressIpv4 == true)
    printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.*f ms\n", params.rdns, icmp->un.echo.sequence, ip->ttl, time_precision, time);
  else if (params.rdns == NULL)
    printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.*f ms\n", params.ipv4, icmp->un.echo.sequence, ip->ttl, time_precision, time);
}

void terminate()
{
  int sec;
  int usec;
  gettimeofday(&params.end, NULL);
  sec = params.end.tv_sec - params.start.tv_sec;
  usec = params.end.tv_usec - params.start.tv_usec;
  int duration = (usec / 1000) + (sec * 1000); // revoir
  free_all();
  exit(EXIT_SUCCESS);
}

int send_ping()
{
  struct timeval timeout;
  struct timeval request;
  struct timeval reply;

  timeout.tv_sec = 0;
  timeout.tv_usec = 950;
  request.tv_sec = 0;
  request.tv_usec = 0;
  reply.tv_sec = 0;
  reply.tv_usec = 0;
  struct iphdr *ip;
  struct icmphdr *icmp;
  char *p_saddr;
  float time;
  int ret_recvmsg;

  if (setsockopt(params.sock, IPPROTO_UDP, IP_TTL, &params.ttl, sizeof(params.ttl)) < 0)
    print_error("setsockopt setting IP_TTL failed");
  if (setsockopt(params.sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    print_error("setsockopt setting SO_RCVTIMEO failed");
  if (setsockopt(params.sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    print_error("setsockopt failed");

  params.packet.hdr.un.echo.sequence++;
  params.packet.hdr.checksum = 0;
  params.packet.hdr.checksum = checksum(&(params.packet), sizeof(params.packet));

  gettimeofday(&request, NULL);
  if (sendto(params.sock, &(params.packet), sizeof(params.packet), 0, (struct sockaddr *)params.target, sizeof(*params.target)) <= 0)
  {
    if (params.packet.hdr.un.echo.sequence == 1)
      print_error("ping: sendto: network unavailable");
    printf("ping: sendto: network unavailable\n");
    return (-1);
  }

  ip = (struct iphdr *)params.msg.msg_iov[0].iov_base;
  icmp = (struct icmphdr *)(params.msg.msg_iov[0].iov_base + sizeof(struct iphdr));

  ft_bzero(ip, sizeof(struct iphdr));
  ft_bzero(icmp, sizeof(struct icmphdr));
  icmp->type = -1;
  icmp->code = -1;

  while (icmp->type != 0 && icmp->type != 11)
  {
    if ((ret_recvmsg = recvmsg(params.sock, &params.msg, 0)) <= 0)
    {
      if (icmp->type == 8)
        recvmsg(params.sock, &params.msg, 0);
      return(1);
    }
    if (icmp->type == 11)
    {
      p_saddr = ntop(ip->saddr);
      printf("From %s icmp_seq=%d Time to live exceeded\n", p_saddr, params.packet.hdr.un.echo.sequence);
      params.error_cnt++;
      free(p_saddr);
      return (0);
    }
  }
  gettimeofday(&reply, NULL);

  long int sec = (reply.tv_sec - request.tv_sec) * 1000000;
  long int usec = reply.tv_usec - request.tv_usec;

  time = ((float)sec + (float)usec) / 1000;
  print_line(ip, icmp, time);
}