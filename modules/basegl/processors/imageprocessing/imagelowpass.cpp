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

#include "imagelowpass.h"

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
ProcessorClassIdentifier(ImageLowPass, "org.inviwo.ImageLowPass")
ProcessorDisplayName(ImageLowPass, "Image Low Pass") 
ProcessorTags(ImageLowPass, Tags::GL);
ProcessorCategory(ImageLowPass, "Image Operation");
ProcessorCodeState(ImageLowPass, CODE_STATE_EXPERIMENTAL);

ImageLowPass::ImageLowPass()
    : ImageGLProcessor("img_lowpass.frag")
    , kernelSize_("kernelSize", "Kernel Size", 3, 1, 15, 2) {
    addProperty(kernelSize_);
}

void ImageLowPass::preProcess() { shader_.setUniform("kernelSize", kernelSize_.get()); }

}  // namespace
