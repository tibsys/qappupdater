#ifndef QAPPUPDATERPLATFORM_H
#define QAPPUPDATERPLATFORM_H

/**
 * Macro definition for DLL exporting symbols
 */
#ifdef _WIN32
#ifdef _QAPPUPDATER_SHARED
#define QAPPUPDATER_API __declspec(dllexport)
#else
#ifdef _QAPPUPDATER_STATIC
#define QAPPUPDATER_API
#else
#define QAPPUPDATER_API __declspec(dllimport)
#endif
#endif
#else
#define QAPPUPDATER_API
#endif

/**
 * Disable excessive warnings on Windows
 */
#ifdef _WIN32
#pragma warning(disable:4251) //DLL Interface missing
#endif


#endif // PLATFORM_H
