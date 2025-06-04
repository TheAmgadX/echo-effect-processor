#include "AudioFile.h"
#include "echo_processor.h"

void echo::EchoProcessor::processNaive(audio::AudioFile &audio, int delay_samples, float decay){
    /*
        The Echo Effect is added using this equation:
            buffer[i] = buffer[i] + decay * buffer[i - delay_samples]
            

            => decay is the echo loud usually is 0.5 or 0.6
                --> 1.0 means it's the same loud as the actual sound (not realistic)
                --> 0.0 means there is no echo

            => delay_samples is the amount of delay of that echo
                --> the real echo is 100ms to 500ms 
                --> we can calculate it using this formula: sample_rate * delay_in_seconds
            
            
        => we will process right and left channels separately why?
            --> because in this part of the formula: 
                buffer[i - delay_samples] might get the data of right channel and the buffer[i] is targeted for left channel.
    */


    // left channel processing
    size_t size = audio.left_chan.size(); // the size for both right and left since in a valid .wav file the size of both must be equal.
    
    // using separate buffers to avoid overriding.
    vector<int16_t> left_temp(size);
    vector<int16_t> right_temp(size);
    
    for (size_t i = delay_samples; i < size; ++i) { 
        left_temp[i] = static_cast<int16_t>(echo::EchoProcessor::calculateEchoCombinedSample(
                    audio.left_chan, delay_samples, decay, i));
    }
    
    // right channel processing

    for (size_t i = delay_samples; i < size; ++i) { 
        right_temp[i] = static_cast<int16_t>(echo::EchoProcessor::calculateEchoCombinedSample(
                    audio.right_chan, delay_samples, decay, i));
    }

    audio.left_chan = left_temp;
    audio.right_chan = right_temp;

    // combine channels in the audio_data vector
    for(size_t i = 0, j = 0; i < size; i++, j += 2){
        // left before right:
        audio.audio_data[j] = audio.left_chan[i];
        audio.audio_data[j + 1] = audio.right_chan[i]; 
    }
}
