/*
 * config_compiler.h
 *
 *  Created on: 2014年10月10日
 *      Author: Fifi Lyu
 */

#ifndef INCLUDE_ZPSOE_CONFIG_COMPILER_H
#define INCLUDE_ZPSOE_CONFIG_COMPILER_H

#ifdef _MSC_VER

#   ifdef ZPSOE_BUILD_STATIC_LIB
#       define ZPSOE_SHARED_LIB_API
#   else
#       ifdef ZPSOE_BUILD_SHARED_LIB
#           define ZPSOE_SHARED_LIB_API __declspec(dllexport)
#       else
#           define ZPSOE_SHARED_LIB_API __declspec(dllimport)
    #endif
#   endif

#elif defined(__GNUC__)

#   ifdef ZPSOE_BUILD_STATIC_LIB
#       define ZPSOE_SHARED_LIB_API
#   else
#        ifdef ZPSOE_BUILD_SHARED_LIB
#           define ZPSOE_SHARED_LIB_API __attribute__ ((visibility ("default")))
#        else
            // If you use -fvisibility=hidden in GCC, exception handling and RTTI
            // would break if visibility wasn't set during export _and_ import
            // because GCC would immediately forget all type infos encountered.
            // See http://gcc.gnu.org/wiki/Visibility
#           define ZPSOE_SHARED_LIB_API __attribute__ ((visibility ("default")))
        #endif
#   endif

#else

  #error Unknown compiler

#endif

#endif // INCLUDE_ZPSOE_CONFIG_COMPILER_H
