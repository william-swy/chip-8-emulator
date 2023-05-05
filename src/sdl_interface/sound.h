#pragma once

#include <memory>

namespace sdl_interface {
  class AudioDevice;

  class Audio {
  public:
    Audio();
    ~Audio();

    Audio(const Audio& other) = delete;
    Audio& operator=(const Audio& other) = delete;
    Audio(const Audio&& other) = delete;
    Audio& operator=(const Audio&& other) = delete;

    void buzzer_on() const;
    void buzzer_off() const;

  private:
    std::unique_ptr<AudioDevice> audioDevice;
  };
}  // namespace sdl_interface