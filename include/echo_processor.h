#pragma once

#include "AudioFile.h"
#include <cstdint>
using namespace std;

namespace echo {

class EchoProcessor {
public:
  static void processNaive(audio::AudioFile &audio, int delay_samples,
                            float decay);

  static void processSIMD(audio::AudioFile &audio, int delay_samples,
                          float decay);

private:
  static float calculateEchoCombinedSample(vector<int16_t> &audio_channel, int delay_samples,  float decay, size_t idx);
};
} // namespace echo

