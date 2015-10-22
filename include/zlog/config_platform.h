/*
 * config_platform.h
 *
 *  Created on: 2014年10月10日
 *      Author: Fifi Lyu
 */

#ifndef INCLUDE_ZPSOE_CONFIG_PLATFORM_H
#define INCLUDE_ZPSOE_CONFIG_PLATFORM_H

// Platform recognition
#if defined(WIN32) || defined(_WIN32)

    #define PLATFORM_WIN32 1

#elif defined(__unix__) || defined(unix)

    #define PLATFORM_LINUX 1

#else

    #error Unknown platform

#endif

#endif // INCLUDE_ZPSOE_CONFIG_PLATFORM_H
