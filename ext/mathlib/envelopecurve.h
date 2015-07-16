#ifndef N_ENVELOPE_CURVE_H
#define N_ENVELOPE_CURVE_H
//------------------------------------------------------------------------------
/**
    @class nEnvelopeCurve
    @ingroup Math

    An envelope curve class.

    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/nmath.h"

//------------------------------------------------------------------------------
//  nEnvelopeVurve
//------------------------------------------------------------------------------
class nEnvelopeCurve
{
public:
    /// possible modulation functions
    enum 
    {
        Sine = 0,
        Cosine,
    };

    /// constructor 1
    nEnvelopeCurve();
    /// constructor 2
    nEnvelopeCurve(const float keyFrameValue0, const float keyFrameValue1,
        const float keyFrameValue2, const float keyFrameValue3,
        const float keyFramePos1, const float keyFramePos2,
        const float frequency, const float amplitude,
        const int modulationFunc);
    // set all parameters
    void SetParameters(const float keyFrameValue0, const float keyFrameValue1,
        const float keyFrameValue2, const float keyFrameValue3,
        const float keyFramePos1, const float keyFramePos2,
        const float frequency, const float amplitude,
        const int modulationFunc);
    // assign to other envelope curve
    void SetParameters(const nEnvelopeCurve& src);
    /// get the function value; pos must be between 0 and 1
    float GetValue(float pos) const;
    /// get the highest possible value
    float GetMaxPossibleValue() const;

    enum
    {
        NumValues = 4,
    };
    float keyFrameValues[NumValues];
    float keyFramePos1, keyFramePos2;   // 0 through 1
    float frequency, amplitude;         // parameters of the sinus function
    int modulationFunc;      // use sine or cosine for modulation?
};

//------------------------------------------------------------------------------
/**
*/
inline
nEnvelopeCurve::nEnvelopeCurve() :
    keyFramePos1(.2f),
    keyFramePos2(.8f),
    frequency(.0f),
    amplitude(.0f),
    modulationFunc(Sine)
{
    keyFrameValues[0] = .0f;
    keyFrameValues[1] = .0f;
    keyFrameValues[2] = .0f;
    keyFrameValues[3] = .0f;
}

//------------------------------------------------------------------------------
/**
*/
inline
nEnvelopeCurve::nEnvelopeCurve(const float keyFrameValue0, 
        const float keyFrameValue1, const float keyFrameValue2,
        const float keyFrameValue3, const float keyFramePos1,
        const float keyFramePos2, const float frequency,
        const float amplitude, const int modulationFunc/* = Sine*/) :
    keyFramePos1(keyFramePos1),
    keyFramePos2(keyFramePos2),
    frequency(frequency),
    amplitude(amplitude),
    modulationFunc(modulationFunc)
{
    this->keyFrameValues[0] = keyFrameValue0;
    this->keyFrameValues[1] = keyFrameValue1;
    this->keyFrameValues[2] = keyFrameValue2;
    this->keyFrameValues[3] = keyFrameValue3;
}

//------------------------------------------------------------------------------
/**
*/
inline
void nEnvelopeCurve::SetParameters(const float keyFrameValue0, const float keyFrameValue1,
    const float keyFrameValue2, const float keyFrameValue3,
    const float keyFramePos1, const float keyFramePos2,
    const float frequency, const float amplitude,
    const int modulationFunc/* = Sine*/)
{
    this->keyFrameValues[0] = keyFrameValue0;
    this->keyFrameValues[1] = keyFrameValue1;
    this->keyFrameValues[2] = keyFrameValue2;
    this->keyFrameValues[3] = keyFrameValue3;
    this->keyFramePos1 = keyFramePos1;
    this->keyFramePos2 = keyFramePos2;
    this->frequency = frequency;
    this->amplitude = amplitude;
    this->modulationFunc = modulationFunc;
}
//------------------------------------------------------------------------------
/**
*/
inline
void nEnvelopeCurve::SetParameters(const nEnvelopeCurve& src)
{
    this->keyFrameValues[0] = src.keyFrameValues[0];
    this->keyFrameValues[1] = src.keyFrameValues[1];
    this->keyFrameValues[2] = src.keyFrameValues[2];
    this->keyFrameValues[3] = src.keyFrameValues[3];
    this->keyFramePos1 = src.keyFramePos1;
    this->keyFramePos2 = src.keyFramePos2;
    this->frequency = src.frequency;
    this->amplitude = src.amplitude;
    this->modulationFunc = src.modulationFunc;
}

//------------------------------------------------------------------------------
/**
*/
inline
float nEnvelopeCurve::GetValue(float pos) const
{   
    pos = n_saturate(pos);

    float value;
    if (pos < this->keyFramePos1)
    {
        value = this->keyFrameValues[0] + 
            (this->keyFrameValues[1] - this->keyFrameValues[0]) * 
            (pos / this->keyFramePos1);
    }
    else if (pos < this->keyFramePos2)
    {
        value = this->keyFrameValues[1] + 
            (this->keyFrameValues[2] - this->keyFrameValues[1]) * 
            ((pos-this->keyFramePos1) / (this->keyFramePos2-this->keyFramePos1));
    }
    else
    {
        value = this->keyFrameValues[2] + 
            (this->keyFrameValues[3] - this->keyFrameValues[2]) * 
            ((pos-this->keyFramePos2) / (1.0f-this->keyFramePos2));
    }

    if (this->amplitude > 0.0f)
    {
        if (Sine == this->modulationFunc)
        {
            value += n_sin(pos * N_PI * 2 * this->frequency) * this->amplitude;
        }
        else
        {
            value += n_cos(pos * N_PI * 2 * this->frequency) * this->amplitude;
        }
    }

    return value;
}

//------------------------------------------------------------------------------
/**
*/
inline
float nEnvelopeCurve::GetMaxPossibleValue() const
{
    float maxVal;
    int keyFrame;
    maxVal = this->keyFrameValues[0];
    for (keyFrame = 1; keyFrame < NumValues; keyFrame++)
    {
        maxVal = n_max(maxVal, this->keyFrameValues[keyFrame]);
    }

    return maxVal + this->amplitude;
}
//------------------------------------------------------------------------------
#endif
