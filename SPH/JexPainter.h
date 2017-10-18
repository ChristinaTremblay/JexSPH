#pragma once

#ifndef JEX_PAINTER_H
#define JEX_PAINTER_H

#include <OpenGL\glew.h>
#include <OpenGL\freeglut.h>

namespace Jex {

	class JexPainter {
	private:
	public:
		JexPainter();
		~JexPainter();

		void InitRenderEnvir();
		void DisplayFunc();
		void IdleFunc();
		void MouseFunc();
		void KeyboardFunc();

	};

}

#endif