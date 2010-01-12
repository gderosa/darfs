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
extern "C" int open_dar_archive(const char * basepath);

static libdar::archive * archive_object_ptr = NULL;



  // our own callback functions.
  // for the illustration of what theses 'context' arguments
  // can be used for we will imagine the situation where
  // multiple windows or multiple threads may each one use
  // libdar, but all share the same callback functions.
 
//typedef t_window_type int;

  // this is an arbitrary type that here we will say
  // points to a graphical window object wrapped in a C++
  // class.
  // Note that the method show() wait_for_click() and so on
  // attributed to the t_win class are absolutely
  // imaginary. Any link to an existing class is a pure
  // coincidence...

void warning_callback(const std::string &x, void *context)
{
    //(t_win *)(context)->show(x);
}
 
bool answer_callback(const std::string &x, void *context)
{
	return true;
	/*
    click_type ret;

    (t_win *)(context)->show(x);
    ret = (t_win *)(context)->wait_for_click();

    return ret == click_OK;
    */
}

std::string string_callback(const std::string &x, bool echo, void *context)
{
	return "true";
	/*
    (t_win *)(context)->show(x);
    if(!echo)
      (t_win *)(context)->set_hide_typed_char();
    (t_win *)(context)->wait_for_click();
    return (t_win *)(context)->read_text();
    */
}

int get_current_windows_id()
{
		return 0;
}

  // So now each window can have its user_interaction object based on the same
  // user_interaction_callback object pointing to the same functions.
  // user_interaction_callback objects can be shared among different window objects

libdar::user_interaction_callback dialog =
        libdar::user_interaction_callback(&warning_callback, &answer_callback, &string_callback,
                                          (void *)get_current_windows_id());

  // just the "context" argument changes, and will be passed as is from the constructor to the callback
  // functions




extern "C" int open_dar_archive(const char * basepath)
{
	dar_check_version(); // initializes too (mandatory!) 
	try {
	libdar::archive *archive_object = new libdar::archive(dialog,     
        "/tmp",  // where is the archive
        "etc", // slice name
        "dar",   // dar's archive extensions
        libdar::crypto_none,
        "",
        20480, // theses three previous are for encryptions
        "",    // not used as we didn't gave "-" as
        "",    // slice name
        "",    // no command executed for now
        true); // no verbose output
        
        // archive_object_ptr = &archive_object;
      }
      catch(libdar::Egeneric & e)
      {
         std::cerr << e.get_message() << std::endl;
      }  
        
        
        return 0;
}

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

