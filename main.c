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
