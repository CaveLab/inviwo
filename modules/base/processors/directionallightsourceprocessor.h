/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#ifndef IVW_DIRECTIONAL_LIGHT_SOURCE_PROCESSOR_H
#define IVW_DIRECTIONAL_LIGHT_SOURCE_PROCESSOR_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/light/baselightsource.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <modules/base/basemoduledefine.h>

namespace inviwo {

class DirectionalLight;
/** \docpage{org.inviwo.Directionallightsource, Directional light source}
 * ![](org.inviwo.Directionallightsource.png?classIdentifier=org.inviwo.Directionallightsource)
 *
 * ...
 * 
 * ### Outports
 *   * __DirectionalLightSource__ ...
 * 
 * ### Properties
 *   * __Light power__ ...
 *   * __Color__ ...
 *   * __Light Source Position__ ...
 *   * __Enabled__ ...
 */
class IVW_MODULE_BASE_API DirectionalLightSourceProcessor : public Processor {
public:
    DirectionalLightSourceProcessor();
    virtual ~DirectionalLightSourceProcessor();

    InviwoProcessorInfo();

protected:
    virtual void process();

    /**
     * Update light source parameters. Transformation will be given in texture space.
     *
     * @param lightSource
     * @return
     */
    void updateDirectionalLightSource(DirectionalLight* lightSource);

private:
    DataOutport<LightSource> outport_;

    CompositeProperty lighting_;
    FloatProperty lightPowerProp_;
    FloatVec4Property lightDiffuse_;
    FloatVec3Property lightPosition_;
    BoolProperty lightEnabled_;
    
    DirectionalLight* lightSource_;
};

}  // namespace

#endif  // IVW_DIRECTIONAL_LIGHT_SOURCE_PROCESSOR_H
