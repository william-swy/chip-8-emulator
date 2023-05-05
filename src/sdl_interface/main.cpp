#include "sound.h"

#include <thread>

int main() {
  sdl_interface::Audio audio{};

  audio.buzzer_on();

  std::this_thread::sleep_for(std::chrono::seconds(5));

  audio.buzzer_off();

  std::this_thread::sleep_for(std::chrono::seconds(5));

}