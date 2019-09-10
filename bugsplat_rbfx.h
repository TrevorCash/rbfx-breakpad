#pragma once

#define URHOBUGSPLAT_EXPORT_API __declspec(dllexport)


#ifdef URHOBUGSPLAT_STATIC
#  define URHOBUGSPLAT_API
#  define URHOBUGSPLAT_NO_EXPORT
#else
#  ifndef URHOBUGSPLAT_API
#    ifdef URHOBUGSPLAT_EXPORTS
/* We are building this library */
#      define URHOBUGSPLAT_API URHOBUGSPLAT_EXPORT_API
#    else
/* We are using this library */
#      define URHOBUGSPLAT_API __declspec(dllimport)
#    endif
#  endif

#  ifndef URHOBUGSPLAT_NO_EXPORT
#    define URHOBUGSPLAT_NO_EXPORT 
#  endif
#endif


namespace Urho3D {



}
