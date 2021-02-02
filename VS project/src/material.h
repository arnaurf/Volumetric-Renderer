#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "extra/hdre.h"

class Material {
public:

	Shader* shader = NULL;
	Texture* texture = NULL;
	vec4 color;

	virtual void setUniforms(Camera* camera, Matrix44 model) = 0;
	virtual void render(Mesh* mesh, Matrix44 model, Camera * camera) = 0;
	virtual void renderInMenu() = 0;
};

class StandardMaterial : public Material {
public:

	StandardMaterial();
	~StandardMaterial();

	//------------ Parameters
	float text_num = 0;
	float configuration = 0;
	float prueba = 0.6;
	float brillo = 22;
	float value3 = 1;
	float stepsize = 0.01;
	float threshold = 0.32;
	float max_steps = 300;
	Vector3 max_thr = Vector3(1, 1, 1);
	Vector3 min_thr = Vector3(-1, -1, -1);
	Vector3 volume_dim = Vector3(1, 1, 1);
	//---------------------


	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera * camera);
	void renderInMenu();
};

class WireframeMaterial : public StandardMaterial {
public:

	WireframeMaterial();
	~WireframeMaterial();

	void render(Mesh* mesh, Matrix44 model, Camera * camera);
};

#endif