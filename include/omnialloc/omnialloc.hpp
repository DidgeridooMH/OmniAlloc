#pragma once

#ifdef _WIN32
#ifdef OMNIALLOC_LIBRARY_BUILD
#define OMNIALLOC_EXPORT __declspec(dllexport)
#else
#define OMNIALLOC_EXPORT __declspec(dllimport)
#endif
#else
#define OMNIALLOC_EXPORT
#endif


