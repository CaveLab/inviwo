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

#include "distancetransformram.h"

namespace inviwo {

const ProcessorInfo DistanceTransformRAM::processorInfo_{
    "org.inviwo.DistanceTransformRAM",  // Class identifier
    "Distance Transform",               // Display name
    "Volume Operation",                 // Category
    CodeState::Experimental,            // Code state
    Tags::CPU,                          // Tags
};
const ProcessorInfo DistanceTransformRAM::getProcessorInfo() const {
    return processorInfo_;
}

DistanceTransformRAM::DistanceTransformRAM()
    : Processor()
    , volumePort_("volume.inport")
    , outport_("volume.outport")
    , transformEnabled_("transformActive", "Enabled", true)
    , resultSquaredDist_("distSquared", "Squared Distance", true)
    , resultDistScale_("distScale", "Scaling Factor", 1.0f, 0.0f, 1.0e3, 0.05f)
    , btnForceUpdate_("forceUpdate", "Update Distance Map")
    , volDim_(1u)
    , dirty_(true)
    , distTransformDirty_(true)
    , numThreads_(8) {
    addPort(volumePort_);
    addPort(outport_);

    addProperty(transformEnabled_);
    addProperty(resultSquaredDist_);
    addProperty(resultDistScale_);
    addProperty(btnForceUpdate_);

    transformEnabled_.onChange(this, &DistanceTransformRAM::paramChanged);
    btnForceUpdate_.onChange(this, &DistanceTransformRAM::paramChanged);

    progressBar_.hide();

#ifndef __clang__
    if (numThreads_ == 0) numThreads_ = 2 * omp_get_max_threads();
    LogInfo("max available threads (OpenMP): " << omp_get_max_threads());

    omp_set_num_threads(numThreads_);
    int id;
#pragma omp parallel
    {
        id = omp_get_thread_num();
        if (id == 0) {
            LogInfo("Threads used: " << omp_get_num_threads());
        }
    }
#endif
}

DistanceTransformRAM::~DistanceTransformRAM() {}

void DistanceTransformRAM::process() {
    if (!transformEnabled_.get()) {
        // copy inport to outport
        outport_.setData(volumePort_.getData());
        return;
    }

    if (dirty_ || volumePort_.isChanged()) {
        dirty_ = false;

        std::shared_ptr<const Volume> srcVolume = volumePort_.getData();
        volDim_ = glm::max(srcVolume->getDimensions(), size3_t(1u));

        if (!volDist_ || (volDist_->getDimensions() != volDim_) || (volDist_ == srcVolume)) {
            // Volume* volume = new Volume(volDim_, DataUInt32::get());
            volDist_ = std::make_shared<Volume>(volDim_, DataUInt16::get());
            volDist_->setModelMatrix(srcVolume->getModelMatrix());
            volDist_->setWorldMatrix(srcVolume->getWorldMatrix());
            // pass meta data on
            volDist_->copyMetaDataFrom(*srcVolume);
            outport_.setData(volDist_);
        }
        distTransformDirty_ = true;
    }

    if (!dirty_ && distTransformDirty_) {
        // progressBar_.resetProgress();
        // progressBar_.show();

        updateOutport();

        // progressBar_.finishProgress();
        // progressBar_.hide();
    }
}

void DistanceTransformRAM::updateOutport() {
    VolumeRAM* vol = volDist_->getEditableRepresentation<VolumeRAM>();
    DataFormatId dataFormat = vol->getDataFormat()->getId();
#include <warn/push>
#include <warn/ignore/switch-enum>
    switch (dataFormat) {
        case DataFormatId::NotSpecialized:
            break;
//#define DataFormatIdMacro(i) case i: computeDistanceTransform<Data##i::type, Data##i::bits>();
//break;
//#include <inviwo/core/util/formatsdefinefunc.h>
#define DataFormatIdMacro(i) case DataFormatId::i: computeDistanceTransform<Data##i::type>(); break;
DataFormatIdMacro(Float16)
DataFormatIdMacro(Float32)
DataFormatIdMacro(Float64)
DataFormatIdMacro(Int8)
DataFormatIdMacro(Int16)
DataFormatIdMacro(Int32)
DataFormatIdMacro(Int64)
DataFormatIdMacro(UInt8)
DataFormatIdMacro(UInt16)
DataFormatIdMacro(UInt32)
DataFormatIdMacro(UInt64)
#undef DataFormatIdMacro

    default:
        break;
    }
#include <warn/pop>
    distTransformDirty_ = false;
}

void DistanceTransformRAM::paramChanged() { distTransformDirty_ = true; }

}  // namespace

