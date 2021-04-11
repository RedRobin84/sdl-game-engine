#include "Audio.h"
#include "Logger.h"
#include "Renderer.h"

#include <stdexcept>
#include <string_view>
#include <sstream>

namespace {
constexpr int MAX_RECORDING_SECONDS = 5;
constexpr int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;
constexpr std::string_view PROMPT_MESSAGE = "Select your recording device:";
constexpr std::string_view RECORDING_ERROR_MESSAGE = "Failed to open recording device!";
constexpr std::string_view PLAYBACK_ERROR_MESSAGE = "Failed to open playback device!";
constexpr std::string_view RECORDING_PROMPT_MESSAGE = "Press 1 to record for 5 seconds.";
constexpr std::string_view RECORDING_PROGRESS = "Recording...";
}// namespace

Uint32 Audio::gBufferBytePosition = 0;
Uint8 *Audio::gRecordingBuffer = nullptr;

Audio::Audio() : Program(ProgramTypeEnum::AUDIO)
{
  if (loadMedia() == State::FAILURE) {
    throw std::runtime_error("Audio::init: Some exceptions occured during program initialization.");
  }
}

Audio::~Audio()
{
  //Free playback audio
  if (gRecordingBuffer != NULL) {
    delete[] gRecordingBuffer;
    gRecordingBuffer = NULL;
  }
}

State Audio::loadMedia()
{
  //Set starting prompt
  gPromptTexture.loadFromRenderedText(PROMPT_MESSAGE, d_textColor, d_font.get());

  //Get capture device count
  gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

  //No recording devices
  if (gRecordingDeviceCount < 1) {
    Logger::error("Unable to get audio capture device! SDL Error: %s\n", SDL_GetError());
    return State::FAILURE;
  }
  //At least one device connected
  else {
    //Cap recording device count
    if (gRecordingDeviceCount > MAX_RECORDING_DEVICES) {
      gRecordingDeviceCount = MAX_RECORDING_DEVICES;
    }
    //Render device names
    std::stringstream promptText;
    for (int i = 0; i < gRecordingDeviceCount; ++i) {
      //Get capture device name
      promptText.str("");
      promptText << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);

      //Set texture from name
      gDeviceTextures[i].loadFromRenderedText(promptText.str().c_str(), d_textColor, d_font.get());
    }
  }
  return State::SUCCESS;
}

