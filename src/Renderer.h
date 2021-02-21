#pragma once

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "DirLight.h"
#include "Exceptions.h"

enum class CullingMode : int {
	FRONT = 0,
	BACK = 1,
	FRONT_AND_BACK = 2
};

class Renderer
{
private:

	bool m_isAntiAliasingEnabled = false;

	bool m_isFaceCullingEnabled = false;
	CullingMode m_faceCullingMode = CullingMode::BACK;

	GLenum GetGlCullingMode(CullingMode mode);

public:

	void Init();
	void Draw(Model* model, Camera* camera, DirLight* light);
	void Shutdown();
	void SetViewport(GLint x, GLint y, GLint width, GLint height);
	void ToggleAntiAliasing(bool status);

	void ToggleFaceCulling(bool status);
	void SetFaceCullingMode(CullingMode mode);

	bool IsAntiAliasingEnabled();

	bool IsFaceCullingEnabled();
	CullingMode GetFaceCullingMode();

};