/*
 * Company: Blackaddr Audio
 * Effect Name: Analog Delay
 * Description: A warm analog-based delay with multiple filters for different flavours. Boss DM3 inspired.
 *
 * This file was auto-generated by Aviate Audio Effect Creator for the Multiverse.
 */
#include <cmath>
#include "Aviate/EfxPrint.h"
#include "Aviate/LibBasicFunctions.h"
#include "AnalogDelay.h"

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
    case 1 : filter( (paramValue - 0.000000) / (2.000000 - 0.000000) ); break;
    case 2 : delay( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 3 : mix( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 4 : feedback( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 5 : volume( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    default : break;
    }
}

void AnalogDelay::processMidi(int channel, int control, int value)
{
    float val = (float)value / 127.0f;

    if ((m_midiConfig[Bypass_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Bypass_e][MIDI_CONTROL] == control)) {
        bypass(val);
        return;
    }

    if ((m_midiConfig[Filter_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Filter_e][MIDI_CONTROL] == control)) {
        filter(val);
        return;
    }

    if ((m_midiConfig[Delay_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Delay_e][MIDI_CONTROL] == control)) {
        delay(val);
        return;
    }

    if ((m_midiConfig[Mix_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Mix_e][MIDI_CONTROL] == control)) {
        mix(val);
        return;
    }

    if ((m_midiConfig[Feedback_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Feedback_e][MIDI_CONTROL] == control)) {
        feedback(val);
        return;
    }

    if ((m_midiConfig[Volume_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Volume_e][MIDI_CONTROL] == control)) {
        volume(val);
        return;
    }

}

audio_block_t* AnalogDelay::m_basicInputCheck(audio_block_t* inputAudioBlock, unsigned outputChannel)
{
    // Check if effect is disabled
    if (m_enable == false) {
        // do not transmit or process any audio, return as quickly as possible after releasing the inputs
        if (inputAudioBlock) { release(inputAudioBlock); }
        return nullptr; // disabled, no further EFX processing in update()
    }  // end of enable check

    // check if effect is in bypass
    if (m_bypass == true) {
        // drive input directly to the specified output. ie. bypass
        if (inputAudioBlock != nullptr) {
            // valid input, drive to outputChannel if specified
            if (outputChannel >= 0) {
                transmit(inputAudioBlock, outputChannel); // drive to specified output
            }
            release(inputAudioBlock); // release the input block as we are done with it
        } else { // invalid input block, allocate a block and drive silence if specified
            if (outputChannel >= 0) {
                audio_block_t* silenceBlock = allocate();
                if (silenceBlock) {
                    clearAudioBlock(silenceBlock);  // create silence in the buffer
                    transmit(silenceBlock, outputChannel);
                    release(silenceBlock);
                }
            }
        }
        return nullptr;  // bypassed, no further EFX processing in update()
    }  // end of bypass check

    // If not disabled or bypassed, create silence if the input block is invalid then
    // return the valid audio block so update() can continue.
    if (inputAudioBlock == nullptr) {
        inputAudioBlock = allocate();
        if (inputAudioBlock == nullptr) { return nullptr; } // check if allocate was unsuccessful
        // else
        clearAudioBlock(inputAudioBlock);
    }
    return inputAudioBlock; // inputAudioBLock is valid and ready for update() processing
}

}
