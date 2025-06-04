#pragma once
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <sys/types.h>
#include <vector>

using namespace std;

namespace audio {
class AudioFile {

public:
    // audio data
    vector<int16_t> audio_data;
    vector<int16_t> right_chan;
    vector<int16_t> left_chan;

    
    struct WAVFileHeader {
        // following this docs: http://soundfile.sapp.org/doc/WaveFormat/
        // all variables should be just 4 bytes, I noted what requires 2 bytes.

        // RIFF Chunck
        char chunk_id[4];
        uint32_t chunk_size;
        char format[4];

        // FMT - sub-chunk
        char sub_chunk1_id[4];
        uint32_t sub_chunk1_size; // 16 For PCM
        uint16_t audio_format;    // 1 for PCM it's 2 bytes so it needs handling
        uint16_t num_channels; // stereo it's 2 bytes so it needs handling
        uint32_t sample_rate; // usually  44100
        uint32_t byte_rate;   // = sample_rate * num_of_channels * bits_per_sample / 8;
        uint16_t block_align; // num_of_channels * bits_per_sample / 8; -->  2 bytes
        uint16_t bits_per_sample; // 2 bytes

        // DATA - sub-chunk
        char sub_chunk2_id[4]; // usually "data"
        uint32_t sub_chunk2_size ; // num_samples * num_of_channels * (bits_per_sample / 8); it's just 4 bytes
    }__attribute__((packed)); // total size 44 bytes
    /*
        the __attribute__((packed)) is for avoiding the compiler padding fot the struct.
    
        --> what is Compiler padding?
            --> the compiler add padding bytes inside the struct to align fields for faster memory access
                --> but since we are handling binary file format that will cause a problem 
                since it must match exactly the file binary structure or it will not work.
    */


    WAVFileHeader wav;
    
    void read_wav_file(string fname);
    void store_wav_file(string fname);
private:
    bool write_as_bytes(ofstream &file, const int val, const int byte_size);
    bool write_as_bytes(ofstream &file, const char val[4], const int byte_size);

    void read_wav_file_header(ifstream &wavFile, string fname);
};

} // namespace AudioFile
