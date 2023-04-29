// Inspired from segross 
// Distributed under the MIT License (MIT) (see accompanying LICENSE file)
// Original Source Code: https://github.com/DarkMattersSolutions/UnrealImGui/commit/20a7675787209613eb5e18dec50f6af6b6701539

#include <Runtime/Launch/Resources/Version.h>

#define BELOW_ENGINE_VERSION(Major, Minor)  (ENGINE_MAJOR_VERSION < (Major) || (ENGINE_MAJOR_VERSION == (Major) && ENGINE_MINOR_VERSION < (Minor)))
#define FROM_ENGINE_VERSION(Major, Minor)   !BELOW_ENGINE_VERSION(Major, Minor)


// One place to define compatibility with older engine versions.

#define ENGINE_COMPATIBILITY_NOENUMFORWARDING       BELOW_ENGINE_VERSION(4, 27)

#define ENGINE_COMPATIBILITY_NOOBJECTPTR            BELOW_ENGINE_VERSION(4, 27)