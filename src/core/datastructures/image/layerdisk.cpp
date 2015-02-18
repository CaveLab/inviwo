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

#include <inviwo/core/datastructures/image/layerdisk.h>

namespace inviwo {

LayerDisk::LayerDisk(LayerType type)
    : LayerRepresentation(uvec2(0), type, DataFormatBase::get()), DiskRepresentation() {
    initialize();
}

LayerDisk::LayerDisk(std::string url, LayerType type)
    : LayerRepresentation(uvec2(0), type, DataFormatBase::get()), DiskRepresentation(url) {
    initialize();
}

LayerDisk::LayerDisk(const LayerDisk& rhs)
    : LayerRepresentation(rhs)
    , DiskRepresentation(rhs) {
    initialize();
}

LayerDisk& LayerDisk::operator=(const LayerDisk& that) {
    if (this != &that)
        LayerRepresentation::operator=(that);

    return *this;
}

LayerDisk::~LayerDisk() {
    deinitialize();
}

LayerDisk* LayerDisk::clone() const {
    return new LayerDisk(*this);
}

void LayerDisk::initialize() {}

void LayerDisk::deinitialize() {}

bool LayerDisk::copyAndResizeLayer(DataRepresentation*) const {
    return false;
}

void LayerDisk::updateDataFormat(const DataFormatBase* format){
    setDataFormat(format);
}

} // namespace
