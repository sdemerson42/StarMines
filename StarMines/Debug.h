#pragma once
#include <crtdbg.h>

#ifdef _DEBUG
#define ASSERT _ASSERTE
#define VERIFY ASSERT
#define _VERIFY(expr, val) ASSERT(expr == val)
#else
#define ASSERT __noop
#define VERIFFY(expr) (expr)
#define _VERIFY(expr, val) (expr)
#endif
