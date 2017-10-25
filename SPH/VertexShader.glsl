#version 450 core

in vec4 vPosition;
uniform mat4 view_mtrx;
uniform mat4 proj_mtrx;

void main() {
	
	gl_Position = proj_mtrx * view_mtrx * vPosition;

}