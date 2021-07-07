#include "display.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <array>
#include <memory>

#include "display.h"
#include "keys.h"

class display::Display::sdl_pimpl {
public:
  sdl_pimpl(int screen_width, int screen_height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = std::unique_ptr<SDL_Window, sdl_deleter>(
        SDL_CreateWindow("Title", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN),
        sdl_deleter());

    renderer = std::unique_ptr<SDL_Renderer, sdl_deleter>(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  }

  ~sdl_pimpl() { SDL_Quit(); }

  // Should not be cloned so disable copy and assignment
  sdl_pimpl(const sdl_pimpl&) = delete;

  sdl_pimpl& operator=(const sdl_pimpl&) = delete;

  void draw_pixel(unsigned char red, unsigned char green, unsigned char blue, int x_pos,
                  int y_pos) const {
    SDL_SetRenderDrawColor(renderer.get(), red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer.get(), x_pos, y_pos);
  }

  void render_display() const { SDL_RenderPresent(renderer.get()); }

  void delay(unsigned int milli_sec) const { SDL_Delay(milli_sec); }

  bool stop_rendering() { return SDL_PollEvent(&event) && event.type == SDL_QUIT; }

  KeyPressed handle_keyboard() {
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_KEYDOWN:
        keys[event.key.keysym.sym] = true;
        break;
      case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        break;
      default:
        break;
    }

    // some hard coded mappings using the layout shown here
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy2
    return KeyPressed{keys[SDLK_x], keys[SDLK_1], keys[SDLK_2], keys[SDLK_3],
                      keys[SDLK_q], keys[SDLK_w], keys[SDLK_e], keys[SDLK_a],
                      keys[SDLK_s], keys[SDLK_d], keys[SDLK_z], keys[SDLK_c],
                      keys[SDLK_4], keys[SDLK_r], keys[SDLK_f], keys[SDLK_v]};
  }

private:
  struct sdl_deleter {
    void operator()(SDL_Window* ptr) const { SDL_DestroyWindow(ptr); };
    void operator()(SDL_Renderer* ptr) const { SDL_DestroyRenderer(ptr); };
  };

  SDL_Event event;

  std::array<bool, 322> keys;

  std::unique_ptr<SDL_Window, sdl_deleter> window;

  std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;
};

display::Display::Display(int screen_width, int screen_height) {
  p_impl = std::make_unique<sdl_pimpl>(screen_width, screen_height);
}

display::Display::~Display() = default;

void display::Display::draw_pixel(unsigned char red, unsigned char green, unsigned char blue,
                                  int x_pos, int y_pos) const {
  p_impl->draw_pixel(red, green, blue, x_pos, y_pos);
}

void display::Display::draw_scaled_pixel(unsigned char red, unsigned char green, unsigned char blue,
                                         int original_x_pos, int original_y_pos,
                                         unsigned int scaling_factor) const {
  for (unsigned int i = 0; i < scaling_factor; i++) {
    for (unsigned int j = 0; j < scaling_factor; j++) {
      draw_pixel(red, green, blue, original_x_pos * scaling_factor + i,
                 original_y_pos * scaling_factor + j);
    }
  }
}

void display::Display::render_display() const { p_impl->render_display(); }

void display::Display::delay(unsigned int milli_sec) const { p_impl->delay(milli_sec); }

bool display::Display::stop_rendering() const { return p_impl->stop_rendering(); }

KeyPressed display::Display::handle_keyboard() const { return p_impl->handle_keyboard(); }
