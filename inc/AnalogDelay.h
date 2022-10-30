/*
 * Company: Blackaddr Audio
 * Effect Name: Analog Delay
 * Description: A warm analog-based delay with multiple filters for different flavours. Boss DM3 inspired.
 *
 * This file was auto-generated by Aviate Audio Effect Creator for the Multiverse.
 */
#pragma once

#include <Audio.h>
#include <arm_math.h>
#include "Aviate/AudioEffectWrapper.h"

//!s - START_USER_INCLUDES - put your #includes below this line before the matching END
#include "Aviate/SramManager.h"
//!e - END_USER_INCLUDES

namespace BlackaddrAudio_AnalogDelay {

//!s - START_USER_EFFECT_TYPES - put your effect types below this line before the matching END
//!e - END_USER_EFFECT_TYPES

class AnalogDelay : public AudioStream, public Aviate::AudioEffectWrapper {
public:
    static constexpr unsigned NUM_INPUTS  = 1;
    static constexpr unsigned NUM_OUTPUTS = 1;

    // List of effect control names
    enum {
        Bypass_e = 0,
        Delay_e = 1,
        Filter_e = 2,
        Mix_e = 3,
        Feedback_e = 4,
        Volume_e = 5,
        NUM_CONTROLS
    };

    //!s - START_USER_CLASS_TYPES - put your custom class types below this line before the matching END
    enum class Filter {
        DM3 = 0,
        WARM,
        DARK,
        NUM_FILTERS
    };
    //!e - END_USER_CLASS_TYPES

    AnalogDelay();

    //!s - START_USER_CONSTRUCTORS - put your custom constructors below this line before the matching END
    /// Construct an analog delay using internal memory by specifying the maximum
    /// delay in milliseconds.
    /// @param maxDelayMs maximum delay in milliseconds. Larger delays use more memory.
    AnalogDelay(float maxDelayMs, bool useExtMem = false);

    /// Construct an analog delay using external SPI via an ExtMemSlot. The amount of
    /// delay will be determined by the amount of memory in the slot.
    /// @param slot A pointer to the ExtMemSlot to use for the delay.
    //AnalogDelay(float maxDelayMs); // requires sufficiently sized pre-allocated memory
    //!e - END_USER_CONSTRUCTORS

    virtual ~AnalogDelay();

    // Standard EFX interface functions - do not change these declaration
    virtual void update(); // main audio processing loop function
    void mapMidiControl(int parameter, int midiCC, int midiChannel = 0) override;
    void processMidi(int channel, int midiCC, int value) override;
    void setParam(int paramIndex, float paramValue) override;
    const char* getName() override;
    const uint8_t* getRblk() override;

    // control value set functions, must take floats between 0.0f and 1.0f - do not change these declarations
    void delay(float value);
    void filter(float value);
    void mix(float value);
    void feedback(float value);
    void volume(float value) override;

    //!s - START_USER_PUBLIC_MEMBERS - put your public members below this line before the matching END

    /// Set the delay in milliseconds.
    /// @param milliseconds the request delay in milliseconds. Must be less than max delay.
    void delayMs(float milliseconds);

    /// Set the delay in number of audio samples.
    /// @param delaySamples the request delay in audio samples. Must be less than max delay.
    void delaySamples(size_t numDelaySamples);

    /// Set the delay as a fraction of the maximum delay.
    /// The value should be between 0.0f and 1.0f
    void delayFractionMax(float delayFraction);

    /// Set the filter coefficients to one of the presets. See AudioEffectAnalogDelay::Filter
    /// for options.
    /// @details See AudioEffectAnalogDelayFIlters.h for more details.
    /// @param filter the preset filter. E.g. AudioEffectAnalogDelay::Filter::WARM
    void setFilter(Filter filter);

    /// Override the default coefficients with your own. The number of filters stages affects how
    /// much CPU is consumed.
    /// @details The effect uses the CMSIS-DSP library for biquads which requires coefficents.
    /// be in q31 format, which means they are 32-bit signed integers representing -1.0 to slightly
    /// less than +1.0. The coeffShift parameter effectively multiplies the coefficients by 2^shift. <br>
    /// Example: If you really want +1.5, must instead use +0.75 * 2^1, thus 0.75 in q31 format is
    /// (0.75 * 2^31) = 1610612736 and coeffShift = 1.
    /// @param numStages the actual number of filter stages you want to use. Must be <= MAX_NUM_FILTER_STAGES.
    /// @param coeffs pointer to an integer array of coefficients in q31 format.
    /// @param coeffShift Coefficient scaling factor = 2^coeffShift.
    void setFilterCoeffs(int numStages, const int32_t *coeffs, int coeffShift);

    //!e - END_USER_PUBLIC_MEMBERS

private:
    audio_block_t *m_inputQueueArray[1]; // required by AudioStream base class, array size is num inputs
    int m_midiConfig[NUM_CONTROLS][2]; // stores the midi parameter mapping

    // m_bypass and m_volume are already provided by the base class AudioEffectWrapper
    float m_delay = 0.0f;
    float m_filter = 0.0f;
    float m_mix = 0.0f;
    float m_feedback = 0.0f;

    audio_block_t* m_basicInputCheck(audio_block_t* inputAudioBlock, unsigned outputChannel);

    //!s - START_USER_PRIVATE_MEMBERS - put your private members below this line before the matching END
    bool m_extMemConfigured = false;
    Aviate::SramMemSlot* m_slot = nullptr;
    //bool m_slotCleared = false;
    //static constexpr float FEEDBACK_LIMIT_F = 1.0f;
    bool                m_useExternalMemory  = false;
    Aviate::AudioDelay *m_memory             = nullptr;
    size_t              m_maxDelaySamples    = 0;

    audio_block_t             *m_previousBlock  = nullptr;
    audio_block_t             *m_blockToRelease = nullptr;
    Aviate::IirBiQuadFilterHQ *m_iir            = nullptr;

    size_t m_delaySamples = 0;

    void m_configExtMem();
    void m_preProcessing(audio_block_t *out, audio_block_t *dry, audio_block_t *wet);
    void m_postProcessing(audio_block_t *out, audio_block_t *dry, audio_block_t *wet);

    // Coefficients
    void m_constructFilter(void);

    //!e - END_USER_PRIVATE_MEMBERS

};

}
