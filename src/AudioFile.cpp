#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "AudioFile.h"
using namespace std;

void audio::AudioFile::read_wav_file_header(ifstream &wavFile, string fname){
    if (!wavFile.is_open()){
        cerr << "failed to open file: " << fname << "\n";
        return;
    }

    wavFile.seekg(0, ios::end);
    int length = wavFile.tellg();
    wavFile.seekg(0, ios::beg);
    cout << "File size: " << length << " bytes\n";


    // read the wav header.
    if (!wavFile.read(reinterpret_cast<char *>(&wav), sizeof(WAVFileHeader))){
        wavFile.close();
        cerr << "Failed to read WAV header.\n";
        return;
    }
    
    // check if the file is a valid WAV file.
    if (string(wav.format, 4) != "WAVE" || string(wav.chunk_id, 4) != "RIFF") {
        wavFile.close();
        cerr << "Invalid WAVE or RIFF\n";
        return;
    }

    if (wav.num_channels != 2 || wav.bits_per_sample != 16 || wav.audio_format != 1){
        // audio format = 1 for PCM
        // bits per sample = 16 as usual
        // num channels = 2 for stereo
        wavFile.close();

        cerr << "Unsupported WAV format (bit depth : 16, audio format: 1, num channels: 2)\n";
        return;
    }
}

bool audio::AudioFile::write_as_bytes(ofstream &file, const int val, const int bytes_size){
    file.write(reinterpret_cast<const char *>(&val), bytes_size); 
    return file.fail();
}

bool audio::AudioFile::write_as_bytes(ofstream &file, const char val[4], const int bytes_size){
    file.write(val, bytes_size);
    return file.fail();
}

void audio::AudioFile::read_wav_file(string fname) {
    ifstream wavFile(fname, ios::binary); // open the wav header.             

    read_wav_file_header(wavFile, fname);

    

    // reading the data part. 
    size_t num_samples = wav.sub_chunk2_size / sizeof(uint16_t);
    audio_data = vector<int16_t>(num_samples);

    if (!wavFile.read(reinterpret_cast<char*>(audio_data.data()), wav.sub_chunk2_size)){
        cerr << "Failed to read audio data\n";
        return;
    } 
    
    wavFile.close();

    // making the audio in two diementions.
    left_chan.clear();
    right_chan.clear();

    for (int i = 0; i < audio_data.size(); i += 2) {
        left_chan.push_back(audio_data[i]);
        right_chan.push_back(audio_data[i + 1]);
    }
}

void audio::AudioFile::store_wav_file(string fname) {
    if (audio_data.empty()) {
        cerr << "Audio is empty!\n";
        return;
    }

    ofstream file;

    file.open(fname, ios::binary);

    if (!file.is_open()) {
        file.close();
        cerr << "Failed to open file: " << fname << "\n";
        return;
    }

    // fail function that handles failing in writing
    auto fail = [&](bool fail) {
        if (fail){
            cerr << "Failed to write in file: " << fname << "\n";
            file.close();
            return true; // stop the store function
        }
        return false; // continue writing
    };

    // RIFF chunk
    if (fail(write_as_bytes(file, wav.chunk_id, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.chunk_size, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.format, 4))) {
        file.close();
        return;
    }

    // FMT - sub chunk
    if (fail(write_as_bytes(file, wav.sub_chunk1_id, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.sub_chunk1_size, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.audio_format, 2))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.num_channels, 2))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.sample_rate, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.byte_rate, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.block_align, 2))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.bits_per_sample, 2))) {
        file.close();
        return;
    }

    // DATA - sub chunk
    if (fail(write_as_bytes(file, wav.sub_chunk2_id, 4))) {
        file.close();
        return;
    }
    if (fail(write_as_bytes(file, wav.sub_chunk2_size, 4))) {
        file.close();
        return;
    }

    // Write audio sample data
    file.write(reinterpret_cast<const char*>(audio_data.data()), audio_data.size() * sizeof(uint16_t));

    if(fail(file.fail())) {
        file.close();
        return;
    }


    file.close();
}
