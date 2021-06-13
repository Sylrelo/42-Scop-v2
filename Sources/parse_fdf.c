/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fdf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:13:02 by slopez            #+#    #+#             */
/*   Updated: 2021/06/13 12:09:33 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <sys/stat.h>
#include <float.h>
#include <math.h>

typedef struct stat t_stat;

void        create_triangle(int **data, int i, int j, t_mat *material, t_vec3f *min, t_vec3f *max, int dir)
{
    float       buffer[8];

    _floatset(buffer, 0.00f, 8);

    buffer[0] = i;
    buffer[1] = data[i][j] * .2;
    buffer[2] = j;
    update_minmax(min, max, buffer);
    mat_push_buffer(material, buffer, 8);
    calculate_missing_normal(material);
    buffer[0] = i;
    buffer[1] = data[i][j + dir] * .2;
    buffer[2] = j + dir;
    update_minmax(min, max, buffer);
    mat_push_buffer(material, buffer, 8);
    calculate_missing_normal(material);
    buffer[0] = i + dir;
    buffer[1] = data[i + dir][j] * .2;
    buffer[2] = j;
    update_minmax(min, max, buffer);
    mat_push_buffer(material, buffer, 8);
    calculate_missing_normal(material);
}

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
    float       buffer[8];
    t_vec3f     min;
    t_vec3f     max;

    _floatset(buffer, 0.00f, 8);
    
	max = (t_vec3f){ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	min = (t_vec3f){ FLT_MAX, FLT_MAX, FLT_MAX };


	if (stat(file, &st))
		die ("File not found.");
	
	if (st.st_mode & S_IFDIR)
		die ("Cannot open directory.");	

	if (!(fp = fopen(file, "r")))
		die("Cannot open file");

    if (!(fdf_data = calloc(1, sizeof(int *) * 2048)))
        die ("Calloc error");

    
    if (!(scop->materials = calloc(1, sizeof(t_mat))))
        die ("Error creating default material");
    scop->nb_mats = 1;
    init_mat_default_values(&(scop->materials)[0]);
    strcpy((scop->materials)[0].material_name, "DEFAULT_NO_MATERIAL");

    while (i < 2048)
    {
        if (!(fdf_data[i] = calloc(1, sizeof(int) * 2048)))
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
            create_triangle(fdf_data, i, j, &scop->materials[0], &min, &max, +1);
        }
    }
    for (size_t i = row_max - 1; i > 0; i--)
    {
        if (i == 0)
            break ;
        for (size_t j = col_max - 1; j > 0; j--)
        {
            if (j == 0)
                continue ;
            create_triangle(fdf_data, i, j, &scop->materials[0], &min, &max, -1);
        }
    }
	scop->center = vec_multf(vec_add(min, max), .5);
    // exit (1);
    (void)scop;
}