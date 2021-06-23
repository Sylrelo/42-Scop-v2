/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:47:33 by slopez            #+#    #+#             */
/*   Updated: 2021/06/21 19:27:38 by slopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"

#include <stdio.h>

static void get_uniforms_location_depth(t_uniforms *uniform, uint32_t program)
{
	uniform->m4_view 		= glGetUniformLocation(program, "View");
	uniform->m4_model 		= glGetUniformLocation(program, "Model");
	uniform->m4_light 		= glGetUniformLocation(program, "Light");
}


 #include <cglm/cglm.h>

static void render_depthmap(t_scop *scop, t_uniforms uniform)
{
	t_mat4 	mat_final;


	t_mat4 mat_ortho 	= m4_orthogonal(1, 7.5, -10, 10, -10, 10);
	t_mat4 mat_light 	= m4_look_at((t_vec3f) {1, 1.f, 2.0f}, (t_vec3f) {0, 0, 0}, (t_vec3f){0, 1, 0});
	mat_final 	= m4_mult(mat_ortho, mat_light);

	mat4 ortho, light, final;

	vec3 lpo, zero, up;

	lpo[0] = 1;
	lpo[1] = 1;
	lpo[2] = 2;

	zero[0] = 0;
	zero[1] = 0;
	zero[2] = 0;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	glm_ortho(-10, 10, -10, 10, -100, 200, ortho);
	glm_lookat(lpo, zero, up, light);
	glm_mat4_mul(ortho, light, final);


    glUseProgram(scop->ogl.p_depth);

    // glUniformMatrix4fv(uniform.m4_light, 1, GL_FALSE, mat_final.value[0]);


	glBindFramebuffer(GL_FRAMEBUFFER, scop->ogl.fbo_depth);
	glViewport(0, 0, 1024, 1024);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(uniform.m4_light, 1, GL_FALSE, final[0]);

	glBindVertexArray(scop->vao);
	// moche

	t_objects object;
	t_mat material;
	size_t offset_obj = 0;
	size_t offset_mat = 0;

	for (size_t obj_i = 0; obj_i < scop->objects_count; obj_i++)
	{
		offset_mat 	= 0;
		object 		= scop->objects[obj_i];


		const t_mat4	mat_scale 		= m4_scale(object.scale, object.scale, object.scale);
		const t_mat4	mat_translate 	= m4_translate(object.pos.x, object.pos.y, object.pos.z);
		const t_mat4	mat_rotate		= m4_rotation_around_center(object.center, object.rot.x, object.rot.y, object.rot.z);
		const t_mat4	mat_model 		= m4_mult(m4_mult(mat_rotate, mat_scale), mat_translate);

		glUniformMatrix4fv(uniform.m4_model, 1, GL_FALSE, mat_model.value[0]);


		if (obj_i > 0)
			offset_obj = scop->objects[obj_i - 1].offset;
		for (size_t mat_i = 0; mat_i < object.nb_mats; mat_i++)
		{
			material = object.materials[mat_i];
			if (mat_i > 0)
				offset_mat += object.materials[mat_i - 1].gl_buffer_size;
			glDrawArrays(GL_TRIANGLES, (offset_obj + offset_mat) / BUFFER_COMPONENT, (material.gl_buffer_size) / BUFFER_COMPONENT);
		}
	}
	// fin moche


	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, scop->width * 2, scop->height * 2);

}

static void	get_uniforms_location(t_uniforms *uniform, uint32_t program)
{
	uniform->m4_projection 	= glGetUniformLocation(program, "Persp");
	uniform->m4_view 		= glGetUniformLocation(program, "View");
	uniform->m4_model 		= glGetUniformLocation(program, "Model");
	uniform->m4_light 		= glGetUniformLocation(program, "Light");
	uniform->kd 			= glGetUniformLocation(program, "kd");
	uniform->ka 			= glGetUniformLocation(program, "ka");
	uniform->ks 			= glGetUniformLocation(program, "ks");
	uniform->ns 			= glGetUniformLocation(program, "ns");
	uniform->textured 		= glGetUniformLocation(program, "textured");
	uniform->mapping 		= glGetUniformLocation(program, "mapping");
	uniform->glfw_time 		= glGetUniformLocation(program, "glfw_time");
	uniform->glfw_options 	= glGetUniformLocation(program, "glfw_options");
	uniform->tex_size 		= glGetUniformLocation(program, "tex_size");
	uniform->obj_max_y 		= glGetUniformLocation(program, "obj_max_y");
	uniform->obj_center 	= glGetUniformLocation(program, "obj_center");
	uniform->tex_basic 		= glGetUniformLocation(program, "basic_texture");
	uniform->tex_object 	= glGetUniformLocation(program, "ourTexture");
	uniform->tex_shadowmap 	= glGetUniformLocation(program, "shadow_map");
	uniform->object_selected= glGetUniformLocation(program, "object_selected");
	uniform->view_pos 		= glGetUniformLocation(program, "view_pos");

}

static void	send_default_uniforms(t_uniforms uniform, float glfw_time, t_scop *scop)
{
	t_mat4	mat_view 		= m4_init();
	t_mat4	mat_perspective;


	glUniform1f(uniform.glfw_time, scop->fade_start_time > 0 ? glfw_time - scop->fade_start_time : glfw_time);

	glUniform1i(uniform.mapping, scop->ogl.s_mapping);

	mat_view = m4_viewmat(scop->cam_rot.x, scop->cam_rot.y, scop->cam_rot.z, m4_translate(scop->cam_pos.x, scop->cam_pos.y, scop->cam_pos.z));
	mat_perspective = m4_perspective(1.0472, (float) scop->width / (float) scop->height, 0.00001f, 1000.0f);
	// mat_perspective = m4_orthogonal(1, 7.5, -10, 10, -10, 10);


	vec3 lpo, zero, up;
	mat4 ortho, light, final;

	lpo[0] = 1;
	lpo[1] = 1;
	lpo[2] = 2;

	zero[0] = 0;
	zero[1] = 0;
	zero[2] = 0;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	glm_ortho(-10, 10, -10, 10, -100, 200, ortho);
	glm_lookat(lpo, zero, up, light);
	glm_mat4_mul(ortho, light, final);

	glUniformMatrix4fv(uniform.m4_light, 1, GL_FALSE, final[0]);

	glUniform3f(uniform.view_pos, scop->cam_pos.x, -scop->cam_pos.y, scop->cam_pos.z);
	glUniformMatrix4fv(uniform.m4_view, 1, GL_FALSE, mat_view.value[0]);
	glUniformMatrix4fv(uniform.m4_projection, 1, GL_FALSE, mat_perspective.value[0]);
	glUniform1i(uniform.tex_basic, 0);
	glUniform1i(uniform.tex_object, 1);
	glUniform1i(uniform.tex_shadowmap, 2);
}

static void	send_model_data(t_uniforms uniform, t_objects object)
{
	const t_mat4	mat_scale 		= m4_scale(object.scale, object.scale, object.scale);
	const t_mat4	mat_translate 	= m4_translate(object.pos.x, object.pos.y, object.pos.z);
	const t_mat4	mat_rotate		= m4_rotation_around_center(object.center, object.rot.x, object.rot.y, object.rot.z);
	const t_mat4	mat_model 		= m4_mult(m4_mult(mat_rotate, mat_scale), mat_translate);

	glUniformMatrix4fv(uniform.m4_model, 1, GL_FALSE, mat_model.value[0]);
	glUniform1f(uniform.obj_max_y, object.max.y);
	glUniform3f(uniform.obj_center, object.center.x, object.center.y, object.center.z);
}

static void	send_material_data(t_uniforms uniform, t_textures *textures, t_mat material, int s_texturing)
{
	t_textures	tex;

	glUniform3f(uniform.kd, material.kd.x, material.kd.y, material.kd.z);
	glUniform3f(uniform.ka, material.ka.x, material.ka.y, material.ka.z);
	glUniform3f(uniform.ks, material.ks.x, material.ks.y, material.ks.z);
	glUniform1f(uniform.ns, material.ns);

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

void	    display_loop(t_scop *scop)
{
	t_uniforms 		uniform;
	float		    glfw_time;
	float		    base_time;
	size_t		    offset_mat;
	size_t		    offset_obj;
	t_objects	    object;
	t_mat		    material;
	int 			old_s_texturing;


	t_uniforms 		uniform_depth;

	get_uniforms_location(&scop->ogl.u_render, scop->ogl.p_render);

	get_uniforms_location_depth(&uniform_depth, scop->ogl.p_depth);

	uniform = scop->ogl.u_render;

	glfw_time = glfwGetTime();
	base_time = glfw_time;
	glfw_time -= base_time;
	old_s_texturing = 0;
	

	while (!glfwWindowShouldClose(scop->window))
	{
		offset_mat 		= 0;
		offset_obj 		= 0;
		
		render_depthmap(scop, uniform_depth);

    	glUseProgram(scop->ogl.p_render);

		glfw_time = glfwGetTime() - base_time;
		
		if (scop->ogl.s_texturing != old_s_texturing && scop->fade_start_time == 0.f)
		{
			scop->fade_start_time = glfw_time;
			glUniform1i(uniform.glfw_options, 1);
			old_s_texturing = scop->ogl.s_texturing;
		}
		if (scop->fade_start_time > 0.0f && glfw_time - scop->fade_start_time >= 1.0f)
		{
			glUniform1i(uniform.glfw_options, 0);
			scop->fade_start_time = 0.f;
		}

		handle_keyboard(scop->window, scop->keys, &scop->ogl.s_texturing, &scop->ogl.s_mapping, &scop->selected_object, scop->objects_count);
		handle_mouse(scop->window, &scop->cam_rot);
		handle_transformations(scop);
    	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		send_default_uniforms(uniform, glfw_time, scop);

		for (size_t obj_i = 0; obj_i < scop->objects_count; obj_i++)
		{
			offset_mat 	= 0;
			object 		= scop->objects[obj_i];
			if ((short) obj_i == scop->selected_object)
				glUniform1i(uniform.object_selected, 1);
			else if (scop->selected_object != -1)
				glUniform1i(uniform.object_selected, 0);
			else
				glUniform1i(uniform.object_selected, -1);

			if (obj_i > 0)
				offset_obj = scop->objects[obj_i - 1].offset;
			for (size_t mat_i = 0; mat_i < object.nb_mats; mat_i++)
			{
				send_model_data(uniform, object);
				material = object.materials[mat_i];
				if (mat_i > 0)
					offset_mat += object.materials[mat_i - 1].gl_buffer_size;
				
				if (scop->ogl.s_texturing == 2 && scop->basic_texture_id != -1)
				{
					glUniform2f(uniform.tex_size, scop->textures[scop->basic_texture_id].width, scop->textures[scop->basic_texture_id].height);
					glUniform1i(uniform.textured, 2);
					glActiveTexture(GL_TEXTURE0 + 0);
					glBindTexture(GL_TEXTURE_2D, scop->textures[scop->basic_texture_id].gl_texture);
				}
				send_material_data(uniform, scop->textures, material, scop->ogl.s_texturing);
				glActiveTexture(GL_TEXTURE0 + 2);
				glBindTexture(GL_TEXTURE_2D, scop->ogl.tex_depth);
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, scop->ogl.tex_depth);
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, scop->ogl.tex_depth);

				glDrawArrays(GL_TRIANGLES, (offset_obj + offset_mat) / BUFFER_COMPONENT, (material.gl_buffer_size) / BUFFER_COMPONENT);
			}
		}
		glfwSwapBuffers(scop->window);
		glfwPollEvents();
	}
}
