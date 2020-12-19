/*#pragma once

#include "LTexture.h"

#include <SDL2/SDL.h>

void close();
void loadMedia();

//AUDIO functions
void audioRecordingCallback( void* userdata, Uint8* stream, int len );
void audioPlaybackCallback( void* userdata, Uint8* stream, int len );

//AUDIO CONSTANTS
extern const int MAX_RECORDING_DEVICES ;
extern const int MAX_RECORDING_SECONDS;
extern const int RECORDING_BUGGER_SECONDS;

enum class RecordingState: unsigned int;

//AUDIO
extern LTexture gDeviceTextures[];
extern int gRecordingDeviceCount;
extern SDL_AudioSpec gReceivedRecordingSpec;
extern SDL_AudioSpec gReceivedPlaybackSpec;
extern Uint8* gRecordingBuffer;
extern Uint32 gBufferByteSize;
extern Uint32 gBufferBytePosition;
extern Uint32 gBufferByteMaxPosition;*/