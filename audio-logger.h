/*! \file audio-logger.h
 *  \brief Audio capture helper class
 *
 *  Used by all kbd-audio tools.
 *  It provides a stream of captured audio via a provided callback.
 *
 *  \author Georgi Gerganov
 */

#pragma once

#include "constants.h"
#include "common.h"

#include <memory>
#include <array>
#include <vector>
#include <functional>

class AudioLogger {
    public:
        using Sample = TSampleF;

        using Frame = std::array<Sample, kSamplesPerFrame>;
        using Record = std::vector<Frame>;
        using Callback = std::function<void(const Record & frames)>;

        struct Parameters {
            Callback callback;

            int32_t captureId = -1;
            int32_t nChannels = -1;

            int64_t sampleRate = -1;

            // Sample Type

            // todo : support for other sample types
            enum ESampleType {
                F32SYS,
            };

            ESampleType sampleType = F32SYS;

            // Audio Filter

            EAudioFilter filter = FirstOrderHighPass;

            float freqCutoff_Hz = 1000.0f;
        };

        AudioLogger();
        ~AudioLogger();

        bool install(Parameters && parameters);
        bool terminate();
        bool addFrame(const Sample * stream);
        bool record(float bufferSize_s, int32_t nPrevFrames);

        bool pause();
        bool resume();

        bool isValidBufferSize(float bufferSize_s) const;

    private:
        struct Data;
        std::unique_ptr<Data> data_;
        Data & getData() { return *data_; }
        const Data & getData() const { return *data_; }
};
