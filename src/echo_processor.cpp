#include <cstddef>
#include <cstdint>
#include <vector>
#include "echo_processor.h"
#include "AudioFile.h"

float echo::EchoProcessor::calculateEchoCombinedSample(vector<int16_t> &audio_channel, int delay_samples, float decay, size_t idx){
        float echo_sample = decay * audio_channel[idx - delay_samples];

        float combined = audio_channel[idx] + echo_sample;

        // make sure it's in the valid range of int16_t 
        if (combined > 32767.0f)
            combined = 32767.0f;
        else if (combined < -32768.0f)
            combined = -32768.0f;   

        return combined;
}

