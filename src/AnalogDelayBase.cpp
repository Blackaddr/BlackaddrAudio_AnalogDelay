/*
 * Company: Blackaddr Audio
 * Effect Name: Analog Delay
 * Description: A warm analog-based delay with multiple filters for different flavours. Boss DM3 inspired. Delay up to 3 seconds.
 *
 * This file was auto-generated by Aviate Audio Effect Creator for the Multiverse.
 */
#include <cmath>
#include "Aviate/LibBasicFunctions.h"
#include "AnalogDelay.h"

#define audioBlockReceiveReadOnly receiveReadOnly
#define audioBlockReceiveWritable receiveWritable
#define audioBlockAllocate        allocate

using namespace Aviate;

namespace BlackaddrAudio_AnalogDelay {

void AnalogDelay::mapMidiControl(int parameter, int midiCC, int midiChannel)
{
    if (parameter >= NUM_CONTROLS) {
        return ; // Invalid midi parameter
    }
    m_midiConfig[parameter][MIDI_CHANNEL] = midiChannel;
    m_midiConfig[parameter][MIDI_CONTROL] = midiCC;
}

void AnalogDelay::setParam(int paramIndex, float paramValue)
{
    switch(paramIndex) {
    case 0 : bypass( (paramValue - 0.000000) / (1.000000 - 0.000000) ); break;
    case 1 : delay( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 2 : filter( (paramValue - 0.000000) / (2.000000 - 0.000000) ); break;
    case 3 : mix( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 4 : feedback( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 5 : volume( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 6 : longdelay( (paramValue - 0.000000) / (1.000000 - 0.000000) ); break;
    default : break;
    }
}

float AnalogDelay::getUserParamValue(int paramIndex, float normalizedParamValue)
{
    switch(paramIndex) {
    case 0 : return ( ((1.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // bypass
    case 1 : return ( ((10.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // delay
    case 2 : return ( ((2.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // filter
    case 3 : return ( ((10.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // mix
    case 4 : return ( ((10.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // feedback
    case 5 : return ( ((10.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // volume
    case 6 : return ( ((1.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // longdelay
    default : return 0.0f;
    }
}

void AnalogDelay::processMidi(int status, int data1, int data2)
{
}

audio_block_t* AnalogDelay::m_basicInputCheck(audio_block_t* inputAudioBlock, unsigned outputChannel)
{
    // Check if effect is disabled
    if (m_enable == false) {
        // do not transmit or process any audio, return as quickly as possible after releasing the inputs
        if (inputAudioBlock) { AudioStream::release(inputAudioBlock); }
        return nullptr; // disabled, no further EFX processing in update()
    }  // end of enable check

    // check if effect is in bypass
    if (m_bypass == true) {
        // drive input directly to the specified output. ie. bypass
        if (inputAudioBlock != nullptr) {
            // valid input, drive to outputChannel if specified
            if (outputChannel >= 0) {
                AudioStream::transmit(inputAudioBlock, outputChannel); // drive to specified output
            }
            AudioStream::release(inputAudioBlock); // release the input block as we are done with it
        } else { // invalid input block, allocate a block and drive silence if specified
            if (outputChannel >= 0) {
                audio_block_t* silenceBlock = AudioStream::allocate();
                if (silenceBlock) {
                    clearAudioBlock(silenceBlock);  // create silence in the buffer
                    AudioStream::transmit(silenceBlock, outputChannel);
                    AudioStream::release(silenceBlock);
                }
            }
        }
        return nullptr;  // bypassed, no further EFX processing in update()
    }  // end of bypass check

    // If not disabled or bypassed, create silence if the input block is invalid then
    // return the valid audio block so update() can continue.
    if (inputAudioBlock == nullptr) {
        inputAudioBlock = AudioStream::allocate();
        if (inputAudioBlock == nullptr) { return nullptr; } // check if allocate was unsuccessful
        // else
        clearAudioBlock(inputAudioBlock);
    }
    return inputAudioBlock; // inputAudioBLock is valid and ready for update() processing
}

const uint8_t rblk[256] = TEENSY_AUDIO_BLOCK;
const uint8_t* AnalogDelay::getRblk() { return rblk; }
static constexpr char PROGMEM AnalogDelay_name[] = {0x42, 0x6c, 0x61, 0x63, 0x6b, 0x61, 0x64, 0x64, 0x72, 0x20, 0x41, 0x75, 0x64, 0x69, 0x6f, 0x3a, 0x41, 0x6e, 0x61, 0x6c, 0x6f, 0x67, 0x20, 0x44, 0x65, 0x6c, 0x61, 0x79, 0x0};
const char* AnalogDelay::getName() { return AnalogDelay_name; }

}
