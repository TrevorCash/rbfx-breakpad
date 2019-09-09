#pragma once

#define URHOBREAKPAD_EXPORT_API __declspec(dllexport)


#ifdef URHOBREAKPAD_STATIC
#  define URHOBREAKPAD_API
#  define URHOBREAKPAD_NO_EXPORT
#else
#  ifndef URHOBREAKPAD_API
#    ifdef URHOBREAKPAD_EXPORTS
/* We are building this library */
#      define URHOBREAKPAD_API URHOBREAKPAD_EXPORT_API
#    else
/* We are using this library */
#      define URHOBREAKPAD_API __declspec(dllimport)
#    endif
#  endif

#  ifndef URHOBREAKPAD_NO_EXPORT
#    define URHOBREAKPAD_NO_EXPORT 
#  endif
#endif


namespace Urho3D {



}
