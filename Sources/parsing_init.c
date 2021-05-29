/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/05/29 12:13:21 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
#include <sys/stat.h>
// #include <unistd.h>
// #include <math.h>
// #include <signal.h>
// #include <string.h>
// #include <ctype.h>
typedef struct stat t_stat;

void parser_init(char *file)
{
	FILE 	*fp;
	t_stat	st;

	fp = fopen(file, "r");
	stat(file, &st);

	
	// ssize_t	read;
	// size_t	len;
	// char	*line;


	// printf("%lld\n", st.st_size / 1000);

	// while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	// {
	// 	//printf("%s", line);
	// }

	fclose(fp);

}