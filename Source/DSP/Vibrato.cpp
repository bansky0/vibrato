/*
  ==============================================================================

    Vibrato.cpp
    Created: 25 Oct 2024 10:43:52am
    Author:  Jhonatan

  ==============================================================================
*/

#include "Vibrato.h"

void VibratoFX::prepare(double theSampleRate)
{
    sampleRate = static_cast<float>(theSampleRate);
    t = 0.0f;
}

void VibratoFX::setDepth(float inDepth)
{
    // Profundidad del vibrato (modulación)
    depth = inDepth;
}

void VibratoFX::setRate(float inRate)
{
    // Frecuencia de la modulación (LFO)
    rate = inRate;
}

void VibratoFX::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            auto sample = buffer.getReadPointer(channel)[i];

            // Calcula el valor del LFO para determinar el retardo
            float lfoValue = depth / 2.0f * sin(2.0 * juce::MathConstants<float>::pi * rate * t) + depth;

            // Calcula el número de muestras de retardo basado en el LFO
            int delaySamples = static_cast<int>(std::fabs(lfoValue));

            // Calcula la posición de lectura del buffer circular
            int readerPointer = writterPointer[channel] - delaySamples;
            if (readerPointer < 0)
            {
                readerPointer += circularBufferSize;
            }

            // Obtiene la muestra retrasada del buffer circular
            auto delayedSample = circularBuffer[readerPointer][channel];

            // Escribe la muestra actual en el buffer circular
            circularBuffer[writterPointer[channel]][channel] = sample;

            // Avanza el puntero de escritura en el buffer circular
            writterPointer[channel]++;
            if (writterPointer[channel] >= circularBufferSize)
            {
                writterPointer[channel] = 0;
            }

            // Escribe la muestra retrasada en el buffer de salida
            buffer.getWritePointer(channel)[i] = delayedSample;

            // Incrementa el tiempo
            t += 1.0f / sampleRate;
        }
    }
}

VibratoFX::VibratoFX()
{
}

VibratoFX::~VibratoFX()
{
}