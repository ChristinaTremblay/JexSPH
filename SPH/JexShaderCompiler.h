#pragma once

#ifndef JEX_SHADER_COMPILER_H
#define JEX_SHADER_COMPILER_H

#include <vector>
#include <string>
#include <fstream>

#include <OpenGL\glew.h>
#include <OpenGL\freeglut.h>

namespace Jex {

	class ShaderCompiler {
	public:
		ShaderCompiler();
		~ShaderCompiler();

		std::string ReadShaderFromFile(const char * file_path);
		GLint Compile();

	};

}

#endif