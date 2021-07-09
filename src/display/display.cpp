#include "display.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <array>
#include <memory>

#include "input_events.h"

class display::Display::sdl_pimpl {
public:
  sdl_pimpl(int screen_width, int screen_height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = std::unique_ptr<SDL_Window, sdl_deleter>(
        SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width,
                         screen_height, SDL_WINDOW_SHOWN),
        sdl_deleter());

    renderer = std::unique_ptr<SDL_Renderer, sdl_deleter>(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
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

  void draw_square(unsigned char red, unsigned char green, unsigned char blue, int x_pos, int y_pos,
                   unsigned int scaling_factor) const {
    SDL_SetRenderDrawColor(renderer.get(), red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_Rect r;
    r.x = x_pos;
    r.y = y_pos;
    r.w = scaling_factor;
    r.h = scaling_factor;
    SDL_RenderFillRect(renderer.get(), &r);
  }

  void render_display() const { SDL_RenderPresent(renderer.get()); }

  void delay(unsigned int milli_sec) const { SDL_Delay(milli_sec); }

  enum input_events::Events handle_input() {
    SDL_PollEvent(&event);
    // some hard coded mappings using the layout shown here
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy2
    // A nasty switch...
    switch (event.type) {
      case SDL_QUIT:
        return input_events::Events::quit;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_x:
            return input_events::Events::zero_pressed;
          case SDLK_1:
            return input_events::Events::one_pressed;
          case SDLK_2:
            return input_events::Events::two_pressed;
          case SDLK_3:
            return input_events::Events::three_pressed;
          case SDLK_q:
            return input_events::Events::four_pressed;
          case SDLK_w:
            return input_events::Events::five_pressed;
          case SDLK_e:
            return input_events::Events::six_pressed;
          case SDLK_a:
            return input_events::Events::seven_pressed;
          case SDLK_s:
            return input_events::Events::eight_pressed;
          case SDLK_d:
            return input_events::Events::nine_pressed;
          case SDLK_z:
            return input_events::Events::a_pressed;
          case SDLK_c:
            return input_events::Events::b_pressed;
          case SDLK_4:
            return input_events::Events::c_pressed;
          case SDLK_r:
            return input_events::Events::d_pressed;
          case SDLK_f:
            return input_events::Events::e_pressed;
          case SDLK_v:
            return input_events::Events::f_pressed;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_x:
            return input_events::Events::zero_released;
          case SDLK_1:
            return input_events::Events::one_released;
          case SDLK_2:
            return input_events::Events::two_released;
          case SDLK_3:
            return input_events::Events::three_released;
          case SDLK_q:
            return input_events::Events::four_released;
          case SDLK_w:
            return input_events::Events::five_released;
          case SDLK_e:
            return input_events::Events::six_released;
          case SDLK_a:
            return input_events::Events::seven_released;
          case SDLK_s:
            return input_events::Events::eight_released;
          case SDLK_d:
            return input_events::Events::nine_released;
          case SDLK_z:
            return input_events::Events::a_released;
          case SDLK_c:
            return input_events::Events::b_released;
          case SDLK_4:
            return input_events::Events::c_released;
          case SDLK_r:
            return input_events::Events::d_released;
          case SDLK_f:
            return input_events::Events::e_released;
          case SDLK_v:
            return input_events::Events::f_released;
          default:
            break;
        }
        break;
      default:
        break;
    }
    return input_events::Events::none;
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
  p_impl->draw_square(red, green, blue, original_x_pos * scaling_factor,
                      original_y_pos * scaling_factor, scaling_factor);
}

void display::Display::render_display() const { p_impl->render_display(); }

void display::Display::delay(unsigned int milli_sec) const { p_impl->delay(milli_sec); }

enum input_events::Events display::Display::handle_input() const { return p_impl->handle_input(); }

long long display::Display::get_performance_counter() const noexcept {
  return SDL_GetPerformanceCounter();
}

long long display::Display::get_performance_frequency() const noexcept {
  return SDL_GetPerformanceFrequency();
}
