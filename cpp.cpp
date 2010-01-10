
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


