/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:47:33 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 18:53:20 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

static void	get_uniforms_location(t_uniforms *uniform, uint32_t program)
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

static void	send_default_uniforms(t_uniforms uniform, float glfw_time, t_scop *scop)
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

static void	send_model_data(t_uniforms uniform, t_objects object)
{
	const t_mat4	mat_scale 		= m4_scale(1, 1, 1);
	const t_mat4	mat_translate 	= m4_translate(object.pos.x, object.pos.y, object.pos.z);
	const t_mat4	mat_rotate		= m4_rotation_around_center(object.center, object.rot.x, object.rot.y, object.rot.z);
	const t_mat4	mat_model 		= m4_mult(m4_mult(mat_scale, mat_rotate), mat_translate);

	glUniformMatrix4fv(uniform.m4_model, 1, GL_FALSE, mat_model.value[0]);
	glUniform1f(uniform.obj_max_y, object.max.y);
	glUniform3f(uniform.obj_center, object.center.x, object.center.y, object.center.z);
}

static void	send_material_data(t_uniforms uniform, t_textures *textures, t_mat material, int s_texturing)
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



#include <stdio.h>

void		check_mouse_object(t_scop *scop)
{
	double pos_x;
	double pos_y;
	glfwGetCursorPos(scop->window, &pos_x, &pos_y);

	if (pos_x < 0 || pos_x >= scop->width || pos_y < 0 || pos_y > scop->height)
		return ;

	
	t_mat4	mat_model;
	t_mat4	mat_camera;
	t_mat4	mat_final;
	t_objects	object;
	t_vec4f		min;
	t_vec4f		max;
	t_vec3f		msp;
	t_mat4 mat_perspective = m4_perspective(1.0472, (float) scop->width / (float) scop->height, 0.00001f, 1000.0f);


	// mat_camera = m4_viewmat(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z, m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));
	mat_camera = m4_mult(m4_mult(m4_scale(1, 1, 1), m4_rotation(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z)), m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));
	msp.x = (pos_x / (scop->width * .5)) - 1;
	msp.y = (pos_y / (scop->height * .5)) - 1;
	msp.z = 1;

	for (size_t obji = 0; obji < scop->objects_count; obji++)
	{
		object = scop->objects[obji];
		//mat_model = m4_mult(m4_mult(m4_scale(1, 1, 1), m4_rotation_around_center(object.center, object.rot.x, object.rot.y, object.rot.z)), m4_translate(object.pos.x, object.pos.y, object.pos.z));
		
		mat_model = m4_init();

		mat_final = m4_mult(m4_mult(mat_perspective, mat_camera), mat_model);
		
		m4_print(mat_final);
		
		min = m4_mult_vec4f(mat_final, (t_vec4f){object.min.x, object.min.y, object.min.z, 1});
		max = m4_mult_vec4f(mat_final, (t_vec4f){object.max.x, object.max.y, object.max.z, 1});


		min.x /= min.w;
		max.x /= max.w;
		printf("min %f %f %f %f\n", min.x, min.y, min.z, min.w);
		printf("max %f %f %f %f\n", max.x, max.y, max.z, max.w);
		printf("msp %f %f %f\n", msp.x, msp.y, msp.z);

		if (msp.x >= max.x && msp.x <= min.x )
		{
			printf("Object found : %zu\n", obji);
		}
	}
	printf("\n");
	
}

void	    display_loop(t_scop *scop)
{
	t_uniforms 		uniform;
	float		    glfw_time;
	float		    base_time;
	size_t		    offset_mat;
	size_t		    offset_obj;
	t_objects	    object;
	t_mat		    material;
	
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
		
		glfw_time = glfwGetTime() - base_time;
		
		handle_keyboard(scop->window, scop->keys, &scop->ogl.s_texturing);
		// handle_mouse(scop->window, &scop->cam_rot);
		handle_transformations(scop);
    	
		check_mouse_object(scop);
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
				
				if (scop->ogl.s_texturing == 2 && scop->textures_count)
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
