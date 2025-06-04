#include <iostream>
#include "AudioFile.h"
#include "echo_processor.h"
#include "Visuals.h"
using namespace std;

int main(int argc, char *argv[]){
    
    Visualizations::Visuals::DrawHeader();
    string File_Path = "";
    string File_Name = "";
    
    if (argc == 1){
        cout << "Enter File Path: \n";
        cin >> File_Path;

        cout << "\nEnter New File Name: \n";
        cin >> File_Name;
    }

    if (argc == 2){
        cout << "\nEnter New File Name: \n";
        cin >> File_Name;
    }

    system("clear");
    Visualizations::Visuals::DrawReading();
    audio::AudioFile audio;
    audio.read_wav_file(File_Path);
    
    system("clear");
    Visualizations::Visuals::DrawProcessing();

    int delay_samples = audio.wav.sample_rate * .3;
    float decay = 0.5;
    echo::EchoProcessor::processSIMD(audio, delay_samples, decay);

    system("clear");
    Visualizations::Visuals::DrawStoring();
    audio.store_wav_file(File_Name);

    system("clear");
    Visualizations::Visuals::DrawDone();

}

