#version 450 core

layout(location = 1) in vec4 tri_pos1;
layout(location = 2) in vec4 tri_color1;
out vec4 vs_out_color;
uniform vec3 Jexangles;
uniform mat4 view_mtrx;
uniform mat4 proj_mtrx;

void main() {

	vec3 r = radians(Jexangles);
	vec3 c = cos(r);
	vec3 s = sin(r);

	mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
		0.0, c.x, -s.x, 0.0,
		0.0, s.x, c.x, 0.0,
		0.0, 0.0, 0.0, 1.0);

	mat4 ry = mat4(c.y, 0.0, s.y, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-s.y, 0.0, c.y, 0.0,
		0.0, 0.0, 0.0, 1.0);

	mat4 rz = mat4(c.z, -s.z, 0.0, 0.0,
		s.z, c.z, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	//gl_Position = rx * ry * rz * tri_pos1;
	gl_Position =  proj_mtrx * view_mtrx * tri_pos1;
	vs_out_color = tri_color1;

}