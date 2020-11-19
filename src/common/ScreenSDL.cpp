/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare,
 *   Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence
 *   Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  ScreenSDL.cpp
 *
 *  Supports displaying the screen via SDL.
 **************************************************************************** */
#ifdef SDL_SUPPORT
#include "common/SDL2.hpp"

#include "common/ScreenSDL.hpp"
#include "common/Log.hpp"

#include <cmath>

using namespace std;

namespace ale {

ScreenSDL::ScreenSDL(OSystem* osystem) : Screen(osystem), mediaSource(NULL), sound(NULL), colourPalette(NULL) {
  mediaSource = &osystem->console().mediaSource();
  sound = &osystem->sound();
  colourPalette = &osystem->colourPalette();
  renderDelay = 1000 / osystem->console().getFrameRate();

  screenHeight = mediaSource->height();
  screenWidth = mediaSource->width();

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("Failed to initialize SDL");
  }

  pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

  int scaleFactor = getScaleFactor();
  window = SDL_CreateWindow("The Arcade Learning Environment",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                windowWidth * scaleFactor,
                windowHeight * scaleFactor,
                SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == nullptr) {
    throw std::runtime_error("Failed to initialize SDL window");
  }


  SDL_RendererInfo rendererInfo;
  if (SDL_GetRenderDriverInfo(0, &rendererInfo) < 0) {
    throw std::runtime_error("Failed to query renderer 0");
  }
  if(!strcmp(SDL_GetCurrentVideoDriver(), "dummy")) {
    rendererInfo.flags = 0;
  }
  renderer = SDL_CreateRenderer(window, -1, rendererInfo.flags);
  if (renderer == nullptr) {
    throw std::runtime_error("Failed to initialize SDL renderer");
  }
  SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

  texture = SDL_CreateTexture(renderer, pixelFormat->format, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
  if (texture == nullptr) {
    throw std::runtime_error("Failed to initialize SDL texture");
  }

  surface = SDL_CreateRGBSurface(0,
    windowWidth,
    windowHeight,
    pixelFormat->BitsPerPixel,
    pixelFormat->Rmask,
    pixelFormat->Gmask,
    pixelFormat->Bmask,
    pixelFormat->Amask);
  if (surface == nullptr) {
    throw std::runtime_error("Failed to initialize SDL surface");
  }

  lastRender = SDL_GetTicks();
}

ScreenSDL::~ScreenSDL() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  SDL_Quit();
}

void ScreenSDL::render() {
  uint32_t* out = reinterpret_cast<uint32_t*>(surface->pixels);
  uint8_t* buffer = mediaSource->currentFrameBuffer();
  uint32_t pitch = surface->pitch / pixelFormat->BytesPerPixel;

  uint32_t bufferOffset = 0, offsetY = 0, pos;
  for (uint32_t y = 0; y < screenHeight; ++y) {
    pos = offsetY;
    for(uint32_t x = screenWidth / 2; x; --x) {
      out[pos++] = colourPalette->getRGB(buffer[bufferOffset++]);
      out[pos++] = colourPalette->getRGB(buffer[bufferOffset++]);
    }
    offsetY += pitch;
  }

  SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  this->poll();

  uint32_t thisTick = SDL_GetTicks();
  uint32_t delta = thisTick - min(lastRender, thisTick);
  if (delta < renderDelay) {
    SDL_Delay(renderDelay - delta);
  } else {
    lastRender = thisTick + delta - renderDelay;
  }
}

void ScreenSDL::poll() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    handleSDLEvent(event);
  }
}

int ScreenSDL::getScaleFactor() {
  SDL_DisplayMode mode;
  SDL_GetCurrentDisplayMode(0, &mode);

  float maxScaleProportion = 0.3;

  int widthHeuristic = ceil(mode.w * maxScaleProportion);
  int heightHeuristic = ceil(mode.h * maxScaleProportion);
  int scaleFactor;

  if (widthHeuristic > heightHeuristic) {
    scaleFactor = floor(widthHeuristic / windowWidth);
  } else {
    scaleFactor = floor(heightHeuristic / windowHeight);
  }

  return max(scaleFactor, 1);
}

void ScreenSDL::handleSDLEvent(const SDL_Event& event) {
  // TODO: OSD for keystrokes
  switch(event.type) {
    case SDL_QUIT:
      std::exit(0);
      break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
        case SDLK_RIGHT:
          renderDelay = renderDelay > 5 ? renderDelay - 5 : 0;
          break;
        case SDLK_LEFT:
          renderDelay = renderDelay + 5;
          break;
        case SDLK_DOWN:
          for (int i = 0; i < 5; i++)
            sound->adjustVolume(-1);
          break;
        case SDLK_UP:
          for (int i = 0; i < 5; i++)
            sound->adjustVolume(1);
          break;
      }
  }
}

} //namespace ale

#endif // SDL_SUPPORT
