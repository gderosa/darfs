#include <cstdio>
#include <cerrno>
extern "C" {
#include <fcntl.h>
}

extern "C" const char * cplusplus_hello_str();
extern "C"  int darfs_exists(const char * path);
extern "C" mode_t darfs_mode(const char * path);
extern "C" off_t darfs_size(const char * path);


class C
{
	public:

	const char * f();
};

const char * C::f()
{
	return "Hello from C++\n";
}

extern "C" const char * cplusplus_hello_str()
{
	C c;

	return c.f();
}

extern "C"  int darfs_exists(const char * path) 
{
	return true;
}

extern "C" mode_t darfs_mode(const char * path)
{
	return S_IFREG | 0777;
}

extern "C" off_t darfs_size(const char * path)
{
	return (off_t) 10;
}
