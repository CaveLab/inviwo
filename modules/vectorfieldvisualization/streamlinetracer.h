/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015 Inviwo Foundation
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

#ifndef IVW_STREAMTRACER_H
#define IVW_STREAMTRACER_H

#include <modules/vectorfieldvisualization/vectorfieldvisualizationmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/vectorfieldvisualization/integralline.h>
#include <inviwo/core/util/volumesampler.h>

namespace inviwo {

/**
 * \class StreamLineTracer
 *
 * \brief VERY_BRIEFLY_DESCRIBE_THE_CLASS
 *
 * DESCRIBE_THE_CLASS
 */
class IVW_MODULE_VECTORFIELDVISUALIZATION_API StreamLineTracer {
public:
    enum class IntegrationScheme {
        Euler ,
        RK4 
    };

    enum class Direction { FWD = 1, BWD = 2, BOTH = 3 };

    StreamLineTracer(const Volume *vol , IntegrationScheme integrationScheme = IntegrationScheme::RK4);

    virtual ~StreamLineTracer();

    void addMetaVolume(const std::string &name, const VolumeRAM *vol);

    IntegralLine traceFrom(const dvec3 &p, int steps, double stepSize, Direction dir,
                           bool normalzieSample);
    IntegralLine traceFrom(const vec3 &p, int steps, double stepSize, Direction dir,
                           bool normalzieSample);

    IntegrationScheme getIntegrationScheme() const { return integrationScheme_; }
    void setIntegrationScheme(IntegrationScheme scheme) { integrationScheme_ = scheme; }

private:
    void step(int steps, dvec3 curPos, IntegralLine &line, double stepSize, bool normalzieSample);
    dvec3 euler(const dvec3 &curPos);
    dvec3 rk4(const dvec3 &curPos , double stepSize, bool normalzieSample, dmat3 m);
    dmat3 invBasis_;
    std::map<std::string, VolumeSampler> metaVolumes_;
    VolumeSampler volumeSampler_;
    size3_t dimensions_;
    IntegrationScheme integrationScheme_;
    
};

}  // namespace

#endif  // IVW_INTEGRALLINETRACER_H
