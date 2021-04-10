#pragma once

#include "Program.h"
#include "LTexture.h"

#include <array>

class Audio : public Program
{

public:
  Audio();
  ~Audio();

private:
  static constexpr int MAX_RECORDING_DEVICES = 10;

  enum class RecordingState {
    SELECTING_DEVICE,
    STOPPED,
    RECORDING,
    RECORDED,
    PLAYBACK,
    ERROR
  };

  LTexture gPromptTexture;
  LTexture m_promptTextTexture;

  //The text textures that specify recording device names
  std::array<LTexture, MAX_RECORDING_DEVICES> gDeviceTextures;

  //Number of available devices
  int gRecordingDeviceCount = 0;

  //Recieved audio spec
  SDL_AudioSpec gReceivedRecordingSpec;
  SDL_AudioSpec gReceivedPlaybackSpec;
  //Recording data buffer
  static Uint8 *gRecordingBuffer;

  //Size of data buffer
  Uint32 gBufferByteSize = 0;

  //Position in data buffer
  static Uint32 gBufferBytePosition;

  //Maximum position in data buffer for recording
  Uint32 gBufferByteMaxPosition = 0;

  RecordingState currentState = RecordingState::SELECTING_DEVICE;

  SDL_AudioDeviceID recordingDeviceId = 0;
  SDL_AudioDeviceID playbackDeviceId = 0;

  [[nodiscard]] State loadMedia() override;
  static void audioRecordingCallback(void *userdata, unsigned char *stream, int len);
  static void audioPlaybackCallback(void *userdata, Uint8 *stream, int len);
  void handleEvents() override;
  void update() override;
  void render() override;
};
