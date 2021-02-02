#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

float prueba = 0.6;
float brillo = 22;
float value3 = 1;
float stepsize = 0.01;
float threshold = 0.32;
float max_steps = 300;
Vector3 max_thr = Vector3(1, 1, 1);
Vector3 min_thr = Vector3(-1, -1, -1);
Vector3 light_pos = Vector3(1, 1.8, 2.2);
Vector4 material_spec = Vector4(0.16, 0.62, 0.06, 1);
Vector4 light = Vector4(1, 1, 1, 1);
StandardMaterial::StandardMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	this->text_num = 0;
	this->configuration = 0;
}

StandardMaterial::~StandardMaterial()
{

}

void StandardMaterial::setUniforms(Camera* camera, Matrix44 model)
{

	//----new
	Matrix44 aux = model; aux.inverse();
	Texture* transfer = Texture::Get("data/transfer_function.png");
	shader->setUniform("transfer", transfer);
	//----

	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	Vector3 l = aux * camera->eye;
	shader->setUniform("local_camera", l);
	shader->setUniform("u_model", model);

	//---------------------------------------
	shader->setUniform("y_transfer_text", prueba);
	shader->setUniform("brillo", brillo);
	shader->setUniform("value3", value3);
	shader->setUniform("threshold", threshold);
	shader->setUniform("max_steps", max_steps);
	shader->setUniform("min_thr", min_thr);
	shader->setUniform("max_thr", max_thr);
	shader->setUniform("volume_dim", volume_dim);
	//------------------
	shader->setUniform("light", Vector3(light.x, light.y, light.z));
	shader->setUniform("material", material_spec);
	shader->setUniform("light_pos", light_pos);


	//Texture* noise = new Texture();
	Texture* noise = Texture::Get("data/h.png");
	/*for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			float col = rand();
			noise->setPixel(i, j, Color(col, col, col) );
		}
	}*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	shader->setUniform("noise", noise);
	shader->setUniform("u_color", color);

	shader->setUniform("stepsize", stepsize);

	if (texture)
		shader->setUniform("u_texture", texture);

}

void StandardMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);
		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}
float aa;
void StandardMaterial::renderInMenu()
{
	ImGui::DragFloat3("Max coordinate", (float*)&max_thr, 0.02, -1, 1);
	ImGui::DragFloat3("Min coordinate", (float*)&min_thr, 0.02, -1, 1); 
	ImGui::DragFloat3("Volume dimentions", (float*)&this->volume_dim, 0.02, 0.1, 10); 
	ImGui::DragFloat3("Light position", (float*)&light_pos, 0.02);
	ImGui::DragFloat4("Material parameters", (float*)&material_spec, 0.02, 0, 1000);
	ImGui::ColorEdit3("light color", (float*)&light);

	ImGui::DragFloat("Texture range", (float*)&prueba, 0.05, 0, 1);
	ImGui::DragFloat("Brightness", (float*)&brillo, 0.1, 0, 250);
	ImGui::DragFloat("Value \"contrast\"", (float*)&value3,0.05,0.001,10);
	ImGui::DragFloat("Brightness threshold", (float*)&threshold, 0.02, 0.05, 1);
	ImGui::DragFloat("Max steps", (float*)&max_steps, 1, 10, 1000);
	ImGui::DragFloat("Step size", (float*)&stepsize, 0.001, 0.001, 0.5);
	if (ImGui::Button("Press to change the volume")) {
		this->text_num = (int)(this->text_num + 1.0) % 3;
	}

	if (ImGui::Button("Press to change switch preset")) {
		this->configuration = (int)(this->configuration + 1.0) % 3;
		switch ((int)this->configuration) {
		default:
		case 0:
			prueba = 0.3;
			brillo = 12;
			value3 = 1;
			stepsize = 0.01;
			threshold = 0.32;
			max_steps = 300;
			max_thr = Vector3(1, 0.53, 1);
			min_thr = Vector3(-1, -1, -1);
			break;
		case 1:
			prueba = 0.3;
			brillo = 12.7;
			value3 = 1.2;
			stepsize = 0.01;
			threshold = 0.14;
			max_steps = 300;
			max_thr = Vector3(1, 0.53, 1);
			min_thr = Vector3(-1, -1, -1);
			break;
		case 2:
			prueba = 0;
			brillo = 20;
			value3 = 1.15;
			stepsize = 0.01;
			threshold = 0.14;
			max_steps = 300;
			max_thr = Vector3(1, 0.53, 0.78);
			min_thr = Vector3(-1, -1, 0.26);
			break;
		}
	}
}

WireframeMaterial::WireframeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

WireframeMaterial::~WireframeMaterial()
{

}

void WireframeMaterial::render(Mesh* mesh, Matrix44 model, Camera * camera)
{
	if (shader && mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}