/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 14:59:03 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h> // deplacer
#include <float.h>


#include "stb_image.h"

void	get_uniforms_location(t_uniforms *uniform, uint32_t program)
{
	uniform->m4_projection 	= glGetUniformLocation(program, "Persp");
	uniform->m4_view 		= glGetUniformLocation(program, "View");
	uniform->m4_model 		= glGetUniformLocation(program, "Model");
	uniform->kd 			= glGetUniformLocation(program, "kd");
	uniform->ka 			= glGetUniformLocation(program, "ka");
	uniform->textured 		= glGetUniformLocation(program, "textured");
	uniform->glfw_time 		= glGetUniformLocation(program, "glfw_time");
	uniform->glfw_options 	= glGetUniformLocation(program, "glfw_options");

	uniform->tex_size 		= glGetUniformLocation(program, "tex_size");
	uniform->obj_max_y 		= glGetUniformLocation(program, "obj_max_y");
	uniform->obj_center 	= glGetUniformLocation(program, "obj_center");

	uniform->tex_basic 		= glGetUniformLocation(program, "basic_texture");
	uniform->tex_object 	= glGetUniformLocation(program, "ourTexture");

}

void	send_default_uniforms(t_uniforms uniform, float glfw_time, t_scop *scop)
{
	t_mat4	mat_view 		= m4_init();
	t_mat4	mat_perspective;

	if (glfw_time > .5)
	{
		glUniform1i(uniform.glfw_options, 0);
		glUniform1f(uniform.glfw_time, glfw_time);
	}
	else
	{
		glUniform1i(uniform.glfw_options, 1);
		glUniform1f(uniform.glfw_time, .5);
	}

	mat_view = m4_viewmat(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z, m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));
	glUniformMatrix4fv(uniform.m4_view, 1, GL_FALSE, mat_view.value[0]);
	mat_perspective = m4_perspective(1.0472, (float) scop->width / (float) scop->height, 0.00001f, 1000.0f);
	glUniformMatrix4fv(uniform.m4_projection, 1, GL_FALSE, mat_perspective.value[0]);

	glUniform1i(uniform.tex_basic, 0);
	glUniform1i(uniform.tex_object, 1);
}

void	send_model_data(t_uniforms uniform, t_objects object)
{
	const t_mat4	mat_scale 		= m4_scale(1, 1, 1);
	const t_mat4	mat_translate 	= m4_translate(object.pos.x, object.pos.y, object.pos.z);
	const t_mat4	mat_rotate		= m4_rotation_around_center(object.center, object.rot.x, object.rot.y, object.rot.z);
	const t_mat4	mat_model 		= m4_mult(m4_mult(mat_scale, mat_rotate), mat_translate);

	glUniformMatrix4fv(uniform.m4_model, 1, GL_FALSE, mat_model.value[0]);
	glUniform1f(uniform.obj_max_y, object.max.y);
	glUniform3f(uniform.obj_center, object.center.x, object.center.y, object.center.z);
}

void	send_material_data(t_uniforms uniform, t_textures *textures, t_mat material, int s_texturing)
{
	t_textures	tex;

	glUniform3f(uniform.kd, material.kd.x, material.kd.y, material.kd.z);
	glUniform3f(uniform.ka, material.ka.x, material.ka.y, material.ka.z);

	if ((s_texturing == 1 || s_texturing == 2) && material.tex_id != -1)
	{
		tex = textures[material.tex_id];
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, tex.gl_texture);
		glUniform1i(uniform.textured, 1);
		glUniform2f(uniform.tex_size, tex.width, tex.height);
	}
	else if (s_texturing != 2)
		glUniform1i(uniform.textured, s_texturing != 1 ? s_texturing : 0);
}

void	display_loop(t_scop *scop)
{
	t_uniforms 		uniform;
	float		glfw_time;
	float		base_time;
	size_t		offset_mat;
	size_t		offset_obj;
	t_objects	object;
	t_mat		material;

	float			rot = 0;
	
	get_uniforms_location(&scop->ogl.u_render, scop->ogl.p_render);

	uniform = scop->ogl.u_render;

	glfw_time = glfwGetTime();
	base_time = glfw_time;
	glfw_time -= base_time;
	
    glUseProgram(scop->ogl.p_render);

	while (!glfwWindowShouldClose(scop->window))
	{
		offset_mat 		= 0;
		offset_obj 		= 0;
		rot				+= 0.010;
		
		glfw_time = glfwGetTime() - base_time;
		
		handle_keyboard(scop->window, scop->keys, &scop->ogl.s_texturing);
		handle_mouse(scop->window, &scop->cam_rot);
		handle_transformations(scop);
    	
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, scop->width * 2, scop->height * 2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		send_default_uniforms(uniform, glfw_time, scop);



		for (size_t obj_i = 0; obj_i < scop->objects_count; obj_i++)
		{
			offset_mat 	= 0;
			object 		= scop->objects[obj_i];
			
			if (obj_i > 0)
				offset_obj = scop->objects[obj_i - 1].offset;
			for (size_t mat_i = 0; mat_i < object.nb_mats; mat_i++)
			{
				send_model_data(uniform, object);
				material = object.materials[mat_i];
				if (mat_i > 0)
					offset_mat += object.materials[mat_i - 1].gl_buffer_size;
				
				if (scop->ogl.s_texturing == 2)
				{
					glUniform2f(uniform.tex_size, scop->textures[scop->textures_count - 1].width, scop->textures[scop->textures_count - 1].height);
					glUniform1i(uniform.textured, 2);
					glActiveTexture(GL_TEXTURE0 + 0);
					glBindTexture(GL_TEXTURE_2D, scop->textures[scop->textures_count - 1].gl_texture);
				}
				send_material_data(uniform, scop->textures, material, scop->ogl.s_texturing);
				glDrawArrays(GL_TRIANGLES, (offset_obj + offset_mat) / BUFFER_COMPONENT, (material.gl_buffer_size) / BUFFER_COMPONENT);
			}
		}
		glfwSwapBuffers(scop->window);
		glfwPollEvents();
	}
}

