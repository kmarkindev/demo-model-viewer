#include "Renderer.h"

void Renderer::Init()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_DEPTH_TEST);

	ToggleAntiAliasing(true);
	ToggleFaceCulling(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

void Renderer::Draw(Model* model, Camera* camera, DirLight* light)
{
	auto* shader = model->m_shader;

	shader->UseProgram();
	
	shader->SetMat4Uniform("ModelMatrix", model->GetModelMatrix());
	shader->SetMat4Uniform("ViewMatrix", camera->GetViewMatrix());
	shader->SetMat4Uniform("ProjectionMatrix", camera->GetProjectionMatrix());
	shader->SetVec3Uniform("LightDir", light->GetForwardVector());
	shader->SetVec3Uniform("CameraDir", camera->GetForwardVector());
	shader->SetVec3Uniform("LightColor", light->color);
	shader->SetIntUniform("diffuseTexture", 0);
	shader->SetIntUniform("specularTexture", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->m_material->diffuse.id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, model->m_material->specular.id);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
