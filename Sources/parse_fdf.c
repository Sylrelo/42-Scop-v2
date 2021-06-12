/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fdf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:13:02 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 01:50:08 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <sys/stat.h>
#include <float.h>
#include <math.h>

typedef struct stat t_stat;

void 		parser_init_fdf(t_scop *scop, char *file)
{
	FILE 		*fp;
	t_stat		st;
	char		*line 		= NULL;
    char        *token      = NULL;
	size_t		len 		= 0;
	ssize_t		read 		= 0;
    size_t      col_max     = 0;
    size_t      col_curr    = 0;
    size_t      row_curr    = 0;
    size_t      row_max     = 0;
    int         **fdf_data  = NULL;
    size_t      i           = 0;

	if (stat(file, &st))
		die ("File not found.");
	
	if (st.st_mode & S_IFDIR)
		die ("Cannot open directory.");	

	if (!(fp = fopen(file, "r")))
		die("Cannot open file");

    if (!(fdf_data = calloc(1, sizeof(int *) * 256)))
        die ("Calloc error");
    while (i < 256)
    {
        if (!(fdf_data[i] = calloc(1, sizeof(int) * 256)))
            die ("Calloc[i] error");
        i++;
    }

    while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
        col_curr = 0;
        token = strtok(line, " ");
        while (token != NULL)
        {
            fdf_data[row_curr][col_curr] = atoi(_strtrim(token));
            col_curr++;
            col_max = fmax(col_curr, col_max);
		    token = strtok(NULL, " ");
        }
        if (col_curr != col_max)
        {
            die ("Error file, free all");
        }
        row_curr++;
        row_max = fmax(row_curr, row_max);
    }


    for (size_t i = 0; i < row_max; i++)
    {
        if (i == row_max - 1)
            break ;
        for (size_t j = 0; j < col_max; j++)
        {
            if (j == col_max - 1)
                continue ;
            printf("tri (%d, %d, %d) ", fdf_data[i][j], fdf_data[i][j + 1], fdf_data[i + 1][j]);
        }

        // printf("tri (%d %d %d) ", fdf_data[i][0], fdf_data[i][1], fdf_data[i + 1][0]);

        printf("\n");
    }
    
    exit (1);
    (void)scop;
}