void Audio::handleEvents()
{
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      Program::endProgram();
    }
  }
  //Do current state event handling
  switch (currentState) {
  //User is selecting recording device
  case RecordingState::SELECTING_DEVICE:
    //On key press
    if (event.type == SDL_KEYDOWN) {
      //Handle key press from 0 to 9
      if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
        //Get selection index
        int index = event.key.keysym.sym - SDLK_0;
        //Index is valid
        if (index < gRecordingDeviceCount) {
          //Default audio spec
          SDL_AudioSpec desiredRecordingSpec;
          SDL_zero(desiredRecordingSpec);
          desiredRecordingSpec.freq = 44100;
          desiredRecordingSpec.format = AUDIO_F32;
          desiredRecordingSpec.channels = 2;
          desiredRecordingSpec.samples = 4096;
          desiredRecordingSpec.callback = audioRecordingCallback;

          //Open recording device
          recordingDeviceId = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, SDL_TRUE), SDL_TRUE, &desiredRecordingSpec, &gReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
          //Device failed to open
          if (recordingDeviceId == 0) {
            //Report error
            Logger::error("Failed to open recording device! SDL Error: %s", SDL_GetError());
            gPromptTexture.loadFromRenderedText(RECORDING_ERROR_MESSAGE, d_textColor, d_font.get());
            currentState = RecordingState::ERROR;
          }
          //Device opened successfully
          else {
            //Default audio spec
            SDL_AudioSpec desiredPlaybackSpec;
            SDL_zero(desiredPlaybackSpec);
            desiredPlaybackSpec.freq = 44100;
            desiredPlaybackSpec.format = AUDIO_F32;
            desiredPlaybackSpec.channels = 2;
            desiredPlaybackSpec.samples = 4096;
            desiredPlaybackSpec.callback = audioPlaybackCallback;

            //Open playback device
            playbackDeviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desiredPlaybackSpec, &gReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
            //Device failed to open
            if (playbackDeviceId == 0) {
              //Report error
              Logger::error("Failed to open playback device! SDL Error: %s", SDL_GetError());
              gPromptTexture.loadFromRenderedText(PLAYBACK_ERROR_MESSAGE, d_textColor, d_font.get());
              currentState = RecordingState::ERROR;
            }
            //Device opened successfully
            else {
              //Calculate per sample bytes
              int bytesPerSample = gReceivedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(gReceivedRecordingSpec.format) / 8);

              //Calculate bytes per second
              int bytesPerSecond = gReceivedRecordingSpec.freq * bytesPerSample;

              //Calculate buffer size
              gBufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;

              //Calculate max buffer use
              gBufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;

              //Allocate and initialize byte buffer
              gRecordingBuffer = new Uint8[gBufferByteSize];
              memset(gRecordingBuffer, 0, gBufferByteSize);

              //Go on to next state
              gPromptTexture.loadFromRenderedText(RECORDING_PROMPT_MESSAGE, d_textColor, d_font.get());
              currentState = RecordingState::STOPPED;
            }
          }
        }
      }
    }
    break;
    //User getting ready to record
  case RecordingState::STOPPED:
    //On key press
    if (event.type == SDL_KEYDOWN) {
      //Start recording
      if (event.key.keysym.sym == SDLK_1) {
        //Go back to beginning of buffer
        gBufferBytePosition = 0;

        //Start recording
        SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);

        //Go on to next state
        gPromptTexture.loadFromRenderedText(RECORDING_PROGRESS, d_textColor, d_font.get());
        currentState = RecordingState::RECORDING;
      }
    }
    break;
    //User has finished recording
  case RecordingState::RECORDED:
    //On key press
    if (event.type == SDL_KEYDOWN) {
      //Start playback
      if (event.key.keysym.sym == SDLK_1) {
        //Go back to beginning of buffer
        gBufferBytePosition = 0;

        //Start playback
        SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);

        //Go on to next state
        gPromptTexture.loadFromRenderedText("Playing...", d_textColor, d_font.get());
        currentState = RecordingState::PLAYBACK;
      }
      //Record again
      if (event.key.keysym.sym == SDLK_2) {
        //Reset the buffer
        gBufferBytePosition = 0;
        memset(gRecordingBuffer, 0, gBufferByteSize);

        //Start recording
        SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);

        //Go on to next state
        gPromptTexture.loadFromRenderedText("Recording...", d_textColor, d_font.get());
        currentState = RecordingState::RECORDING;
      }
    }
    break;
  case RecordingState::ERROR:
    throw std::logic_error("Audio::handleEvents: An error occured.");
  default:
    break;
  }
}

void Audio::audioRecordingCallback(void *userdata, unsigned char *stream, int len)
{
  //Copy audio from stream
  memcpy(&gRecordingBuffer[gBufferBytePosition], stream, len);

  //Move along buffer
  gBufferBytePosition += len;
}

void Audio::audioPlaybackCallback(void *userdata, Uint8 *stream, int len)
{
  //Copy audio to stream
  memcpy(stream, &gRecordingBuffer[gBufferBytePosition], len);

  //Move along buffer
  gBufferBytePosition += len;
}

void Audio::update() {}

void Audio::render()
{
  //Render prompt centered at the top of the screen
  gPromptTexture.render((Renderer::SCREEN_WIDTH - gPromptTexture.getWidth()) / 2, 0);
  //User is selecting
  if (currentState == RecordingState::SELECTING_DEVICE) {
    //Render device names
    int yOffset = gPromptTexture.getHeight() * 2;
    for (int i = 0; i < gRecordingDeviceCount; ++i) {
      gDeviceTextures[i].render(0, yOffset);
      yOffset += gDeviceTextures[i].getHeight() + 1;
    }
  } else if (currentState == RecordingState::RECORDING) {
    //Updating recording
    //Lock callback
    SDL_LockAudioDevice(recordingDeviceId);

    if (gBufferBytePosition > gBufferByteMaxPosition) {
      //Stop recording audio
      SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);

      //Go on to next state
      gPromptTexture.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", d_textColor, d_font.get());
      currentState = RecordingState::RECORDED;
    }

    //Unlock callback
    SDL_UnlockAudioDevice(recordingDeviceId);
  } else if (currentState == RecordingState::PLAYBACK) {
    //Updating playback
    //Lock callback
    SDL_LockAudioDevice(playbackDeviceId);

    //Finished playback
    if (gBufferBytePosition > gBufferByteMaxPosition) {
      //Stop playing audio
      SDL_PauseAudioDevice(playbackDeviceId, SDL_TRUE);

      //Go on to next state
      gPromptTexture.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", d_textColor, d_font.get());
      currentState = RecordingState::RECORDED;
    }

    //Unlock callback
    SDL_UnlockAudioDevice(playbackDeviceId);
  }
}
