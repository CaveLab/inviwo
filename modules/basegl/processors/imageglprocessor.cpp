/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
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

#include "imageglprocessor.h"
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <modules/opengl/shaderutils.h>
#include <modules/opengl/glwrap/framebufferobject.h>

namespace inviwo {

ImageGLProcessor::ImageGLProcessor(std::string fragmentShader)
    : Processor()
    , inport_(fragmentShader + "inport")
    , outport_(fragmentShader + "outport", COLOR_ONLY)
    , internalInvalid_(false)
    , fragmentShader_(fragmentShader)
    , shader_(NULL)
{
    addPort(inport_);
    addPort(outport_);

    inport_.onChange(this,&ImageGLProcessor::inportChanged);
}

ImageGLProcessor::~ImageGLProcessor() {}

void ImageGLProcessor::initialize() {
    Processor::initialize();
    delete shader_;
    shader_ = new Shader(fragmentShader_, true);
    internalInvalid_ = true;
}

void ImageGLProcessor::deinitialize() {
    Processor::deinitialize();
    delete shader_;
}

void ImageGLProcessor::process() {
    if (internalInvalid_ || inport_.getInvalidationLevel() >= INVALID_OUTPUT) {
        internalInvalid_ = false;
        const DataFormatBase* format = inport_.getData()->getDataFormat();

        Image *img = new Image(inport_.getData()->getDimensions(), COLOR_ONLY, format);
        img->copyMetaDataFrom(*inport_.getData());
        outport_.setData(img);
    }
    inport_.passOnDataToOutport(&outport_);

    TextureUnit imgUnit;    
    utilgl::bindColorTexture(inport_, imgUnit);

    utilgl::activateTarget(outport_);
    shader_->activate();

    utilgl::setShaderUniforms(shader_, outport_, "outportParameters_");
    shader_->setUniform("inport_", imgUnit.getUnitNumber());

    preProcess();

    utilgl::singleDrawImagePlaneRect();
    shader_->deactivate();
    utilgl::deactivateCurrentTarget();

    postProcess();
}

}  // namespace
