#include "PreampDSP.h"

void PreampDSP::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    rmsCoef = std::exp(-1.0 / (0.050 * sampleRate)); // 50ms RMS window
    inRmsL = 0.0;
    inRmsR = 0.0;
    outRmsL = 0.0;
    outRmsR = 0.0;
}

void PreampDSP::process(float* leftChannel, float* rightChannel, int numSamples, const Parameters& params)
{
    if (params.type == 0) return; // Bypass

    // hardware-style logarithmic drive mapping
    // 0-50% (character): 0dB to +12dB
    // 50-100% (distortion): +12dB to +36dB
    double driveDb = 0.0;
    if (params.drive <= 50.0) driveDb = (params.drive / 50.0) * 12.0;
    else driveDb = 12.0 + ((params.drive - 50.0) / 50.0) * 24.0;
    
    double driveScale = std::pow(10.0, driveDb / 20.0);
    double trimScale = std::pow(10.0, params.trim / 20.0);
    double invDriveSqrt = 1.0 / std::sqrt(std::max(0.001, driveScale));


    for (int i = 0; i < numSamples; ++i)
    {
        double spl0 = leftChannel[i];
        double spl1 = rightChannel[i];

        if (params.auto_level) {
            inRmsL = spl0 * spl0 + rmsCoef * (inRmsL - spl0 * spl0);
            inRmsR = spl1 * spl1 + rmsCoef * (inRmsR - spl1 * spl1);
        }

        spl0 *= driveScale;
        spl1 *= driveScale;

        if (params.type == 1) // Telefunken
        {
            auto tubeShaper = [&](double x) {
                double absX = std::abs(x);
                if (absX < 0.0001) return x;
                return x / (1.0 + absX * 0.5);
            };
            spl0 = tubeShaper(spl0);
            spl1 = tubeShaper(spl1);
        }
        else if (params.type == 2) // Neve
        {
            auto neveShaper = [&](double x) {
                return std::tanh(x + 0.05 * x * std::abs(x));
            };
            spl0 = neveShaper(spl0);
            spl1 = neveShaper(spl1);
        }
        else if (params.type == 3) // Modern
        {
            auto folder = [](double x) {
                return std::sin(std::max(-M_PI, std::min(x * 1.5, M_PI)) * 0.5);
            };
            spl0 = folder(spl0);
            spl1 = folder(spl1);
        }

        if (params.auto_level) {
            outRmsL = spl0 * spl0 + rmsCoef * (outRmsL - spl0 * spl0);
            outRmsR = spl1 * spl1 + rmsCoef * (outRmsR - spl1 * spl1);
            
            if (++counter >= 32) {
                double inRms = std::sqrt((inRmsL + inRmsR) * 0.5);
                double outRms = std::sqrt((outRmsL + outRmsR) * 0.5);
                cachedMakeup = (outRms > 0.00001) ? (inRms / outRms) : 1.0;
                cachedMakeup = std::max(0.1, std::min(cachedMakeup, 10.0));
                counter = 0;
            }
            
            spl0 *= cachedMakeup;
            spl1 *= cachedMakeup;
        } else {
            spl0 *= invDriveSqrt;
            spl1 *= invDriveSqrt;
        }

        spl0 *= trimScale;
        spl1 *= trimScale;

        leftChannel[i] = static_cast<float>(spl0);
        rightChannel[i] = static_cast<float>(spl1);
        
        double maxAbs = std::max(std::abs(spl0), std::abs(spl1));
        currentRMS = currentRMS * 0.999f + static_cast<float>(maxAbs) * 0.001f;
    }
}
