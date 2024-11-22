#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include "Thretris.hpp"

extern "C" {
EXPORT void _CacaoLaunch() {
	Thretris::GetInstance()->OnStartup();
}

EXPORT void _CacaoExiting() {
	Thretris::GetInstance()->OnShutdown();
	delete Thretris::GetInstance();
}
}
