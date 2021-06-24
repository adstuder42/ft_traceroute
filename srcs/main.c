/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adstuder <adstuder@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 15:19:17 by adstuder          #+#    #+#             */
/*   Updated: 2021/06/24 16:16:41 by adstuder         ###   ########.fr       */
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

int ft_ping(int argc, char **argv)
{
  init_params();
  get_flags(argc, argv);
  get_target(params.address);
  gettimeofday(&params.start, NULL);
  set_params();
  return (0);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "ping: usage error: destination adress required\n");
    exit(EXIT_FAILURE);
  }
  params.ttl = 1;
  int i = 3;
 // int ret = -1;
  //while (ret != 1)
  while (i > 0)
  {
    params.ttl++;
    params.time = 0.0;
  ft_ping(argc, argv);
  printf("%.*f\n",3,params.time);
  i--;
  } 
  return (0);
}