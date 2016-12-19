/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2016 Inviwo Foundation
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

#ifndef IVW_ABUFFER_GEOMETRYGL_RENDERING_PROCESSOR_H
#define IVW_ABUFFER_GEOMETRYGL_RENDERING_PROCESSOR_H

#include <modules/abuffergl/abufferglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/minmaxproperty.h>
#include <inviwo/core/interaction/trackball.h>
#include <modules/basegl/processors/meshrenderprocessorgl.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/texture/textureunit.h>
#include <inviwo/core/interaction/interactionhandler.h>

#include <modules/abuffergl/abufferglhelpers/abuffergl.h>

namespace inviwo {

class IVW_MODULE_ABUFFERGL_API ABufferGeometryGLProcessor : public MeshRenderProcessorGL {
public:
    ABufferGeometryGLProcessor();
    virtual ~ABufferGeometryGLProcessor();
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    virtual void initializeResources();
    virtual void process();
    void onAbufferSettingChanged();
    void onAbufferTransparencyChanged();

private:
    void geometryRender();
    Inviwo_ABufferGL4 abuffer_;
    FloatProperty transparency_;
    bool updateRequried_;
    Shader abufferGeometryShader_;
};

}  // namespace

#endif  // IVW_ABUFFER_GEOMETRYGL_RENDERING_PROCESSOR_H