void	start_parser(t_scop *scop, int argc, char *argv[])
{
	struct stat		st;
	int				st_result;
	float			start	= glfwGetTime();
	float			diff 	= start;

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
}

void	auto_positions(t_scop *scop)
{
	t_objects	*obj;
	t_objects	*prev_obj;
	t_vec3f		min;
	t_vec3f		max;
	t_vec3f		center;
	float		max_component;

	max	= (t_vec3f){ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	min	= (t_vec3f){ FLT_MAX, FLT_MAX, FLT_MAX };
	max_component = -FLT_MAX;
	
	for (size_t obji = 0; obji < scop->objects_count; obji++)
	{
		obj = &scop->objects[obji];
		obj->pos = (t_vec3f) {-obj->center.x, -obj->min.y, -obj->center.z};
		obj->rot = (t_vec3f) {0, (rand() % 100) * 0.01, 0};
		if (obji > 0)
		{
			prev_obj = &scop->objects[obji - 1];
			obj->pos.x += prev_obj->pos.x + prev_obj->max.x + obj->max.x + obj->center.x;
		}
		max.x = fmax(max.x, obj->pos.x);
		max.y = fmax(max.y, obj->pos.y);
		max.z = fmax(max.z, obj->pos.z);
		min.x = fmin(min.x, obj->pos.x);
		min.y = fmin(min.y, obj->pos.y);
		min.z = fmin(min.z, obj->pos.z);
		if (scop->objects_count == 1)
		{
			obj->pos = (t_vec3f) {-obj->center.x, -obj->center.y / 2, -obj->center.z};
			max.x = fmax(max.x, obj->max.x);
			max.y = fmax(max.y, obj->max.y);
			max.z = fmax(max.z, obj->max.z);
			min.x = fmin(min.x, obj->min.x);
			min.y = fmin(min.y, obj->min.y);
			min.z = fmin(min.z, obj->min.z);
		}
	}
	
	center = vec_multf(vec_add(min, max), .5);

	max_component = fmax(max_component, max.x);
	max_component = fmax(max_component, max.y);
	max_component = fmax(max_component, max.z);

	if (scop->objects_count > 1)
	{
		scop->cam_pos.x -= center.x;
		scop->cam_pos.y -= center.y;
	}
	scop->cam_pos.z -= max_component ;

	scop->multiplier = max_component * 0.125;
}

int 	main(int argc, char *argv[])
{
	t_scop	*scop;

	scop = calloc(1, sizeof(t_scop));

	if (!scop)
		die ("Could not start Scop");

	srand(time(NULL));

	scop->textures_count 	= 0;
	scop->textures 			= NULL;
	scop->cam_pos			= (t_vec3f) {0, 0, 0};
	scop->cam_rot			= (t_vec3f) {0, 0, 0};
	scop->width	 			= 1920;
	scop->height 			= 1080;
	scop->multiplier		= 1;
	scop->ogl.s_texturing	= 0;
	memset(scop->keys, 0, sizeof(uint32_t) * 348);
		
	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);

	scop->objects_count = 0;
	scop->objects = calloc(argc, sizeof(t_objects));

	scop->ogl.p_render = create_shader_program("Shaders/simple/vertex.glsl", "Shaders/simple/fragment.glsl", NULL);
	// scop->ogl.p_render = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl", "Shaders/geometry_classic.glsl");

	printf("[Scop] Starting parser\n\n");
	start_parser(scop, argc, argv);
	
	printf("[Scop] Starting OpenGL Buffer initialization\n");
	init_opengl_buffer_multi(scop);

	printf("[Scop] Opening Window\n");
	glfwShowWindow(scop->window);

	glUseProgram(scop->ogl.p_render);

	printf("[Scop] Ready\n");

	auto_positions(scop);


	// pas fini ça la berk
    unsigned char   *image;
    int             width;
    int             height;
    int             channels;

 	if (!(image = stbi_load("Resources/large_qpupier.png", &width, &height, &channels, 0)))
    {
        return (1);
    }
	generate_gl_texture(scop, width, height, image, "CHECKER", GL_UNSIGNED_BYTE);
	// 
	
	display_loop(scop);
	clean_exit(scop);
}