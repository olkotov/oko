// Oleg Kotov

#pragma once

#include "baseapi.h"
#include <stdint.h>

class Profiler;

class ENGINE_API ProfilerRenderer
{
public:

	ProfilerRenderer();
	~ProfilerRenderer();

	void draw( Profiler* profiler );
	void clear();

private:

	class Shader* m_textShader = nullptr;
	class TextElement* m_textElement = nullptr;
};

