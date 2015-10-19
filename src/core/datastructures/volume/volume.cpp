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

#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/util/tooltiphelper.h>

namespace inviwo {

Volume::Volume(size3_t dimensions, const DataFormatBase* format)
    : Data<VolumeRepresentation>(format), StructuredGridEntity<3>(dimensions), dataMap_(format) {}

Volume::Volume(const Volume& rhs)
    : Data<VolumeRepresentation>(rhs), StructuredGridEntity<3>(rhs), dataMap_(rhs.dataMap_) {}

Volume::Volume(std::shared_ptr<VolumeRepresentation> in)
    : Data<VolumeRepresentation>(in->getDataFormat())
    , StructuredGridEntity<3>(in->getDimensions())
    , dataMap_(in->getDataFormat()) {
    addRepresentation(in);
}

Volume& Volume::operator=(const Volume& that) {
    if (this != &that) {
        Data<VolumeRepresentation>::operator=(that);
        StructuredGridEntity<3>::operator=(that);
        dataMap_ = that.dataMap_;
    }

    return *this;
}
Volume* Volume::clone() const { return new Volume(*this); }
Volume::~Volume() {}

std::string Volume::getDataInfo() const {
    ToolTipHelper t("Volume");
    t.tableTop();

    t.row("Format", getDataFormat()->getString());
    t.row("Dimension", toString(getDimensions()));
    t.row("Data Range", toString(dataMap_.dataRange));
    t.row("Value Range", toString(dataMap_.valueRange));

    if (hasRepresentation<VolumeRAM>()) {
        auto volumeRAM = getRepresentation<VolumeRAM>();
        if (volumeRAM->hasHistograms()) {
            auto histograms = volumeRAM->getHistograms();
            for (size_t i = 0; i < histograms->size(); ++i) {
                std::stringstream ss;
                ss << "Channel " << i << " Min: " << (*histograms)[i].stats_.min
                   << " Mean: " << (*histograms)[i].stats_.mean
                   << " Max: " << (*histograms)[i].stats_.max
                   << " Std: " << (*histograms)[i].stats_.standardDeviation;
                t.row("Stats", ss.str());

                std::stringstream ss2;
                ss2 << "(1: " << (*histograms)[i].stats_.percentiles[1]
                   << ", 25: " << (*histograms)[i].stats_.percentiles[25]
                   << ", 50: " << (*histograms)[i].stats_.percentiles[50]
                   << ", 75: " << (*histograms)[i].stats_.percentiles[75]
                   << ", 99: " << (*histograms)[i].stats_.percentiles[99] << ")";
                t.row("Percentiles", ss2.str());
            }
        }
    }

    t.tableBottom();
    return t;
}

size3_t Volume::getDimensions() const {
    if (hasRepresentations() && lastValidRepresentation_) {
        return lastValidRepresentation_->getDimensions();
    }
    return StructuredGridEntity<3>::getDimensions();
}

void Volume::setDimensions(const size3_t& dim) {
    StructuredGridEntity<3>::setDimensions(dim);

    if (lastValidRepresentation_) {
        // Resize last valid representation
        lastValidRepresentation_->setDimensions(dim);
        removeOtherRepresentations(lastValidRepresentation_.get());
    }
}

void Volume::setOffset(const vec3& offset) {
    SpatialEntity<3>::setOffset(Vector<3, float>(offset));
}
vec3 Volume::getOffset() const { return SpatialEntity<3>::getOffset(); }

mat3 Volume::getBasis() const { return SpatialEntity<3>::getBasis(); }
void Volume::setBasis(const mat3& basis) { SpatialEntity<3>::setBasis(Matrix<3, float>(basis)); }

mat4 Volume::getModelMatrix() const { return SpatialEntity<3>::getModelMatrix(); }
void Volume::setModelMatrix(const mat4& mat) {
    SpatialEntity<3>::setModelMatrix(Matrix<4, float>(mat));
}

mat4 Volume::getWorldMatrix() const { return SpatialEntity<3>::getWorldMatrix(); }
void Volume::setWorldMatrix(const mat4& mat) {
    SpatialEntity<3>::setWorldMatrix(Matrix<4, float>(mat));
}

std::shared_ptr<VolumeRepresentation> Volume::createDefaultRepresentation() const {
    return createVolumeRAM(getDimensions(), getDataFormat());
}

float Volume::getWorldSpaceGradientSpacing() const {
    mat3 textureToWorld = mat3(getCoordinateTransformer().getTextureToWorldMatrix());

    // Find the maximum distance we can go from the center of a voxel without ending up outside the voxel
    // Shorten each basis to the distance from one voxel to the next
    mat3 voxelSpaceBasis;
    for (int dim = 0; dim < 3; ++dim) {
        voxelSpaceBasis[dim] = textureToWorld[dim]/static_cast<float>(getDimensions()[dim]);
    }
    
    // Find the distance three of the sides of a voxel
    // Project x-axis onto the y-axis. 
    // Distance from that point to the x-axis will be the shortest distance 
    vec3 distanceToSide;
    // Project x onto y axis, x onto z axis and y onto z axis
    vec3 xOntoY = voxelSpaceBasis[1]*glm::dot(voxelSpaceBasis[0], voxelSpaceBasis[1]);
    vec3 xOntoZ = voxelSpaceBasis[2]*glm::dot(voxelSpaceBasis[0], voxelSpaceBasis[2]);
    vec3 yOntoZ = voxelSpaceBasis[2]*glm::dot(voxelSpaceBasis[1], voxelSpaceBasis[2]);
    distanceToSide[0] = glm::distance(voxelSpaceBasis[0], xOntoY);
    distanceToSide[1] = glm::distance(voxelSpaceBasis[0], xOntoZ);
    distanceToSide[2] = glm::distance(voxelSpaceBasis[1], yOntoZ);

    // From the center of the voxel we can go half of the minimum distance without going outside
    float minimumDistance = 0.5f*std::min(distanceToSide[0], std::min(distanceToSide[1], distanceToSide[2]));
    // Return the minimum distance we can travel along each basis
    return minimumDistance;
}

inviwo::uvec3 Volume::COLOR_CODE = uvec3(188, 101, 101);
const std::string Volume::CLASS_IDENTIFIER = "org.inviwo.Volume";

const StructuredCameraCoordinateTransformer<3>& Volume::getCoordinateTransformer(
    const Camera& camera) const {
    return StructuredGridEntity<3>::getCoordinateTransformer(camera);
}

}  // namespace