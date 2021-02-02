attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec3 a_uv;
attribute vec4 a_color;

uniform vec3 u_camera_pos;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec3 v_uv;
varying vec4 v_color;
varying vec3 transformed_eye;
varying vec3 vray_dir;
void main()
{	
	vec3 volume_scale = vec3(1.0);
	vec3 volume_translation = vec3(0.5) - volume_scale * 0.5;
	gl_Position = u_viewprojection * vec4(a_vertex * volume_scale + volume_translation, 1);
	v_uv = a_uv;
	// Compute eye position and ray directions in the unit cube space
	transformed_eye = (u_camera_pos - volume_translation) / volume_scale;
	vray_dir = a_vertex - transformed_eye;
}