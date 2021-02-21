#pragma once

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "DirLight.h"

class Renderer
{
private:

	bool m_isAntiAliasingEnabled = false;

public:

	void Init();
	void Draw(Model* model, Camera* camera, DirLight* light);
	void Shutdown();
	void SetViewport(GLint x, GLint y, GLint width, GLint height);
	void ToggleAntiAliasing(bool status);

	bool IsAntiAliasingEnabled();

};