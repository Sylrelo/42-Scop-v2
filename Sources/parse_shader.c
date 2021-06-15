/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 14:31:59 by slopez            #+#    #+#             */
/*   Updated: 2021/06/15 11:30:14 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *load_file(char *file, int *size)
{
	char		*data;
	char 		*line;
	FILE 		*fp;
	ssize_t		read;
	size_t		len;

    printf("[OpenGL] Loading %s\n", file);
	*size = 0;
	len = 0;
	data = NULL;
	line = NULL;
	if ((fp = fopen(file, "r")) == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
	rewind(fp);
	if (!(data = calloc(*size + 1, sizeof(char))))
	{
		printf("[Shader] Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	while ((read = getline(&line, &len, fp)) != -1) {
		strcat(data, line);
	}
	free(line);
	return (data);
}

static int  load_shader(char *file, unsigned int type)
{
	int			len;
	char		*source;
	int			shader;
	int			success;
	char		log[512];

	source = load_file(file, &len);
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar * const*)&source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("[Shader] Compilation failed %d\n%s\n", type, log);
		exit(EXIT_FAILURE);
	}
    printf("[OpenGL] Create shader %u\n", type);
	free(source);
	return (shader);
}

int         create_shader_program(char *file_vertex, char *file_fragment)
{
    int			program;
	int			success;
	char		log[512];
	const int 	vertex 		= load_shader(file_vertex, GL_VERTEX_SHADER);
	const int 	fragment 	= load_shader(file_fragment, GL_FRAGMENT_SHADER);
	const int 	geometry 	= load_shader("Shaders/geometry.glsl", GL_GEOMETRY_SHADER);

    printf("[OpenGL] Create program\n");
    program = glCreateProgram();
	glAttachShader(program, geometry);
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		printf("[Program] Linking failed\n%s\n", log);
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	return (program);
}

