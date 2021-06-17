/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/17 13:38:30 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <math.h>

#include <sys/stat.h> // deplacer

void	display_loop(t_scop *scop)
{
	t_mat		material;
	t_objects		object;
	size_t	mat_i 	= 0;
	size_t	obj_i 	= 0;
	size_t	offset 	= 0;
	size_t offset_obj = 0;

	GLuint glMatView 		= glGetUniformLocation(scop->ogl.p_render, "View");
	GLuint glMatPersp 		= glGetUniformLocation(scop->ogl.p_render, "Persp");
	GLuint glMatModel 		= glGetUniformLocation(scop->ogl.p_render, "Model");
	uint32_t loc_kd			= glGetUniformLocation(scop->ogl.p_render, "kd");
	uint32_t loc_ka			= glGetUniformLocation(scop->ogl.p_render, "ka");
	uint32_t loc_textured 	= glGetUniformLocation(scop->ogl.p_render, "textured");
	uint32_t loc_lightpos 	= glGetUniformLocation(scop->ogl.p_render, "lightPos");
	uint32_t loc_lightcol 	= glGetUniformLocation(scop->ogl.p_render, "lightColor");

	uint32_t loc_utime 		= glGetUniformLocation(scop->ogl.p_render, "glfw_time");
	uint32_t loc_options 	= glGetUniformLocation(scop->ogl.p_render, "glfw_options");


	t_mat4	mat_perspective = m4_perspective(1.0472, (float) scop->width / (float) scop->height, 0.00001f, 1000.0f);
	t_mat4	mat_view 		= m4_init();
	t_mat4	mat_model 		= m4_init();


	// mat_perspective = m4_orthogonal(-10, 100.f, -100, 100, -100, 100);
	glUniformMatrix4fv(glMatPersp, 1, GL_FALSE, mat_perspective.value[0]);

	m4_print(mat_perspective);

	float a = 0;

	// glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glUniform3f(loc_lightpos, 0, 0, 0);
	glUniform3f(loc_lightcol, 1, 1, 1);

	t_mat4	test = m4_look_at((t_vec3f){0, 0, -20}, (t_vec3f){0, .5, 10});
	
	m4_print(test);;


	float glfw_time = glfwGetTime();
	const float base_time = glfw_time;
	glfw_time -= base_time;

	printf("%f\n", glfw_time);
	while (!glfwWindowShouldClose(scop->window))
	{
		obj_i 	= 0;
		offset 	= 0;
		a 		+= 0.010;
		offset_obj = 0;



		test = m4_look_at((t_vec3f){-5, 3, 5}, (t_vec3f){0, 0, 0});


		glfw_time = glfwGetTime() - base_time;

		if (glfw_time > .5)
			glUniform1i(loc_options, 0);
		else 
			glUniform1i(loc_options, 1);

		glUniform1f(loc_utime, glfw_time);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		handle_keyboard(scop->window, scop->keys);
		handle_mouse(scop->window,  &scop->cam_rot);
		handle_transformations(scop->keys, &scop->cam_pos, &scop->cam_rot);

		mat_view = m4_viewmat(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z, 
							m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));

		mat_model = m4_mult(
						m4_mult(
							m4_scale(1, 1, 1), 
							m4_rotation_around_center(scop->center, 0, 0, 0)), 
						m4_translate(0, 0, 0)
					);

		// mat_view = test;
		glUniformMatrix4fv(glMatView, 1, GL_FALSE, mat_view.value[0]);
		glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat_model.value[0]);
		
		obj_i = 0;

		while (obj_i < scop->objects_count)
		{
			mat_i = 0;
			offset = 0;
			object = scop->objects[obj_i];
			
			if (obj_i > 0)
				offset_obj = scop->objects[obj_i - 1].offset;
			while (mat_i < object.nb_mats)
			{
				material = object.materials[mat_i];
				if (mat_i > 0)
					offset += object.materials[mat_i - 1].gl_buffer_size;

				glUniform3f(loc_kd, material.kd.x, material.kd.y, material.kd.z);
				glUniform3f(loc_ka, material.ka.x, material.ka.y, material.ka.z);
				if (material.tex_id != -1)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, scop->textures[material.tex_id].gl_texture);
					glUniform1i(loc_textured, 1);
				}
				else
					glUniform1i(loc_textured, 0);
				glDrawArrays(GL_TRIANGLES, (offset_obj + offset) / 8, (material.gl_buffer_size) / 8);
				mat_i++;
			}
			obj_i++;
		}
		
		glfwSwapBuffers(scop->window);

		glfwPollEvents();
		// printf("%f\n", 1 / ((glfwGetTime() - base_time) -  glfw_time));

	}
}

int 	main(int argc, char *argv[])
{
	t_scop	*scop;

	scop = calloc(1, sizeof(t_scop));

	scop->textures_count = 0;
	scop->textures = NULL;

	scop->cam_pos = (t_vec3f) {0, 0, 0};
	scop->cam_rot = (t_vec3f) {0, 0, 0};
	
	scop->width = 1280;
	scop->height = 720;

	memset(scop->keys, 0, sizeof(uint32_t) * 348);
		
	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);
	init_depth_map(scop);


	//deplacer
	scop->objects_count = 0;
	scop->objects = calloc(argc, sizeof(t_objects));

	printf("[Scop] Starting parser\n\n");

	struct stat		st;
	int				st_result;

	float start = glfwGetTime();
	const float diff = start;
	start -= diff;
	
	while (--argc > 0)
	{
		st_result = stat(argv[argc], &st);
		if (st_result)
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mnot found, skipping.\033[0m\n\n", argv[argc]);
			continue ;
		}
		if (!strstr(argv[argc], ".obj"))
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mextension is invalid, skipping.\033[0m\n\n", argv[argc]);
			continue ;
		}
		if (st.st_mode & S_IFDIR)
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mis a directory, skipping.\033[0m\n\n", argv[argc]);
			continue ;
		}
		printf("+ Loading \033[1m%s\033[0m... (%.2f Mb)\n", argv[argc], st.st_size * 0.0000009765625);
		parser_init(scop, argv[argc]);
		printf("\n");
	}
	printf("Time taken : %.2f\n", (glfwGetTime() - diff) - start);

	//
	
	printf("[Scop] Starting OpenGL Buffer initialization\n");
	init_opengl_buffer_multi(scop);
	printf("[Scop] Opening Window\n");
	glfwShowWindow(scop->window);

	scop->ogl.p_render = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl", "Shaders/geometry_classic.glsl");
	scop->ogl.p_ddepthmap = create_shader_program("Shaders/v.shadowmap_directional.glsl", "Shaders/f.shadowmap_directional.glsl", NULL);

	glUseProgram(scop->ogl.p_render);

	printf("[Scop] Ready\n");

	scop->cam_pos.x -= scop->center.x * .5;
	scop->cam_pos.y -= scop->center.y * .5;
	scop->cam_pos.z -= scop->center.z + 8;


	display_loop(scop);

	clean_exit(scop);
	(void)argc;
}