/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 01:13:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 13:20:50 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void generate_filepath(char filepath[256], char path[256], char *file)
{
    file = _strtrim(file);
	memset(filepath, 0, 256);
    strcat(filepath, path);
    strcat(filepath, file);
    filepath[strlen(path) + strlen(file)] = 0;
}

static int  find_already_loaded(t_textures *textures, size_t texture_count, char *file)
{
    size_t  i = 0;

    while (i < texture_count)
    {
        if (!(strncmp(textures[i].filename, file, strlen(textures[i].filename))))
            return (i);
        i++;
    }
    return (-1);
}

static void realloc_textures(size_t count, void **textures)
{
    const int ocount = (count / 10) + 1;
    const int ncount = ((count + 1) / 10) + 1;

	if (count == 0)
    {
        if (!(*textures = calloc(10, sizeof(t_textures))))
            die ("Textures array initial allocation failed");
    }
    if (ncount > ocount)
    {
        if (!(*textures = realloc(*textures, ncount * 10 + sizeof(t_textures))))
            die ("Textures array realloc failed");
    }
}

void        generate_gl_texture(t_scop *scop, size_t width, size_t height, void *tex, char *file, unsigned int type)
{
    realloc_textures(scop->textures_count, (void *) &scop->textures);

    glGenTextures(1, &scop->textures[scop->textures_count].gl_texture);
    glBindTexture(GL_TEXTURE_2D, scop->textures[scop->textures_count].gl_texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, type, tex);
    glGenerateMipmap(GL_TEXTURE_2D);

    scop->textures[scop->textures_count].width  = width;
    scop->textures[scop->textures_count].height = height;
    strcpy(scop->textures[scop->textures_count].filename, file);
    scop->textures_count++;

}

void        parse_texture(t_scop *scop, t_mat *material, char path[256], char *file)
{
    const int texture_id = find_already_loaded(scop->textures, scop->textures_count, file);
    int             width;
    int             height;
    int             channels;
    unsigned char   *image;
    char            filepath[256];

    generate_filepath(filepath, path, file);
    
    if (texture_id != -1)
    {
        material->tex_id = texture_id;
        return ;
    }

    if (!(image = stbi_load(filepath, &width, &height, &channels, 0)))
    {
		printf("\033[0;31m      Error while parsing texture file \033[1m%s\033[0m\033[0;31m skipping.\033[0m\n", filepath);
        return ;
    }
    
    printf ("      Texture \033[1m%s\033[0m loaded.\n", filepath);

    generate_gl_texture(scop, width, height, image, file, GL_UNSIGNED_BYTE);
    material->tex_id = scop->textures_count - 1;
    stbi_image_free(image);
}