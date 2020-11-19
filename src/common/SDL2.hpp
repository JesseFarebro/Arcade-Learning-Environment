#ifndef SDL2_HPP
#define SDL2_HPP

#include <SDL.h>
#include <stdexcept>

#include "common/DynamicLoad.hpp"

namespace ale {

// Dynamic library name per platform
#if defined(WIN32)
  #define SDL2_LIBRARY_NAME "SDL2.dll"
#elif defined(__APPLE__)
  #define SDL2_LIBRARY_NAME "libsdl2-2.0.0.dylib"
#else
  #define SDL2_LIBRARY_NAME "libSDL2-2.0.so"
#endif

#define OVERRIDE extern "C"

// Macro to define original function pointer in our namespace
#define DEFINE_SDL2_POINTER(FUNC) \
namespace SDL2 { \
  decltype(&FUNC) FUNC; \
}

// Dynamically link function to pointer, see "common/DynamicLoad.hpp"
#define LINK_NAMESPACE_SDL2(FUNC) \
if(!DynamicLinkFunction((void**)&SDL2::FUNC, #FUNC, SDL2_LIBRARY_NAME)) { \
  throw std::runtime_error("Failed to bind " #FUNC " in " SDL2_LIBRARY_NAME ".\n" \
                           "If " SDL2_LIBRARY_NAME " is installed try specifying LD_LIBRARY_PATH."); \
}

/* SDL_Init */
OVERRIDE int SDL_Init(uint32_t flags);

/** SDL_AllocFormat */
OVERRIDE SDL_PixelFormat* SDL_AllocFormat(uint32_t pixelFormat);

/* SDL_CloseAudio */
OVERRIDE void SDL_CloseAudio(void);

/* SDL_CreateRGBSurface */
OVERRIDE SDL_Surface* SDL_CreateRGBSurface(uint32_t flags,
                                           int      width,
                                           int      height,
                                           int      depth,
                                           uint32_t Rmask,
                                           uint32_t Gmask,
                                           uint32_t Bmask,
                                           uint32_t Amask);

/* SDL_CreateRenderer */
OVERRIDE SDL_Renderer* SDL_CreateRenderer(SDL_Window* window,
                                          int         index,
                                          uint32_t    flags);

/* SDL_CreateTexture */
OVERRIDE SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer,
                                        uint32_t      format,
                                        int           access,
                                        int           w,
                                        int           h);

/* SDL_CreateWindow */
OVERRIDE SDL_Window* SDL_CreateWindow(const char* title,
                                      int         x,
                                      int         y,
                                      int         w,
                                      int         h,
                                      uint32_t    flags);

/* SDL_Delay */
OVERRIDE void SDL_Delay(uint32_t ms);

/* SDL_DestroyRenderer */
OVERRIDE void SDL_DestroyRenderer(SDL_Renderer* renderer);

/* SDL_DestroyTexture */
OVERRIDE void SDL_DestroyTexture(SDL_Texture* texture);

/* SDL_DestroyWindow */
OVERRIDE void SDL_DestroyWindow(SDL_Window* window);

/* SDL_FreeSurface */
OVERRIDE void SDL_FreeSurface(SDL_Surface* surface);

/* SDL_GetError */
OVERRIDE const char* SDL_GetError(void);

/* SDL_GetTicks */
OVERRIDE uint32_t SDL_GetTicks(void);

/* SDL_InitSubSystem */
OVERRIDE int SDL_InitSubSystem(uint32_t flags);

/* SDL_LockAudio */
OVERRIDE void SDL_LockAudio(void);

/* SDL_OpenAudio */
OVERRIDE int SDL_OpenAudio(SDL_AudioSpec* desired,
                           SDL_AudioSpec* obtained);

/* SDL_PauseAudio */
OVERRIDE void SDL_PauseAudio(int pause_on);

/* SDL_PollEvent */
OVERRIDE int SDL_PollEvent(SDL_Event* event);

/* SDL_Quit */
OVERRIDE void SDL_Quit(void);

/* SDL_RenderCopy */
OVERRIDE int SDL_RenderCopy(SDL_Renderer*   renderer,
                            SDL_Texture*    texture,
                            const SDL_Rect* srcrect,
                            const SDL_Rect* dstrect);

/* SDL_RenderPresent */
OVERRIDE void SDL_RenderPresent(SDL_Renderer* renderer);

/* SDL_RenderSetIntegerScale */
OVERRIDE int SDL_RenderSetIntegerScale(SDL_Renderer* renderer,
                                       SDL_bool      enable);

/* SDL_RenderSetLogicalSize */
OVERRIDE int SDL_RenderSetLogicalSize(SDL_Renderer* renderer,
                                      int           w,
                                      int           h);

/* SDL_UnlockAudio */
OVERRIDE void SDL_UnlockAudio(void);

/* SDL_UpdateTexture */
OVERRIDE int SDL_UpdateTexture(SDL_Texture*    texture,
                               const SDL_Rect* rect,
                               const void*     pixels,
                               int             pitch);

/* SDL_WasInit */
OVERRIDE uint32_t SDL_WasInit(uint32_t flags);

/* SDL_GetCurrentDisplayMode */
OVERRIDE int SDL_GetCurrentDisplayMode(int              displayIndex,
                                       SDL_DisplayMode* mode);

/* SDL_GetRenderDriverInfo */
OVERRIDE int SDL_GetRenderDriverInfo(int               index,
                                     SDL_RendererInfo* info);

/* SDL_GetCurrentVideoDriver */
OVERRIDE const char* SDL_GetCurrentVideoDriver(void);

} // namespace ale
#endif // SDL2_HPP
