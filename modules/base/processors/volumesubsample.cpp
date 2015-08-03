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

#include "volumesubsample.h"
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

ProcessorClassIdentifier(VolumeSubsample, "org.inviwo.VolumeSubsample");
ProcessorDisplayName(VolumeSubsample, "Volume Subsample");
ProcessorTags(VolumeSubsample, Tags::CPU);
ProcessorCategory(VolumeSubsample, "Volume Operation");
ProcessorCodeState(VolumeSubsample, CODE_STATE_EXPERIMENTAL);

VolumeSubsample::VolumeSubsample()
    : Processor()
    , inport_("volume.inport")
    , outport_("volume.outport")
    , enabled_("enabled", "Enable Operation", true)
    , subSampleFactor_("subSampleFacor", "Factor")
    , dirty_(false) {
    addPort(inport_);
    addPort(outport_);
    addProperty(enabled_);

    subSampleFactor_.addOption("none", "None", VolumeRAMSubSample::Factor::None);
    subSampleFactor_.addOption("half", "Half", VolumeRAMSubSample::Factor::Half);
    subSampleFactor_.addOption("third", "Third", VolumeRAMSubSample::Factor::Third);
    subSampleFactor_.addOption("fourth", "Fourth", VolumeRAMSubSample::Factor::Fourth);
    subSampleFactor_.setSelectedIdentifier("half");
    subSampleFactor_.setCurrentStateAsDefault();
    addProperty(subSampleFactor_);
}

VolumeSubsample::~VolumeSubsample() {}

void VolumeSubsample::process() {
    if (enabled_.get() && subSampleFactor_.get() != VolumeRAMSubSample::Factor::None) {
        if (result_.valid() &&
            result_.wait_for(std::chrono::duration<int, std::milli>(0)) ==
                std::future_status::ready) {
            std::unique_ptr<Volume> volume = std::move(result_.get());

            // pass meta data on
            const Volume* data = inport_.getData();
            volume->copyMetaDataFrom(*data);
            volume->dataMap_ = data->dataMap_;
            volume->setModelMatrix(data->getModelMatrix());
            volume->setWorldMatrix(data->getWorldMatrix());
            outport_.setData(volume.release());
            getActivityIndicator().setActive(false);
            dirty_ = false;

        } else if (!result_.valid()) {
            const Volume* data = inport_.getData();
            const VolumeRAM* vol = data->getRepresentation<VolumeRAM>();
            getActivityIndicator().setActive(true);
            result_ = dispatchPool(
                [this](const VolumeRAM* v, VolumeRAMSubSample::Factor f)
                    -> std::unique_ptr<Volume> {
                    auto volume = util::make_unique<Volume>(VolumeRAMSubSample::apply(v, f));
                    dispatchFront([this]() {
                        dirty_ = true;
                        invalidate(INVALID_OUTPUT);
                    });
                    return volume;
                },
                vol, subSampleFactor_.get());
        }
    } else {
        outport_.setConstData(inport_.getData());
    }
}

void VolumeSubsample::invalidate(InvalidationLevel invalidationLevel, Property* modifiedProperty) {
    notifyObserversInvalidationBegin(this);
    PropertyOwner::invalidate(invalidationLevel, modifiedProperty);

    if (dirty_ || inport_.isChanged() ||
        !enabled_.get() || subSampleFactor_.get() == VolumeRAMSubSample::Factor::None) {
        outport_.invalidate(INVALID_OUTPUT);
    }

    notifyObserversInvalidationEnd(this);
}

}  // inviwo namespace
