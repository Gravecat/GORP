// sam.cpp -- Modified version of SAM's main.c file, to integrate into GORP.
// Modified from SAM code by Raine "Gravecat" Simmons, 2023-2025.

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <vector>

extern "C" {
#include "reciter.h"
#include "sam.h"
}

#include "core/core.hpp"
#include "dictionary.hpp"
#include "sam.hpp"
#include "SFML/Audio.hpp"
#include "util/stringutils.hpp"

//#include <iostream>

// Approximations of some Windows functions to ease portability
#if defined __GNU_LIBRARY__ || defined __GLIBC__
static void strcat_s(char *dest, int size, const char *str) {
    unsigned int dlen = strlen(dest);
    if (static_cast<int>(dlen) >= size - 1) return;
    strncat(dest + dlen, str, size - dlen - 1);
}
void fopen_s(FILE **f, const char *filename, const char *mode) {
    *f = fopen(filename, mode);
}
#endif

namespace gorp {
namespace sam {

std::atomic<bool> shutting_down = false;
sf::SoundBuffer soundBuffer;
std::unique_ptr<sf::Sound> sound;   // Use a pointer since sf::Sound requires a valid buffer at construction
std::thread audioThread;
bool isPlaying = false;

double calculateRMS(const std::vector<int16_t>& samples, size_t start, size_t count)
{
    if (start >= samples.size()) return 0.0;

    // Ensure we don't go out of bounds
    size_t end = std::min(start + count, samples.size());

    // Calculate the sum of squares
    int64_t sumOfSquares = 0;
    for (size_t i = start; i < end; ++i)
        sumOfSquares += static_cast<int64_t>(samples[i]) * samples[i];

    // Calculate RMS
    size_t numSamples = end - start;
    if (numSamples == 0) return 0.0; // Avoid division by zero
    return std::sqrt(static_cast<double>(sumOfSquares) / numSamples);
}

void OutputSound()
{
    try
    {
        int bufferLength = GetBufferLength();
        char *buffer = GetBuffer();

        // Validate buffer and bufferLength
        if (buffer == nullptr || bufferLength <= 0) throw std::runtime_error("Invalid audio buffer or buffer length");

        // Clamp bufferLength to the maximum allocated size
        //bufferLength = std::min(bufferLength, 22050*10);

        // SFML requires the audio data to be in a specific format.
        // SAM outputs unsigned 8-bit PCM data, so we need to convert it to match SFML's expectations.
        std::vector<int16_t> samples(bufferLength, 0);
        for (int i = 0; i < bufferLength; ++i)
            samples[i] = static_cast<int16_t>((buffer[i] - 128) * 256); // Convert unsigned 8-bit to signed 16-bit

        // Load the samples into the sound buffer
        std::vector<sf::SoundChannel> channels = {sf::SoundChannel::Mono};
        if (!soundBuffer.loadFromSamples(samples.data(), static_cast<uint64_t>(samples.size()), 1, 22050, channels)) {
            free(GetBuffer());
            throw std::runtime_error("Failed to load sound buffer");
        }

        // Create the sound object and set the buffer
        sound = std::make_unique<sf::Sound>(soundBuffer);
        sound->setVolume(50.0f);
        sound->play();

        // Monitor playback and detect silence
        size_t currentSample = 0;
        const size_t blockSize = 1024; // Process samples in blocks of 1024
        while (sound->getStatus() == sf::Sound::Status::Playing)
        {

            double rms = calculateRMS(samples, currentSample, blockSize);
            // Visualize the volume level (optional)
            /*
            std::cout << "RMS: " << rms << " ";
            std::string out = std::string(static_cast<int>(rms / 1000), '*');
            std::cout << out << std::endl;
            */

            if (rms == 32768 || shutting_down)
            {
                sound->stop();
                break;
            }

            // Move to the next block
            currentSample += blockSize;

            // Sleep briefly to avoid busy-waiting
            sf::sleep(sf::milliseconds(50));
        }

        free(GetBuffer());
        isPlaying = false;
    }
    catch (std::exception &e)
    {
        gorp::core().log(e.what(), Core::CORE_CRITICAL);
        std::exit(EXIT_FAILURE);
    }
}

void sam_say(std::string phrase)
{
    if (audioThread.joinable()) {
        audioThread.join();
        isPlaying = false;
    }

    int phonetic = 0;
    unsigned char input[256];
    memset(input, 0, 256);

    std::vector<std::string> words = gorp::stringutils::string_explode(phrase, " ");
    while (!words.empty())
    {
        // Check if a given word is in the replacement dictionary.
        SAMDict::replace_string(words.at(0));

        if (words.at(0)[0] != '-')
        {
            strcat_s((char *)input, 256, words.at(0).c_str());
            strcat_s((char *)input, 256, " ");
        } else
        {
            if (strcmp(&words.at(0).c_str()[1], "sing") == 0) EnableSingmode();
            else if (strcmp(&words.at(0).c_str()[1], "phonetic") == 0) phonetic = 1;
            else if (strcmp(&words.at(0).c_str()[1], "pitch") == 0)
            {
                SetPitch((unsigned char)std::min(atoi(words.at(1).c_str()), 255));
                words.erase(words.begin());
            }
            else if (strcmp(&words.at(0).c_str()[1], "speed") == 0)
            {
                SetSpeed((unsigned char)std::min(atoi(words.at(1).c_str()), 255));
                words.erase(words.begin());
            }
            else if (strcmp(&words.at(0).c_str()[1], "mouth") == 0)
            {
                SetMouth((unsigned char)std::min(atoi(words.at(1).c_str()), 255));
                words.erase(words.begin());
            }
            else if (strcmp(&words.at(0).c_str()[1], "throat") == 0)
            {
                SetThroat((unsigned char)std::min(atoi(words.at(1).c_str()), 255));
                words.erase(words.begin());
            }
        }
        words.erase(words.begin());
    }

    for (int i = 0; input[i] != 0; i++)
        input[i] = (unsigned char)toupper((int)input[i]);

    if (!phonetic)
    {
        strcat_s((char *)input, 256, "[");
        if (!TextToPhonemes(input)) return;
    }
    else strcat_s((char *)input, 256, "\x9b");

    SetInput(input);
    SAMMain();

    isPlaying = true;
    audioThread = std::thread(OutputSound);
}

void cleanup()
{
    shutting_down = true;
    if (audioThread.joinable())
    {
        gorp::core().log("Shutting down SAM thread.");
        audioThread.join();
        gorp::core().log("SAM thread terminated.");
    }
}

} } // namespace gorp, sam
