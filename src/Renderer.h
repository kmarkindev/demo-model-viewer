#pragma once

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "DirLight.h"
#include "Exceptions.h"
#include "Skybox.h"

enum class CullingMode : int {
	FRONT = 0,
	BACK = 1,
	FRONT_AND_BACK = 2
};

class Renderer
{
private:

	bool m_useSkybox = false;

	bool m_isAntiAliasingEnabled = false;

	bool m_isFaceCullingEnabled = false;
	CullingMode m_faceCullingMode = CullingMode::BACK;

	GLenum GetGlCullingMode(CullingMode mode);

	glm::vec3 m_clearColor = {0.3f, 0.3f, 0.3f};

public:

	void Init();
	void Draw(Model* model, Camera* camera, DirLight* light, Skybox* skybox);
	void Shutdown();
	void SetViewport(GLint x, GLint y, GLint width, GLint height);
	void ToggleAntiAliasing(bool status);

	void ToggleFaceCulling(bool status);
	void SetFaceCullingMode(CullingMode mode);

	bool IsAntiAliasingEnabled();

	bool IsFaceCullingEnabled();
	CullingMode GetFaceCullingMode();

	glm::vec3 GetClearColor();
	void SetClearColor(glm::vec3 newClearColor);

	bool GetUseSkybox();
	void SetUseSkybox(bool status);

};