#include <xmmintrin.h>  // SSE
#include <emmintrin.h>  // SSE2
#include <algorithm>
#include <vector>
#include <cstdint>
#include <iostream>

#include "AudioFile.h"
#include "echo_processor.h"

void processChannelSSE(const std::vector<int16_t>& channel, std::vector<int16_t>& result_channel, int delay_samples, float decay) {
    // if we have enough samples to apply the echo effect
    if (channel.size() <= static_cast<size_t>(delay_samples)) {
        result_channel = channel;
        return;
    }
    
    result_channel.resize(channel.size());
    
    // we can't apply eacho effect to the initial parts so just copying them.
    for (int i = 0; i < delay_samples; ++i) {
        result_channel[i] = channel[i];
    }
    
    // decay_vector = [decay, decay, decay, decay] 4 float values
    __m128 decay_vector = _mm_set1_ps(decay);
    
    // create SIMD vectors for the minimum and maximum values of int16_t.
    __m128 max_val = _mm_set1_ps(32767.0f);
    __m128 min_val = _mm_set1_ps(-32768.0f);
    
    // process 8 int16_t samples (128 bits) in each iteration.
    size_t i = delay_samples;
    for (; i + 7 < channel.size(); i += 8) {
        // Load 8 int16_t samples (128 bits) from the current position.
        // Use unaligned load since we does not guarantee alignment.
        __m128i current_16 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&channel[i])); 
        // _mm_loadu_si128 => load unaligned signed integer 128
        // __m128i used for integer operations => 8 int16_t 


        // Load 8 int16_t samples from the delayed position.
        __m128i delayed_16 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&channel[i - delay_samples]));

        // ___ Convert loaded int16_t samples to float for processing ___
        
        /*
            *SSE2 doesn't have direct int16_t to float conversion. We go via int32_t.
            * so we convert from int16_t to int32_t 
            * we need two __m128 registers (4 floats each) to hold the 8 results.
        */
        
        /*
            * _mm_unpacklo_epi16 => unpack low packed 16 bit integers.
            * if we have A = [a0, a1, a2, a3, a4, a5, a6, a7] and B = [b0, b1, b2, b3, b4, b5, b6, b7]
            * it shuffle them like that:
            * _mm_unpacklo_epi16(A, B) = [a0, b0, a1, b1, a2, b2, a3, b3]
            * when we use the same (current_16) in both inputes it deplicate the first 4 effectively 
            * [a, a, b, b, c, c, d, d]
            

            * _mm_srai_epi32 => shift right arithmetic packed 32-bit integers. (sign extend)
            * now the current_lower_32 holds [a, b, c, d] in int32_t
        */
        __m128i current_lower_32 = _mm_srai_epi32(_mm_unpacklo_epi16(current_16, current_16), 16);

        // Unpack the upper 4 int16_t values into 4 int32_t values and sign-extend.
        // _mm_unpackhi_epi16 => unpack high packed 16 bit integers.
        __m128i current_upper_32 = _mm_srai_epi32(_mm_unpackhi_epi16(current_16, current_16), 16);

        __m128i delayed_lower_32 = _mm_srai_epi32(_mm_unpacklo_epi16(delayed_16, delayed_16), 16);
        __m128i delayed_upper_32 = _mm_srai_epi32(_mm_unpackhi_epi16(delayed_16, delayed_16), 16);

        /*
            * now onvert 4x int32_t => 4x float 
            * _mm_cvtepi32_ps => convert packed int32_t to packed single precision float 
        */
        __m128 current_lower_float = _mm_cvtepi32_ps(current_lower_32);
        __m128 current_upper_float = _mm_cvtepi32_ps(current_upper_32);
        __m128 delayed_lower_float = _mm_cvtepi32_ps(delayed_lower_32);
        __m128 delayed_upper_float = _mm_cvtepi32_ps(delayed_upper_32);

        // ___ Apply the echo effect ___
        /*
            * the equation simply is that: 
                buffer[i] = buffer[i] + decay * buffer[i - delay_samples]
        */
        
        /*
            delay samples * decay
            [a, b, c, d] * [decay, decay, decay, decay].
            _mm_mul_ps => multiply packed single precision float
        */ 

        __m128 echo_lower = _mm_mul_ps(delayed_lower_float, decay_vector);
        __m128 echo_upper = _mm_mul_ps(delayed_upper_float, decay_vector);

        // current + echo
        // [a, b, c, d] + [echo, echo, echo, echo].
        __m128 result_lower = _mm_add_ps(current_lower_float, echo_lower);
        __m128 result_upper = _mm_add_ps(current_upper_float, echo_upper);

        // ___ Clamp and convert result back to int16_t ___
        /*
            ensure the floating-point result in the valid range for int16_t.
            _mm_max_ps => max packed single precision float.     
        */
        
        // ensure values are not less than the minimum.
        result_lower = _mm_max_ps(result_lower, min_val);
        result_upper = _mm_max_ps(result_upper, min_val);

        // ensure values are not more than the maximum.
        result_lower = _mm_min_ps(result_lower, max_val);
        result_upper = _mm_min_ps(result_upper, max_val);

        /*
            * convert the clamped 4 floats back to 4 int32_ts
            *_mm_cvtps_epi32 => convert packed single precision float to packed int32_t 
        */
        __m128i result_lower_32 = _mm_cvtps_epi32(result_lower);
        __m128i result_upper_32 = _mm_cvtps_epi32(result_upper);
        

        /*
            * Pack the two 4x int32_t results back into one 8x int16_t vector.
            * _mm_packs_epi32 => pack int32_t to packed int16_t using sign saturation
            * first convert from int32_t to int16_t
            * then compine the both results into one vector 
        */
        __m128i result_16 = _mm_packs_epi32(result_lower_32, result_upper_32);

        /* 
            * Store the 8 processed int16_t results (128 bits) back in the result channel.
            * _mm_storeu_si128 => store unaligned signed integer 128        
        */
        _mm_storeu_si128(reinterpret_cast<__m128i*>(&result_channel[i]), result_16);
    }

    // Process any remaining samples (less than 8)
    for (; i < channel.size(); ++i) {
        float current = static_cast<float>(channel[i]);
        float delayed = static_cast<float>(channel[i - delay_samples]);
        float echo = delayed * decay;
        float result = current + echo;
        
        if (result > 32767.0f) result = 32767.0f;
        else if (result < -32768.0f) result = -32768.0f;
        
        result_channel[i] = static_cast<int16_t>(result);
    }
}

void echo::EchoProcessor::processSIMD(audio::AudioFile &audio, int delay_samples, float decay) {

    // Make sure audio_data is properly sized before processing
    if (audio.audio_data.empty()) {
        cerr << "audio file is empty!\n";
        return;
    }
    
    if (audio.left_chan.empty() || audio.right_chan.empty()) {
        cerr << "stereo channels is not valid!\n";
        return;
    }

    std::vector<int16_t> left_result;
    std::vector<int16_t> right_result;
    
    processChannelSSE(audio.left_chan, left_result, delay_samples, decay);
    processChannelSSE(audio.right_chan, right_result, delay_samples, decay);
    
    // move the ownership of the results to be for left / right channels.
    // avoid copying overhead.
    audio.left_chan = std::move(left_result);
    audio.right_chan = std::move(right_result);
    
    // Combine the processed channels back into the main audio_data vector
    size_t channel_size = audio.right_chan.size();

    for (size_t i = 0, j = 0; i < channel_size; ++i, j += 2) {
        audio.audio_data[j] = audio.left_chan[i];
        audio.audio_data[j + 1] = audio.right_chan[i]; 
    }
}

