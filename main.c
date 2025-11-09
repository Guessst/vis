#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "miniaudio.h"
#include "raylib.h"
#include "miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_audio_buffer_ref *bufferRef = (ma_audio_buffer_ref *)pDevice->pUserData;
    ma_uint64 framesRead = ma_audio_buffer_ref_read_pcm_frames(bufferRef, pOutput, frameCount, false);
    (void)pInput;

    if (framesRead < frameCount)
    {
        // Optional: loop or stop
        ma_audio_buffer_ref_seek_to_pcm_frame(bufferRef, 0); // loop
    }
}

int main(void)
{
    const char *filename = "assets/Voxel Revolution.wav";
    ma_decoder decoder;
    ma_result result;

    // First, decode WAV to raw PCM
    result = ma_decoder_init_file(filename, NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        printf("Failed to init decoder\n");
        return -1;
    }

    ma_uint64 totalFrames;
    ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrames);

    size_t pcmDataSize = (size_t)(totalFrames * decoder.outputChannels * ma_get_bytes_per_sample(decoder.outputFormat));
    void *pcmData = malloc(pcmDataSize);
    if (pcmData == NULL)
        return -2;

    ma_uint64 framesRead;
    result = ma_decoder_read_pcm_frames(&decoder, pcmData, totalFrames, &framesRead);
    if (result != MA_SUCCESS)
        return -3;

    ma_decoder_uninit(&decoder); // we no longer need it

    // Now wrap the PCM data in a readable audio buffer
    ma_audio_buffer_ref bufferRef;
    ma_audio_buffer_ref_init(decoder.outputFormat, decoder.outputChannels, pcmData, totalFrames, &bufferRef);

    // Init playback device
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &bufferRef;

    ma_device device;
    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to init device\n");
        return -4;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to start device\n");
        ma_device_uninit(&device);
        return -5;
    }

    printf("Playing raw PCM data... Press Enter to quit.\n");
    getchar();

    ma_device_uninit(&device);
    free(pcmData);
    return 0;
}

/*
int main(void)
{
    // Initializations
    InitWindow(800, 600, "VIS");

    my_buffer = (unsigned char *)malloc(sizeof(unsigned char) * DESIRED_SIZE * 2);
    for (size_t i = 0; i < DESIRED_SIZE; ++i)
    {
        my_buffer[i] = 0;
    }

    ma_decoder decoder_1;
    const char *song = "assets/Voxel Revolution.wav";
    {
        ma_result result = ma_decoder_init_file(song, NULL, &decoder_1);
        if (result != MA_SUCCESS)
        {
            printf("Could not load file: %s\n", song);
            return -2;
        }
    }

    ma_device_config deviceConfig_1;
    deviceConfig_1 = ma_device_config_init(ma_device_type_playback);
    deviceConfig_1.playback.format = decoder_1.outputFormat;
    deviceConfig_1.playback.channels = decoder_1.outputChannels;
    deviceConfig_1.sampleRate = decoder_1.outputSampleRate;
    deviceConfig_1.dataCallback = data_callback;
    deviceConfig_1.pUserData = &decoder_1;

    ma_device device_1;
    if (ma_device_init(NULL, &deviceConfig_1, &device_1) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder_1);
        return -3;
    }

    if (ma_device_start(&device_1) != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device_1);
        ma_decoder_uninit(&decoder_1);
        return -5;
    }

    ma_decoder decoder_2;
    ma_decoder_config decoder_config_2 = ma_decoder_config_init(ma_format_s16, 2, 44100);
    decoder_config_2.encodingFormat = ma_encoding_format_wav;
    ma_device device_2;
    ma_device_config deviceConfig_2;

    // make new device

    while (!WindowShouldClose())
    {
        if (buffer_is_filled && !switched_buffers)
        {
            ma_result result;
            {
                result = ma_decoder_init_memory(my_buffer, DESIRED_SIZE, &decoder_config_2, &decoder_2);
                if (result != MA_SUCCESS)
                {
                    exit(-1);
                }
            }

            deviceConfig_2 = ma_device_config_init(ma_device_type_playback);
            deviceConfig_2.playback.format = decoder_2.outputFormat;
            deviceConfig_2.playback.channels = decoder_2.outputChannels;
            deviceConfig_2.sampleRate = decoder_2.outputSampleRate;
            deviceConfig_2.dataCallback = data_callback_2;
            deviceConfig_2.pUserData = &decoder_2;

            if (ma_device_init(NULL, &deviceConfig_2, &device_2) != MA_SUCCESS)
            {
                printf("Failed to open playback device.\n");
                exit(-1);
            }

            ma_device_uninit(&device_1);
            if (ma_device_start(&device_2) != MA_SUCCESS)
            {
                printf("Failed to start playback device.\n");
                exit(-1);
            }

            switched_buffers = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    // De-initializations
    CloseWindow();
    ma_device_uninit(&device_1);
    ma_decoder_uninit(&decoder_1);

    return 0;
}
*/