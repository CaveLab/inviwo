/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
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

#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

Image::Image(uvec2 dimensions, const DataFormatBase* format) : DataGroup() {
    initialize(nullptr, dimensions, format);
}

Image::Image(Layer* colorLayer) : DataGroup() {
    initialize(colorLayer);
}

Image::Image(const Image& rhs)
    : DataGroup(rhs) {
    for (const auto& elem : rhs.colorLayers_) {
        addColorLayer((elem)->clone());
    }

    const Layer* depth = rhs.getDepthLayer();
    if (depth) {
        depthLayer_ = depth->clone();
    } else {
        depthLayer_ = nullptr;
    }

    const Layer* picking = rhs.getPickingLayer();
    if (picking) {
        pickingLayer_ = picking->clone();
    } else {
        pickingLayer_ = nullptr;
    }
}

Image& Image::operator=(const Image& that) {
    if (this != &that) {
        DataGroup::operator=(that);
        deinitialize();

        for (const auto& elem : that.colorLayers_) {
            addColorLayer((elem)->clone());
        }

        const Layer* depth = that.getDepthLayer();
        if (depth) {
            depthLayer_ = depth->clone();
        } else {
            depthLayer_ = nullptr;
        }

        const Layer* picking = that.getPickingLayer();
        if (picking) {
            pickingLayer_ = that.pickingLayer_->clone();
        } else {
            pickingLayer_ = nullptr;
        }
    }

    return *this;
}

Image* Image::clone() const {
    return new Image(*this);
}

Image::~Image() {
    //Delete all layers
    deinitialize();
}

void Image::deinitialize() {
    for (auto& elem : colorLayers_) delete (elem);
    colorLayers_.clear();
    delete depthLayer_;
    depthLayer_ = nullptr;
    delete pickingLayer_;
    pickingLayer_ = nullptr;
}

void Image::initialize(Layer* colorLayer, uvec2 dimensions, const DataFormatBase* format) {
    if (colorLayer) {
        addColorLayer(colorLayer);
        dimensions = colorLayer->getDimensions();
        format = colorLayer->getDataFormat();
    } else {
        addColorLayer(new Layer(dimensions, format));
    }

    depthLayer_ = new Layer(dimensions, DataFLOAT32::get(), DEPTH_LAYER);
    pickingLayer_ = new Layer(dimensions, format, PICKING_LAYER);
}

uvec2 Image::getDimensions() const {
    return getColorLayer()->getDimensions();
}

size_t Image::addColorLayer(Layer* layer) {
    colorLayers_.push_back(layer);
    //Return index to this layer
    return colorLayers_.size()-1;
}

const Layer* Image::getLayer(LayerType type, size_t idx) const {
    switch (type) {
        case COLOR_LAYER:
            return getColorLayer(idx);

        case DEPTH_LAYER:
            return getDepthLayer();

        case PICKING_LAYER:
            return getPickingLayer();
    }

    return nullptr;
}

Layer* Image::getLayer(LayerType type, size_t idx) {
    switch (type) {
        case COLOR_LAYER:
            return getColorLayer(idx);

        case DEPTH_LAYER:
            return getDepthLayer();

        case PICKING_LAYER:
            return getPickingLayer();
    }

    return nullptr;
}

const Layer* Image::getColorLayer(size_t idx) const {
    return colorLayers_[idx];
}

Layer* Image::getColorLayer(size_t idx) {
    return colorLayers_[idx];
}

size_t Image::getNumberOfColorLayers() const {
    return colorLayers_.size();
}

const Layer* Image::getDepthLayer() const {
    return depthLayer_;
}

Layer* Image::getDepthLayer() {
    return depthLayer_;
}

const Layer* Image::getPickingLayer() const {
    return pickingLayer_;
}

Layer* Image::getPickingLayer() {
    return pickingLayer_;
}

void Image::resize(uvec2 dimensions) {
    setRepresentationsAsInvalid();

    //Resize all layers
    for (auto& elem : colorLayers_) (elem)->resize(dimensions);

    if (depthLayer_)
        depthLayer_->resize(dimensions);

    if (pickingLayer_)
        pickingLayer_->resize(dimensions);
}

