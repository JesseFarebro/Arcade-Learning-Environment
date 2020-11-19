set(VCPKG_TARGET_ARCHITECTURE x64)
if(PORT MATCHES "sdl")
  set(VCPKG_LIBRARY_LINKAGE dynamic)
  set(VCPKG_CRT_LINKAGE dynamic)
else()
  set(VCPKG_LIBRARY_LINKAGE static)
  set(VCPKG_CRT_LINKAGE static)
endif()

set(VCPKG_CMAKE_SYSTEM_NAME Darwin)

if(DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
  set(VCPKG_OSX_DEPLOYMENT_TARGET "$ENV{MACOSX_DEPLOYMENT_TARGET}")
endif()
