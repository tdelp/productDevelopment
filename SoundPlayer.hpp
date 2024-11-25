#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include "all_headers.hpp"

class SoundPlayer {
public:
    SoundPlayer() {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Error initializing SDL audio: " << SDL_GetError() << std::endl;
            std::quick_exit(1);
        }
    }

    ~SoundPlayer() {
        if (audioDevice != 0) {
            SDL_CloseAudioDevice(audioDevice);
            SDL_FreeWAV(audioData);
        }
        SDL_QuitSubSystem(SDL_INIT_AUDIO);  // Quit only audio subsystem
    }

    bool loadSound(const std::string& soundFile) {
        if (SDL_LoadWAV(soundFile.c_str(), &audioSpec, &audioData, &audioSize) == nullptr) {
            std::cerr << "Failed to load WAV file: " << SDL_GetError() << std::endl;
            return false;
        }
        audioDevice = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, 0);
        if (audioDevice == 0) {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
            SDL_FreeWAV(audioData);
            return false;
        }
        return true;
    }

    void playSound() const {
        if (audioDevice != 0) {
            SDL_ClearQueuedAudio(audioDevice);
            SDL_QueueAudio(audioDevice, audioData, audioSize);
            SDL_PauseAudioDevice(audioDevice, 0);  // Start playback
        }
    }

private:
    SDL_AudioSpec audioSpec{};
    Uint8* audioData = nullptr;
    Uint32 audioSize = 0;
    SDL_AudioDeviceID audioDevice = 0;
};

#endif // SOUND_PLAYER_HPP
