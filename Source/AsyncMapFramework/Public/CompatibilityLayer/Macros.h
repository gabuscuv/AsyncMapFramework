#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif