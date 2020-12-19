/*
#include "Commons.h"

#include <sstream>

//AUDIO CONSTANTS
extern const int MAX_RECORDING_DEVICES = 10;
extern const int MAX_RECORDING_SECONDS = 5;
extern const int RECORDING_BUGGER_SECONDS = MAX_RECORDING_SECONDS + 1;

enum class RecordingState : unsigned int
{
	SELECTING_DEVICE,
	STOPPED,
	RECORDING,
	RECORDED,
	PLAYBACK,
	ERROR
};

//AUDIO
LTexture gDeviceTextures[MAX_RECORDING_DEVICES];
int gRecordingDeviceCount = 0;
SDL_AudioSpec gReceivedRecordingSpec;
SDL_AudioSpec gReceivedPlaybackSpec;
Uint8 *gRecordingBuffer = NULL;
Uint32 gBufferByteSize = 0;
Uint32 gBufferBytePosition = 0;
Uint32 gBufferByteMaxPosition = 0;

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("../assets/fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render the prompt
		if (!gPromptTextTexture.loadFromRenderedText("Select your recording device", textColor))
		{
			printf("Failed to render prompt text!\n");
			success = false;
		}
		else
		{
			gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
			if (gRecordingDeviceCount < 1)
			{
				printf("Unable to get audio capture device! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				if (gRecordingDeviceCount > MAX_RECORDING_DEVICES)
				{
					gRecordingDeviceCount = MAX_RECORDING_DEVICES;
				}

				std::stringstream promptText;
				for (int i = 0; i < gRecordingDeviceCount; i++)
				{
					promptText.str("");
					promptText << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);
					//TODO:if error check
					gDeviceTextures[i].loadFromRenderedText(promptText.str().c_str(), textColor);
				}
			}
		}
		return success;
	}
}

void close()
{
	//Open data for writing
	SDL_RWops *file = SDL_RWFromFile("33_file_reading_and_writing/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}

	//Free loaded images
	gPromptTextTexture.free();
	gInputTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;


}
*/
