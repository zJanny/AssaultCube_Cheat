#include "UI.h"
#include "kiero/kiero.h"
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>

namespace UI
{

	typedef void(APIENTRY* glDrawElements_t)(GLenum mode, GLsizei count, GLenum type, const void* indices);
	glDrawElements_t o_glDrawElements = nullptr;

	void APIENTRY hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {

		return o_glDrawElements(mode, count, type, indices);
	}


	void initKiero()
	{
		if (kiero::init(kiero::RenderType::OpenGL) != kiero::Status::Success)
		{
			throw std::runtime_error("Error on kiero init");
		}

		std::cout << "Kiero init successful" << std::endl;

		if (kiero::bind(67, (void**)&o_glDrawElements, hk_glDrawElements) != kiero::Status::Success)
		{
			throw std::runtime_error("Error binding kiero hook");
		}

		std::cout << "Kiero bind successful" << std::endl;
	}
}