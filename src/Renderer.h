#pragma once

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "DirLight.h"

class Renderer
{
private:



public:

	void Init();
	void Draw(Model* model, Camera* camera, DirLight* light);
	void Shutdown();

};