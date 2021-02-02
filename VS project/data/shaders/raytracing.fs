uniform vec3 u_camera_pos;
uniform mat4 u_model;
uniform vec3 local_camera;
uniform mat4 u_viewprojection;
uniform vec4 u_color;
uniform sampler3D u_texture;
uniform sampler2D transfer;

//PARAMETERS
uniform float y_transfer_text;
uniform float brillo;
uniform float value3;
uniform float max_steps;
uniform float threshold;
uniform vec3 min_thr;
uniform vec3 max_thr;
uniform vec3 volume_dim;
uniform float stepsize;
//-----------
uniform vec3 light;			//light colour
uniform vec4 material; 		//specular, diffuse and ambient parameters
uniform vec3 light_pos; 	//position of light
//------------

varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec3 v_uv;
varying vec4 v_color;


float random(vec2 st){
	return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

void main()
{
	vec4 camera =  vec4(u_camera_pos, 1.0);
	
	vec2 st = gl_FragCoord.xy/vec2(700,500);
	float rnd = random (st);

	vec3 ray_vec = v_position - local_camera;
	vec3 step = stepsize * normalize(ray_vec); 
	vec4 color_acc;
	vec4 color_i;
	vec3 current_sample = v_position +  rnd*step; 
	
	

    

	//vec3 current_sample = (dx,dy,dz);

	for( int i=0; i<max_steps; i+=1){

		//Get the 3d texture (voxel) value taking in account the ratio parameter
		color_i = texture3D( u_texture, ( ((current_sample + 1)/2)*volume_dim ) + (1-volume_dim)/2);
		
		
		//partial derivatives (Gradient)
		float dx = ((current_sample.x + step.x) - (current_sample.x))/2*step.x;
		float dy = ((current_sample.y + step.y) - (current_sample.y))/2*step.y;
		float dz = ((current_sample.z + step.z) - (current_sample.z))/2*step.z;


		//Dont print any pixel out of this range
		if(color_i.r > threshold && current_sample.x < max_thr.x && current_sample.x > min_thr.x &&
									current_sample.y < max_thr.y && current_sample.y > min_thr.y &&
									current_sample.z < max_thr.z && current_sample.z > min_thr.z){
									
			
			vec3 norm_vec3 = normalize(vec3(dx, dy, dz));
			vec3 L = normalize(light_pos - current_sample);
			vec3 V = normalize(local_camera - current_sample);
			vec3 R = -reflect(L, norm_vec3);
			float LdotN = dot(L, norm_vec3);
			float RdotV = dot(R, V);
			
			
			//transfer function as a function
			vec4 val_color;
			if(color_i.r < 0.25){ 		val_color = vec4(1,0,0,0.2)*color_i.r;}
			else if(color_i.r < 0.31){ 	val_color = vec4(0,1,0,0.3)*color_i.r;}
			else {					 	val_color = vec4(1,1,1,1)*color_i.r;}
			
			
			//transfer function as a LUT
			//val_color = vec4( texture2D(transfer, vec2(min(1.0,color_i.r), 1)).rgb, texture2D(transfer, vec2(color_i.r, 1)).a*color_i.r);
			
			//Linear transfer function
			//vec4 val_color = vec4(color_i.r, color_i.r, color_i.r, color_i.r);

			//Update the accumulated value
			val_color.rgb = val_color.rgb * val_color.a;
			vec4 aux = length(step) * val_color * (1 - color_acc.a);
			
			vec3 ambient = aux.xyz * material.x * light;
			vec3 diffuse = aux.xyz * vec3(max(0.0f, LdotN)) * light * material.y;
			vec3 specular = aux.xyz * light * vec3(pow(max(0.0f, RdotV), material.a*100)) * material.z*7;
						
			color_acc =  vec4( ambient + diffuse + specular, aux.a) + color_acc;
		}

		//Make a step on in the ray direction.
		current_sample = current_sample + step;

		//Exit conditions
		if( 	current_sample.x < -1 || current_sample.x > 1 ||
				current_sample.y < -1 || current_sample.y > 1 ||
				current_sample.z < -1 || current_sample.z> 1 || color_acc.a > 0.95){
			break;
		}
	}
	
	
	gl_FragColor = pow(color_acc, vec4(1/value3, 1/value3, 1/value3, 1.0)) * brillo;	
}
