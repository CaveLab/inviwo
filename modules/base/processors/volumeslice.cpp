/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2016 Inviwo Foundation
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

#include "volumeslice.h"
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/wheelevent.h>
#include <inviwo/core/interaction/events/gestureevent.h>

namespace inviwo {

const ProcessorInfo VolumeSlice::processorInfo_{
    "org.inviwo.VolumeSlice",  // Class identifier
    "Volume Slice",            // Display name
    "Volume Operation",        // Category
    CodeState::Stable,         // Code state
    Tags::CPU,                 // Tags
};
const ProcessorInfo VolumeSlice::getProcessorInfo() const {
    return processorInfo_;
}

VolumeSlice::VolumeSlice()
    : Processor()
    , inport_("inputVolume")
    , outport_("outputImage", DataVec4UInt8::get(), false)
    , sliceAlongAxis_("sliceAxis", "Slice along axis")
    , sliceNumber_("sliceNumber", "Slice Number", 4, 1, 8)
    , handleInteractionEvents_("handleEvents", "Handle interaction events", true,
                               InvalidationLevel::Valid)
    , mouseShiftSlice_("mouseShiftSlice", "Mouse Slice Shift",
                       [this](Event* e) { eventShiftSlice(e); },
                       util::make_unique<WheelEventMatcher>())

    , stepSliceUp_("stepSliceUp", "Key Slice Up", [this](Event* e) { eventStepSliceUp(e); },
                   IvwKey::W, KeyState::Press)

    , stepSliceDown_("stepSliceDown", "Key Slice Down", [this](Event* e) { eventStepSliceDown(e); },
                     IvwKey::S, KeyState::Press)

    , gestureShiftSlice_(
          "gestureShiftSlice", "Gesture Slice Shift",
          [this](Event* e) { eventGestureShiftSlice(e); },
          util::make_unique<GestureEventMatcher>(GestureType::Pan, GestureStates(flags::any), 3)) {
        
    addPort(inport_);
    addPort(outport_);
    outport_.setHandleResizeEvents(false);
    sliceAlongAxis_.addOption("x", "X axis", static_cast<int>(CartesianCoordinateAxis::X));
    sliceAlongAxis_.addOption("y", "Y axis", static_cast<int>(CartesianCoordinateAxis::Y));
    sliceAlongAxis_.addOption("z", "Z axis", static_cast<int>(CartesianCoordinateAxis::Z));
    sliceAlongAxis_.setSelectedIndex(0);
    sliceAlongAxis_.setCurrentStateAsDefault();
    addProperty(sliceAlongAxis_);
    addProperty(sliceNumber_);

    addProperty(handleInteractionEvents_);

    mouseShiftSlice_.setVisible(false);
    mouseShiftSlice_.setCurrentStateAsDefault();
    addProperty(mouseShiftSlice_);

    addProperty(stepSliceUp_);
    addProperty(stepSliceDown_);

    gestureShiftSlice_.setVisible(false);
    gestureShiftSlice_.setCurrentStateAsDefault();
    addProperty(gestureShiftSlice_);
}

VolumeSlice::~VolumeSlice() = default;

void VolumeSlice::invokeEvent(Event* event) {
    if (!handleInteractionEvents_) return;
    Processor::invokeEvent(event);
}

void VolumeSlice::shiftSlice(int shift) {
    int newSlice = sliceNumber_.get() + shift;
    if (newSlice >= sliceNumber_.getMinValue() && newSlice <= sliceNumber_.getMaxValue())
        sliceNumber_.set(newSlice);
}

void VolumeSlice::process() {
    auto vol = inport_.getData();

    const ivec3 dims(vol->getDimensions());

    switch (static_cast<CartesianCoordinateAxis>(sliceAlongAxis_.get())) {
        case CartesianCoordinateAxis::X:
            if (dims.x != sliceNumber_.getMaxValue()) {
                sliceNumber_.setMaxValue(dims.x);
                sliceNumber_.set(dims.x / 2);
                sliceNumber_.setCurrentStateAsDefault();
            }
            break;
        case CartesianCoordinateAxis::Y:
            if (dims.y != sliceNumber_.getMaxValue()) {
                sliceNumber_.setMaxValue(dims.y);
                sliceNumber_.set(dims.y / 2);
            }
            break;
        case CartesianCoordinateAxis::Z:
            if (dims.z != sliceNumber_.getMaxValue()) {
                sliceNumber_.setMaxValue(dims.z);
                sliceNumber_.set(dims.z / 2);
            }
            break;
    }

    VolumeSliceDispatcher disp;
    image_ = vol->getDataFormat()->dispatch(
        disp, *vol, static_cast<CartesianCoordinateAxis>(sliceAlongAxis_.get()),
        static_cast<size_t>(sliceNumber_.get() - 1), image_);

    outport_.setData(image_);
}

void VolumeSlice::eventShiftSlice(Event* event) {
    auto wheelEvent = static_cast<WheelEvent*>(event);
    int steps = static_cast<int>(wheelEvent->delta().y);
    shiftSlice(steps);
}

void VolumeSlice::eventStepSliceUp(Event*) { shiftSlice(1); }

void VolumeSlice::eventStepSliceDown(Event*) { shiftSlice(-1); }

void VolumeSlice::eventGestureShiftSlice(Event* event) {
    GestureEvent* gestureEvent = static_cast<GestureEvent*>(event);
    if (gestureEvent->deltaPos().y < 0)
        shiftSlice(1);
    else if (gestureEvent->deltaPos().y > 0)
        shiftSlice(-1);
}

}  // inviwo namespace

