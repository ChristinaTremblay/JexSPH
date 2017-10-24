#version 450 core

in vec4 vPosition;

void main() {
	vec4 pos = vPosition;
	if (pos.z > 1.0) pos.z = 0.0;
	gl_Position = pos;
	
}