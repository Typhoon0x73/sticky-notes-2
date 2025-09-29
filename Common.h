#pragma once

/* check for apple platforms */
#if defined(__APPLE__) || defined(__MACH__)
#define DEBUG_BREAK_POINT() __builtin_trap()
#elif defined(_WIN32)
#define DEBUG_BREAK_POINT() __debugbreak()
#else
#define DEBUG_BREAK_POINT() ((void)0)
#endif // PLATFORM


#ifdef _DEBUG
#define STN_DEBUG_BREAK(b) if (!!!(b)) DEBUG_BREAK_POINT()
#else
#define STN_DEBUG_BREAK(b) ((void)0)
#endif // _DEBUG
