#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "miniaudio.h"
#include "raylib.h"

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *decoder = pDevice->pUserData;
    ma_result r = ma_decoder_read_pcm_frames(decoder, pOutput, frameCount, NULL);
    if (r != MA_SUCCESS)
    {
        exit(-1);
    }

    (void)pInput;
}

int main(void)
{
    const char *filename = "assets/Voxel Revolution.wav";

    ma_decoder decoder;
    ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 44100); // Example: float, stereo, 44.1kHz
    ma_result result = ma_decoder_init_file(filename, &decoderConfig, &decoder);
    if (result != MA_SUCCESS)
    {
        // Handle error
        exit(-1);
    }

    ma_uint64 totalFrames = 0;
    ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrames);

    // Allocate memory for the entire decoded audio
    ma_int16 *pcmData = (ma_int16 *)malloc(totalFrames * decoderConfig.channels * sizeof(ma_int16));
    if (pcmData == NULL)
    {
        // Handle memory allocation error
        exit(-1);
    }

    ma_uint64 framesRead = 0;
    result = ma_decoder_read_pcm_frames(&decoder, pcmData, totalFrames, &framesRead);
    if (result != MA_SUCCESS || framesRead != totalFrames)
    {
        // Handle error or incomplete read
        exit(-1);
    }
    ma_decoder_seek_to_pcm_frame(&decoder, 0);

    // pcmData now contains the entire decoded audio in memory

    /* Step 3: Init playback device */
    ma_device device;
    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to init device\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    /* Step 4: Start device */
    result = ma_device_start(&device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to start device\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    printf("Playing sound... Press Enter to quit.\n");
    getchar();

    /* Cleanup */
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

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