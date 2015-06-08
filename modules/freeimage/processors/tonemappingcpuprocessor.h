/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#ifndef IVW_TONEMAPPINGCPUPROCESSOR_H
#define IVW_TONEMAPPINGCPUPROCESSOR_H

#include <modules/freeimage/freeimagemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

/** \docpage{org.inviwo.ToneMappingCPU, Tone Mapping}
 * ![](org.inviwo.ToneMappingCPU.png?classIdentifier=org.inviwo.ToneMappingCPU)
 *
 * ...
 * 
 * ### Inports
 *   * __inport___ ...
 * 
 * ### Outports
 *   * __outport___ ...
 * 
 * ### Properties
 *   * __toneMappingMethod___ ...
 *   * __drago03Exposure___ ...
 *   * __reinhard05Intensity___ ...
 *   * __fattal02Attenuation___ ...
 *   * __drago03Gamma___ ...
 *   * __reinhard05Contrast___ ...
 *   * __fattal02Saturation___ ...
 *
 */
class IVW_MODULE_FREEIMAGE_API ToneMappingCPUProcessor : public Processor {
public:
    ToneMappingCPUProcessor();
    ~ToneMappingCPUProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    void process();

    void toneMappingMethodChanged();

private:
    ImageInport inport_;
    ImageOutport outport_;

    OptionPropertyString toneMappingMethod_;

    DoubleProperty drago03Gamma_;
    DoubleProperty drago03Exposure_;

    DoubleProperty reinhard05Intensity_;
    DoubleProperty reinhard05Contrast_;

    DoubleProperty fattal02Saturation_;
    DoubleProperty fattal02Attenuation_;

    bool drago03Selected_;
    bool reinhard05Selected_;
    bool fattal02Selected_;
    bool passSelected_;
};

} // namespace

#endif // IVW_TONEMAPPINGCPUPROCESSOR_H
