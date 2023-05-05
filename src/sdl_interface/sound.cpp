#include "sound.h"

#include <SDL2/SDL.h>

#include <cmath>
#include <numbers>
#include <span>
#include <stdexcept>
#include <string>

namespace sdl_interface {
  constexpr int buzzer_frequency{44100};
  constexpr int buzzer_amplitude{28000};

  void audio_callback(void* userdata, Uint8* buff, int length);

  class AudioDevice {
  public:
    AudioDevice(double sound_frequency): sound_frequency(sound_frequency)  {
      desired_spec = {.freq = buzzer_frequency,
                      .format = AUDIO_S16SYS,
                      .channels = 1,
                      .samples = 2048,
                      .callback = audio_callback,
                      .userdata = this};
      device_id = ::SDL_OpenAudioDevice(NULL, 0, &desired_spec, &actual_spec, 0);
      if (device_id == 0) {
        throw std::runtime_error("Failed to open audio device: " + std::string(::SDL_GetError()));
      }
    }

    ~AudioDevice() { ::SDL_CloseAudioDevice(device_id); }

    ::SDL_AudioSpec desired_spec;
    ::SDL_AudioSpec actual_spec;
    ::SDL_AudioDeviceID device_id;
    double sound_frequency;
    double vel{0.0};
  };

  void audio_callback(void* userdata, Uint8* buff, int length) {
    auto device{reinterpret_cast<AudioDevice*>(userdata)};
    auto stream{std::span<Sint16>(reinterpret_cast<Sint16*>(buff),
                                  length / (sizeof(Sint16) / sizeof(Uint8)))};
    for (auto& idx : stream) {
      idx = static_cast<Sint16>(buzzer_amplitude * std::sin(device->vel * 2 * std::numbers::pi / buzzer_frequency));
      device->vel += device->sound_frequency;
    }
  }

  Audio::Audio() {
    auto err = ::SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (err) {
      throw std::runtime_error("Failed to initialize SDL audio subsystem: "
                               + std::string(::SDL_GetError()));
    }
    audioDevice = std::make_unique<AudioDevice>(293.665);
  }

  Audio::~Audio() { ::SDL_QuitSubSystem(SDL_INIT_AUDIO); }

  void Audio::buzzer_on() const { ::SDL_PauseAudioDevice(audioDevice->device_id, 0); }
  void Audio::buzzer_off() const { ::SDL_PauseAudioDevice(audioDevice->device_id, 1); }

}  // namespace sdl_interface