#include <stdio.h>
#include "miniaudio.h"
#include "raylib.h"

float ftime = 0;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    ftime += GetFrameTime();
    printf("%f %llu\n", ftime, pDecoder->readPointerInPCMFrames);

    (void)pInput;
}

int main(void)
{
    // Initializations
    InitWindow(800, 600, "VIS");

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    const char *song = "assets/Voxel Revolution.wav";
    result = ma_decoder_init_file(song, NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        printf("Could not load file: %s\n", song);
        return -2;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    // De-initializations
    CloseWindow();
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return 0;
}

/*
int main()
{
    // Initializations
    InitWindow(800, 600, "VIS");

    ma_engine engine;
    { // init ma_engine
        ma_result result;
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS)
        {
            printf("Failed to initialize audio engine.");
            return -1;
        }
    }

    const char *song = "assets/Voxel Revolution.wav";
    // Main loop
    ma_engine_play_sound(&engine, song, NULL);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    // De-initializations
    CloseWindow();
    ma_engine_uninit(&engine);

    return 0;
}
*/