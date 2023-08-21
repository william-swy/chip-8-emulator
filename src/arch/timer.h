#pragma once

#include <chrono>
#include <mutex>
#include <thread>

namespace arch {
  class Timer {
  public:
    Timer() = default;

    ~Timer() noexcept;

    Timer(const Timer& other) = delete;

    Timer(Timer&& other) = delete;

    Timer& operator=(const Timer& other) = delete;

    Timer& operator=(Timer&& other) = delete;

    [[nodiscard]] unsigned char get_timer_register() const noexcept;

    void set_timer_register(unsigned char register_value) noexcept;

    void start_device(const std::chrono::milliseconds decrement_time) noexcept;

  private:
    unsigned char timer_register{0};

    bool countdown_thread_should_run{true};

    std::thread countdown_thread{};

    std::mutex timer_register_mutex{};
  };

  struct DelayTimer {
    Timer timer{};
  };

  struct SoundTimer {
    Timer timer{};
  };
}  // namespace arch