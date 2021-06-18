/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:53:09 by slopez            #+#    #+#             */
/*   Updated: 2021/06/18 19:26:44 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>
#include <sys/stat.h>
#include <float.h>

typedef struct stat t_stat;

static void	free_obj_arrays(t_parser *parser)
{
	if (parser->v_count)
		free(parser->v);
	if (parser->vt_count)
		free(parser->vt);
	if (parser->vn_count)
		free(parser->vn);
}

static void	realloc_obj_arrays(size_t *count, size_t *alloc, size_t size, void **array)
{
	if (*count < *alloc)
		return ;

	*alloc += 1500;
	if (!(*array = realloc(*array, size * *alloc)))
		die("Erreor realloc");
}

static void get_relative_path(char path[256], char *file)
{
	char	*file_tmp;
	char 	*token;
	size_t	path_len;

	if (!(file_tmp = strdup(file)))
		die ("Error strdup file_tmp");

	memset(path, 0, 256);
	path_len = 0;

	token = strtok(file, "/");
	while (token != NULL)
	{
		if (!strcmp(token, strrchr(file_tmp, '/') + 1)) 
			break ;
		strcat(path, token);
		strcat(path, "/");
		path_len += strlen(token) + 1;
		token = strtok(NULL, "/");
	}
	free(file_tmp);
}

void		print_progress(size_t *old, size_t oyo, t_stat st)
{
	char		progress[103];
	size_t		percent = (int)((oyo / (float)st.st_size) * 100);

	if (percent >= 98)
		percent = 100;
	strcpy(progress, "[                                                                                                    ]");
	if (*old != percent)
	{
		*old = percent;
		dprintf(1, "\r");
		for (size_t i = 0; i < percent; i++)
		{
			progress[1 + i] = '-';
		}
		dprintf(1, "%s %zu%%", progress, percent);
	}
}

void 		parser_init(t_scop *scop, char *file)
{
	t_parser	parser;
	FILE 		*fp;
	t_stat		st;
	char		last_mtl[256];
	char		path[256];
	char		*line 		= NULL;
	size_t		f_count 	= 0;
	size_t		v_count 	= 0;
	size_t		vn_count 	= 0;
	size_t		vt_count 	= 0;
	ssize_t		read 		= 0;
	size_t		len 		= 0;

	if (stat(file, &st))
		die ("File not found.");
	
	if (st.st_mode & S_IFDIR)
		die ("Cannot open directory.");	

	if (!(fp = fopen(file, "r")))
		die("Cannot open file");

	parser.max = (t_vec3f){ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	parser.min = (t_vec3f){ FLT_MAX, FLT_MAX, FLT_MAX };

	memset(last_mtl, 0, 256);
	strcpy(last_mtl, "DEFAULT_NO_MATERIAL");
	get_relative_path(path, file);
	
	parser.vn_count = (st.st_size / 200);
	parser.v_count 	= (st.st_size / 200);
	parser.vt_count = (st.st_size / 200);

	if (!(parser.v 	= calloc(parser.v_count, sizeof(t_vec3f))))
		die("Error calloc v");
	if (!(parser.vn = calloc(parser.vn_count, sizeof(t_vec3f))))
		die("Error calloc vn");
	if (!(parser.vt = calloc(parser.vt_count, sizeof(t_vec2f))))
		die("Error calloc vt");

	size_t oyo = 0;
	size_t old = 0;
	while ((read = getline(&line, &len, (FILE *) fp)) != -1)
	{
		oyo += read;

		if (!strncmp(line, "mtllib ", 7))
			parser_mtl_start(scop, path, line + 7);
		else if (!strncmp(line, "usemtl ", 7))
		{
			print_progress(&old, oyo, st);
			memset(last_mtl, 0, 256);
			strcpy(last_mtl, _strtrim(line + 7));
		}
		else if (!strncmp(line, "v ", 2)) 
		{
			print_progress(&old, oyo, st);
			realloc_obj_arrays(&v_count, &parser.v_count, sizeof(t_vec3f), (void *) &parser.v);
			sscanf(line, "v %f %f %f", &parser.v[v_count].x, &parser.v[v_count].y, &parser.v[v_count].z);
			v_count++;
		} 
		else if (!strncmp(line, "vn ", 3)) 
		{
			print_progress(&old, oyo, st);
			realloc_obj_arrays(&vn_count, &parser.vn_count, sizeof(t_vec3f), (void *) &parser.vn);
			sscanf(line, "vn %f %f %f", &parser.vn[vn_count].x, &parser.vn[vn_count].y, &parser.vn[vn_count].z);
			vn_count++;
		}
		else if (!strncmp(line, "vt ", 3)) 
		{
			print_progress(&old, oyo, st);
			realloc_obj_arrays(&vt_count, &parser.vt_count, sizeof(t_vec2f), (void *) &parser.vt);
			sscanf(line, "vt %f %f", &parser.vt[vt_count].x, &parser.vt[vt_count].y);
			vt_count++;
		}
		else if (!strncmp(line, "f ", 2)) 
		{
			print_progress(&old, oyo, st);
			parse_face(&parser, &scop->objects[scop->objects_count].materials, &scop->objects[scop->objects_count].nb_mats, last_mtl, _strtrim(line + 2));
			f_count++;
		}
		// if ()
		// dprintf(1, "")
	}

	free_obj_arrays(&parser);
	free(line);
	line = NULL;
	fclose(fp);

	scop->objects[scop->objects_count].center = vec_multf(vec_add(parser.min, parser.max), .5);
	scop->objects[scop->objects_count].max = parser.max;
	scop->objects[scop->objects_count].min = parser.min;
	
	scop->objects_count++;

}