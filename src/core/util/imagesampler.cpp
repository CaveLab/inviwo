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

#include <inviwo/core/util/imagesampler.h>

#include <inviwo/core/util/interpolation.h>

namespace inviwo {

ImageSampler::ImageSampler(const LayerRAM *ram) : layer_(ram), dims_(layer_->getDimensions()) {}

ImageSampler::ImageSampler(const Layer *layer)
    : ImageSampler(layer->getRepresentation<LayerRAM>()) {}

ImageSampler::ImageSampler(const Image *img) : ImageSampler(img->getColorLayer()) {}

ImageSampler::~ImageSampler() {}

inviwo::dvec4 ImageSampler::sample(const dvec2 &pos) const {
    dvec2 samplePos = pos * dvec2(dims_-size2_t(1));
    size2_t indexPos = size2_t(samplePos);
    dvec2 interpolants = samplePos - dvec2(indexPos);

    dvec4 samples[4];
    samples[0] = layer_->getValueAsVec4Double(indexPos);
    samples[1] = layer_->getValueAsVec4Double(indexPos + size2_t(1, 0));
    if (pos.y == 0) {
        return Interpolation::linear(samples, interpolants.x);
    }
    samples[2] = layer_->getValueAsVec4Double(indexPos + size2_t(0, 1));
    samples[3] = layer_->getValueAsVec4Double(indexPos + size2_t(1, 1));

    return Interpolation::bilinear(samples, interpolants);
}

}  // namespace
