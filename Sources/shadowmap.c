#include "Scop.h"

t_mat4 		get_light_mat(void)
{
	t_mat4 		mat_ortho 	= m4_orthogonal(-100, 200, -10, 10, -10, 10);
	t_mat4 		mat_light 	= m4_look_at((t_vec3f) {1, 1.f, 2.0f}, (t_vec3f) {0, 0, 0}, (t_vec3f){0, 1, 0});
	t_mat4 		mat_final 	= m4_mult(mat_light, mat_ortho);

	return (mat_final);
}

void 		get_uniforms_location_depth(t_uniforms *uniform, uint32_t program)
{
	uniform->m4_view 		= glGetUniformLocation(program, "View");
	uniform->m4_model 		= glGetUniformLocation(program, "Model");
	uniform->m4_light 		= glGetUniformLocation(program, "Light");
}

void 		init_depthmap(t_scop *scop)
{
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glGenFramebuffers(1, &scop->ogl.fbo_depth);  
	glBindFramebuffer(GL_FRAMEBUFFER, scop->ogl.fbo_depth);
	
	glGenTextures(1, &scop->ogl.tex_depth);
	glBindTexture(GL_TEXTURE_2D, scop->ogl.tex_depth);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, scop->ogl.tex_depth, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  

}

void 		render_depthmap(t_scop *scop, t_uniforms uniform)
{
	t_objects 	object;
	t_mat 		material;
	size_t 		offset_obj 	= 0;
	size_t 		offset_mat 	= 0;

    glUseProgram(scop->ogl.p_depth);
	glBindFramebuffer(GL_FRAMEBUFFER, scop->ogl.fbo_depth);
	glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uniform.m4_light, 1, GL_FALSE, get_light_mat().value[0]);
	
	glBindVertexArray(scop->vao);

	for (size_t obj_i = 0; obj_i < scop->objects_count; obj_i++)
	{
		offset_mat 	= 0;
		object 		= scop->objects[obj_i];
		send_model_data(uniform, object);
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, scop->width * 2, scop->height * 2);
}
