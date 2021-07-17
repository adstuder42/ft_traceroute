/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adstuder <adstuder@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 15:19:17 by adstuder          #+#    #+#             */
/*   Updated: 2021/07/10 15:17:15 by adstuder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

t_params params;

void usage()
{
  printf("Usage\n  ft_ping [options] <destination>\n\nOptions:\n  -v                 verbose output\n  -h                 print help and exit\n\n");
  free_all();
  exit(EXIT_SUCCESS);
}

void get_flags(int argc, char **argv)
{
  int i;
  int j;

  i = 1;
  while (i < argc)
  {
    j = 0;
    if (argv[i][j] == '-')
    {
      j++;
      while (argv[i][j] != '\0')
      {
        if (argv[i][j] == 'v')
          params.flag_v = 1;
        else if (argv[i][j] == 'h')
          params.flag_h = 1;
        else
          usage();
        j++;
      }
    }
    else
      params.address = ft_strdup(argv[i]);
    i++;
  }
  if (params.flag_h == 1)
    usage();
  if (params.address == NULL)
    print_error("ping: usage error: destination adress required");
}

int ft_ping()
{

  gettimeofday(&params.start, NULL);
  set_params();
  return (0);
}

int print_time()
{
  printf(" %.*f ms", 3, params.time);
  return (0);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "traceroute : usage error: destination adress required\n");
    exit(EXIT_FAILURE);
  }
  int i = 3;
  bool first_line = true; // int ret = -1;
  char *prev_ip = NULL;
  //while (ret != 1)
  init_params();
  get_flags(argc, argv);
  get_target(params.address);
  params.ttl = 1;
  while (params.ttl <= 30)
  {
    while (i > 0)
    {
      params.time = 0.0;
      ft_ping();
      prev_ip = ft_strdup(params.reply_ip);
      if (first_line == true)
      {
        // vérifier 60 bytes packets
        printf("traceroute to %s (%s), 30 hops max, 60 byte packets\n", params.address, params.ipv4);
        printf(" %d  _gateway (%s) ", params.ttl, params.reply_ip);
        first_line = false;
      }
      if (ft_strcmp(prev_ip, params.reply_ip) != 0)
        printf(" (%s) ", params.reply_ip);
      if (params.reply_ip != NULL)
      {
        free(params.reply_ip);
        params.reply_ip = NULL;
      }
      print_time();

      //    printf("%.*f\n", 3, params.time);

      if (prev_ip != NULL)
      {
        free(prev_ip);
        prev_ip = NULL;
      }
      i--;
    }
    params.ttl++;
  }
  return (0);
}