#include <cstdio>
#include <cerrno>
extern "C" {
#include <fcntl.h>
}
#include <ios>
#include <dar/libdar.hpp>

void dar_check_version();

extern "C" const char * cplusplus_hello_str();
extern "C"  int darfs_exists(const char * path);
extern "C" mode_t darfs_mode(const char * path);
extern "C" off_t darfs_size(const char * path);

void dar_check_version()
{
   try
   {
      libdar::U_I maj, med, min;

        // first we MUST call get_version()

      libdar::get_version(maj, med, min);

      if(maj != libdar::LIBDAR_COMPILE_TIME_MAJOR ||
         med < libdar::LIBDAR_COMPILE_TIME_MEDIUM)
        throw libdar::Erange("initialization",
        "we are linking against a wrong libdar");
   }
   catch(libdar::Egeneric & e)
   {
      std::cerr << e.get_message() << std::endl;
   }		
}

extern "C" const char * cplusplus_hello_str()
{
	return "Hello from C++\n";
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
	return 123L;
}

