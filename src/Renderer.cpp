#include "Renderer.h"

void Renderer::Init()
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.f);
	glEnable(GL_DEPTH_TEST);

	ToggleAntiAliasing(true);
	ToggleFaceCulling(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

void Renderer::Draw(Model* model, Camera* camera, DirLight* light, Skybox* skybox)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto viewMatrix = camera->GetViewMatrix();
	auto projMatrix = camera->GetProjectionMatrix();

	if(m_useSkybox && skybox)
	{
		auto* shader = skybox->m_shader;

		shader->UseProgram();
		shader->SetIntUniform("skyboxTexture", 0);
		shader->SetMat4Uniform("projection", projMatrix);
		shader->SetMat4Uniform("view", viewMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->m_cubemap.GetTextureId());

		glDrawArrays(GL_TRIANGLES, 0, sizeof(skybox->m_vertices) / sizeof(skybox->m_vertices[0]));

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	if(!model)
	{
		return;
	}

	auto* shader = model->m_shader;

	shader->UseProgram();
	
	shader->SetMat4Uniform("ModelMatrix", model->GetModelMatrix());
	shader->SetMat4Uniform("ViewMatrix", viewMatrix);
	shader->SetMat4Uniform("ProjectionMatrix", projMatrix);
	shader->SetVec3Uniform("CameraPos", camera->GetPosition());
	shader->SetVec3Uniform("LightDir", light->GetForwardVector());
	shader->SetVec3Uniform("LightColor", light->color);
	shader->SetFloatUniform("LightShiness", light->shiness);
	shader->SetBoolUniform("UseOpacity", model->m_useOpacity);
	shader->SetBoolUniform("UseNormal", model->m_useNormal);
	
	shader->SetIntUniform("diffuseTexture", 0);
	shader->SetIntUniform("specularTexture", 1);
	shader->SetIntUniform("opacityTexture", 2);
	shader->SetIntUniform("normalTexture", 3);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->m_material->diffuse.id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, model->m_material->specular.id);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, model->m_material->opacity.id);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, model->m_material->normal.id);

	for (Mesh mesh : model->m_meshes[0])
	{
		glBindVertexArray(mesh.m_meshData.vao);
		glDrawElements(GL_TRIANGLES, mesh.m_meshData.dataLength, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::Shutdown()
{
}

void Renderer::SetViewport(GLint x, GLint y, GLint width, GLint height)
{
	glViewport(x, y, width, height);
}

void Renderer::ToggleAntiAliasing(bool status)
{
	if(status)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else
	{
		glDisable(GL_MULTISAMPLE);
	}

	m_isAntiAliasingEnabled = status;
}

void Renderer::ToggleFaceCulling(bool status) 
{
	if(status)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	m_isFaceCullingEnabled = status;
}

void Renderer::SetFaceCullingMode(CullingMode mode) 
{
	glCullFace(GetGlCullingMode(mode));
	m_faceCullingMode = mode;
}

bool Renderer::IsAntiAliasingEnabled()
{
	return m_isAntiAliasingEnabled;
}

bool Renderer::IsFaceCullingEnabled() 
{
	return m_isFaceCullingEnabled;
}

CullingMode Renderer::GetFaceCullingMode() 
{
	return m_faceCullingMode;
}

glm::vec3 Renderer::GetClearColor() 
{
	return m_clearColor;
}

void Renderer::SetClearColor(glm::vec3 newClearColor) 
{
	m_clearColor = newClearColor;
}

bool Renderer::GetUseSkybox() 
{
	return m_useSkybox;
}

void Renderer::SetUseSkybox(bool status) 
{
	m_useSkybox = status;
}

GLenum Renderer::GetGlCullingMode(CullingMode mode) 
{
	switch (mode)
	{
	case CullingMode::FRONT:
		return GL_FRONT;
	case CullingMode::BACK:
		return GL_BACK;
	case CullingMode::FRONT_AND_BACK:
		return GL_FRONT_AND_BACK;
	}

	throw new CannotConvertException();
}
