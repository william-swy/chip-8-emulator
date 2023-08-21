#include "timer.h"

namespace arch {
  Timer::~Timer() noexcept {
    countdown_thread_should_run = false;

    if (countdown_thread.joinable()) {
      countdown_thread.join();
    }
  }

  unsigned char Timer::get_timer_register() const noexcept { return timer_register; }

  void Timer::set_timer_register(unsigned char register_value) noexcept {
    std::lock_guard<std::mutex> guard(timer_register_mutex);
    timer_register = register_value;
  }

  void Timer::start_device(const std::chrono::milliseconds decrement_time) noexcept {
    countdown_thread = std::thread(
        [&](const std::chrono::milliseconds sleep_duration) {
          while (countdown_thread_should_run) {
            std::this_thread::sleep_for(sleep_duration);
            std::lock_guard<std::mutex> guard(timer_register_mutex);
            if (timer_register > 0) {
              timer_register--;
            }
          }
        },
        decrement_time);
  }

}  // namespace arch