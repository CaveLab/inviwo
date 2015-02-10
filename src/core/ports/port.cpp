/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
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

#include <inviwo/core/ports/port.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

uvec3 Port::colorCode = uvec3(128,128,128);

Port::Port(std::string identifier)
    : identifier_(identifier),
      processor_(0)
{}

Port::~Port() {}

uvec3 Port::getColorCode() const {
    return Port::colorCode;
}

Processor* Port::getProcessor() const {
    return processor_;
}

std::string Port::getIdentifier() const {
    return identifier_;
}

void Port::setIdentifier(const std::string& name) {
    identifier_ = name;
}

void Port::setProcessor(Processor* processor) {
    processor_ = processor;
}


void Port::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("identifier", identifier_, true);
//    s.serialize("Processor", processor_);
}

void Port::deserialize(IvwDeserializer& d) {
    d.deserialize("identifier", identifier_, true);
//    d.deserialize("Processor", processor_);
}

void Port::invalidate(InvalidationLevel invalidationLevel) {
    processor_->invalidate(invalidationLevel);
}

} // namespace
