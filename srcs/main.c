/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adstuder <adstuder@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 15:19:17 by adstuder          #+#    #+#             */
/*   Updated: 2021/06/23 12:56:54 by adstuder         ###   ########.fr       */
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
        if (argv[i][j] == 'h')
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
    print_error("usage error: destination adress required");
}

float ft_ping(int argc, char **argv)
{
  init_params();
  get_flags(argc, argv);
  get_target(params.address);
  gettimeofday(&params.start, NULL);
  set_params();
  send_ping();
  return (0);
}


int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "ping: usage error: destination adress required\n");
    exit(EXIT_FAILURE);
  }
  init_params();
  get_flags(argc, argv);
  get_target(params.address);
  gettimeofday(&params.start, NULL);
  set_params();
  send_ping();
  return (0);
}