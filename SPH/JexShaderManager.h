#pragma once

#ifndef JEX_SHADER_COMPILER_H
#define JEX_SHADER_COMPILER_H

#include <vector>
#include <string>
#include <fstream>

#include <OpenGL\glew.h>
#include <OpenGL\freeglut.h>

namespace Jex {

	class JexShaderManager {
	private:
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint shader_program;
	public:
		JexShaderManager();
		~JexShaderManager();

		std::string ReadShaderFromFile(const char * file_path);

		GLint CompileVertexShader(const char * file_path);
		GLint CompileFragmentShader(const char * file_path);
		GLint LinkShaderProgram();
		void UseShaderProgram() const;

		GLuint GetVertexShader() const { return vertex_shader; }
		GLuint GetFragmentShader() const { return fragment_shader; }
		GLuint GetShaderProgram() const { return shader_program; }
	};


}

#endif