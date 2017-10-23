
#include "JexShaderManager.h"

namespace Jex {

	JexShaderManager::JexShaderManager() {
		vertex_shader = 0;
		fragment_shader = 0;
		shader_program = 0;
	}

	JexShaderManager::~JexShaderManager() {

	}

	GLint JexShaderManager::CompileVertexShader(const char * file_path) {

		std::string vertex_str = ReadShaderFromFile(file_path);

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar * vertex_src = (const GLchar *)vertex_str.c_str();
		glShaderSource(vertex_shader, 1, &vertex_src, 0);

		glCompileShader(vertex_shader);

		GLint is_compiled = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE) {
			
			GLint max_length = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);
			
			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

			glDeleteShader(vertex_shader);

			return -1;
		}

		return vertex_shader;
	}

	GLint JexShaderManager::CompileFragmentShader(const char * file_path) {

		std::string fragment_str = ReadShaderFromFile(file_path);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar * fragment_src = (const GLchar *)fragment_str.c_str();
		glShaderSource(vertex_shader, 1, &fragment_src, 0);

		glCompileShader(fragment_shader);

		GLint is_compiled = 0;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE) {

			GLint max_length = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

			glDeleteShader(fragment_shader);

			return -1;
		}

		return fragment_shader;
	}

	std::string JexShaderManager::ReadShaderFromFile(const char * file_path) {

		std::ifstream shader_file = std::ifstream(file_path);
		std::string shader_str = "";

		if (shader_file.is_open()) {
			char c;
			while (shader_file.get(c)) shader_str += c;
		}

		return shader_str;
	}

	GLint JexShaderManager::LinkShaderProgram() {

		shader_program = glCreateProgram();

		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);

		glLinkProgram(shader_program);

		GLint is_linked = 0;
		glGetProgramiv(shader_program, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE) {

			GLint max_length = 0;
			glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(shader_program, max_length, &max_length, &info_log[0]);

			glDeleteProgram(shader_program);
		
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		
			return -1;
		}

		return shader_program;
	}

	void JexShaderManager::UseShaderProgram() const {
		
		glUseProgram(shader_program);

	}

	//int JexShaderManager::Compile() {

	//	//Read our shaders into the appropriate buffers
	//	std::string vertexSource = ReadShaderFromFile("VertexShader.glsl");//Get source code for vertex shader.
	//	std::string fragmentSource = ReadShaderFromFile("FragmentShader.glsl");//Get source code for fragment shader.

	//																			  //Create an empty vertex shader handle
	//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//	//Send the vertex shader source code to GL
	//	//Note that std::string's .c_str is NULL character terminated.
	//	const GLchar *source = (const GLchar *)vertexSource.c_str();
	//	glShaderSource(vertexShader, 1, &source, 0);

	//	//Compile the vertex shader
	//	glCompileShader(vertexShader);
	
	//	GLint isCompiled = 0;
	//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The maxLength includes the NULL character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the shader anymore.
	//		glDeleteShader(vertexShader);

	//		//Use the infoLog as you see fit.

	//		//In this simple program, we'll just leave
	//		return -1;
	//	}

	//	//Create an empty fragment shader handle
	//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//	//Send the fragment shader source code to GL
	//	//Note that std::string's .c_str is NULL character terminated.
	//	source = (const GLchar *)fragmentSource.c_str();
	//	glShaderSource(fragmentShader, 1, &source, 0);

	//	//Compile the fragment shader
	//	glCompileShader(fragmentShader);

	//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The maxLength includes the NULL character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the shader anymore.
	//		glDeleteShader(fragmentShader);
	//		//Either of them. Don't leak shaders.
	//		glDeleteShader(vertexShader);

	//		//Use the infoLog as you see fit.

	//		//In this simple program, we'll just leave
	//		return -1;
	//	}

	//	//Vertex and fragment shaders are successfully compiled.
	//	//Now time to link them together into a program.
	//	//Get a program object.
	//	GLuint program = glCreateProgram();

	//	//Attach our shaders to our program
	//	glAttachShader(program, vertexShader);
	//	glAttachShader(program, fragmentShader);

	//	//Link our program
	//	glLinkProgram(program);

	//	//Note the different functions here: glGetProgram* instead of glGetShader*.
	//	GLint isLinked = 0;
	//	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	//	if (isLinked == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

	//		//The maxLength includes the NULL character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

	//		//We don't need the program anymore.
	//		glDeleteProgram(program);
	//		//Don't leak shaders either.
	//		glDeleteShader(vertexShader);
	//		glDeleteShader(fragmentShader);

	//		//Use the infoLog as you see fit.

	//		//In this simple program, we'll just leave
	//		return -1;
	//	}

	//	//Always detach shaders after a successful link.
	//	glDetachShader(program, vertexShader);
	//	glDetachShader(program, fragmentShader);

	//	return program;

	//}

}