void Image::resizeRepresentations(Image* targetImage, uvec2 targetDim) const {
    // targetImage->resize(targetDim);
    std::vector<DataGroupRepresentation*>& targetRepresentations = targetImage->representations_;
    size_t numRepTargets = targetRepresentations.size();

    if (numRepTargets > 0) {
        //Scheme: Only ask for one editable representations to resize
        //Thus all others can update from one resized version

        // Find out in which preferred order we wanna try resizing
        // We prefer the order, unknown - ImageRAM - ImageDisk.
        std::vector<size_t> preferredResizeOrder;
        preferredResizeOrder.resize(targetRepresentations.size());
        size_t nextInsertIdx = 0;
        bool imageDiskFound = false;
        bool imageRamFound = false;
        for (size_t j = 0; j < numRepTargets; j++) {
            if (dynamic_cast<ImageRAM*>(targetRepresentations[j])){
                if(imageDiskFound){
                    preferredResizeOrder[numRepTargets-2] = j;
                } else {
                    preferredResizeOrder[numRepTargets-1] = j;
                }
                imageRamFound = true;
            }
            else if (dynamic_cast<ImageDisk*>(targetRepresentations[j])) {
                if(imageRamFound){
                    preferredResizeOrder[numRepTargets-2] = preferredResizeOrder[numRepTargets-1];
                    preferredResizeOrder[numRepTargets-1] = j;
                } else {
                    preferredResizeOrder[numRepTargets-1] = j;
                }
                imageDiskFound = true;
            }
            else {
                preferredResizeOrder[nextInsertIdx] = j;
                nextInsertIdx++;
            }
        }

        ImageRepresentation* sourceImageRepresentation = nullptr;
        ImageRepresentation* targetImageRepresentation = nullptr;

        bool copyDone = false;
        for (size_t i = 0; i < targetRepresentations.size() && !copyDone; i++) {
            for (size_t j = 0; j < representations_.size() && !copyDone; j++) {
                if (typeid(*representations_[j]) == typeid(*targetRepresentations[preferredResizeOrder[i]])) {
                    sourceImageRepresentation =
                        static_cast<ImageRepresentation*>(representations_[j]);
                    sourceImageRepresentation->update(false);
                    targetImageRepresentation =
                        static_cast<ImageRepresentation*>(targetRepresentations[preferredResizeOrder[i]]);
                    targetImageRepresentation->update(true);
                    sourceImageRepresentation->copyAndResizeRepresentation(
                        targetImageRepresentation);
                    copyDone = true;
                }
            }
        }
    } else {
        // If not representation exist, create ImageRAM one
        const ImageRAM* imageRAM = this->getRepresentation<ImageRAM>();
        imageRAM->copyAndResizeRepresentation(targetImage->getEditableRepresentation<ImageRAM>());
    }
}

const DataFormatBase* Image::getDataFormat() const {
    return getColorLayer()->getDataFormat();
}

std::string Image::getDataInfo() const{
    std::stringstream ss;
    ss << "<table border='0' cellspacing='0' cellpadding='0' style='border-color:white;white-space:pre;'>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Type</td><td><nobr>" << "Image </nobr></td></tr>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Color channels</td><td><nobr>" << colorLayers_.size() << "</nobr></td></tr>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Depth</td><td><nobr>" << (getDepthLayer() ? "Yes" : "No") << "</nobr></td></tr>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Picking</td><td><nobr>" << (getPickingLayer() ? "Yes" : "No") << "</nobr></td></tr>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Format</td><td><nobr>" << getDataFormat()->getString() << "</nobr></td></tr>\n"
        << "<tr><td style='color:#bbb;padding-right:8px;'>Dimension</td><td><nobr>" << "(" << getDimensions().x << ", "
        << getDimensions().y << ")" << "</nobr></td></tr>\n"
        << "</tr></table>\n";
    return ss.str();
}

} // namespace
