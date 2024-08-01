// Oleg Kotov

#pragma once

#ifdef OKOENGINE_EXPORTS
#define ENGINE_API __declspec( dllexport )
#else
#define ENGINE_API __declspec( dllimport )
#endif

