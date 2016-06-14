/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2016 Inviwo Foundation
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

#ifndef IVW_VOLUMESAMPLER_H
#define IVW_VOLUMESAMPLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/indexmapper.h>


#include <inviwo/core/util/interpolation.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

#include <inviwo/core/util/spatialsampler.h>

namespace inviwo {

class Volume;
class VolumeRAM;

/**
 * \class VolumeDoubleSampler
 *
 * \brief VERY_BRIEFLY_DESCRIBE_THE_CLASS
 *
 * DESCRIBE_THE_CLASS
 */

template <unsigned int DataDims>
class VolumeDoubleSampler : public SpatialSampler<3, DataDims, double> {
public:
    VolumeDoubleSampler(std::shared_ptr<const Volume> vol) : SpatialSampler<3, DataDims, double>(vol), volume_(vol), ram_(vol->getRepresentation<VolumeRAM>()), dims_(vol->getDimensions()) {}
    virtual ~VolumeDoubleSampler() {}

    virtual Vector<DataDims, double> sampleDataSpace(const dvec3 &pos) const override {
        if (!withinBoundsDataSpace(pos)) {
            return Vector<DataDims, double>(0.0);
        }
        dvec3 samplePos = pos * dvec3(dims_ - size3_t(1));
        size3_t indexPos = size3_t(samplePos);
        dvec3 interpolants = samplePos - dvec3(indexPos);

        Vector<DataDims, double> samples[8];
        samples[0] = getVoxel(indexPos);
        samples[1] = getVoxel(indexPos + size3_t(1, 0, 0));
        samples[2] = getVoxel(indexPos + size3_t(0, 1, 0));
        samples[3] = getVoxel(indexPos + size3_t(1, 1, 0));

        samples[4] = getVoxel(indexPos + size3_t(0, 0, 1));
        samples[5] = getVoxel(indexPos + size3_t(1, 0, 1));
        samples[6] = getVoxel(indexPos + size3_t(0, 1, 1));
        samples[7] = getVoxel(indexPos + size3_t(1, 1, 1));

        return Interpolation<Vector<DataDims, double>>::trilinear(
            samples, interpolants, &Interpolation<Vector<DataDims, double>>::linear);
    }

protected:
    Vector<DataDims, double> getVoxel(const size3_t &pos)const;

    virtual bool withinBoundsDataSpace(const dvec3 &pos) const override {
        if (glm::any(glm::lessThan(pos, dvec3(0.0)))) {
            return false;
        }
        if (glm::any(glm::greaterThan(pos, dvec3(1.0)))) {
            return false;
        }
        return true;
    }

    std::shared_ptr<const Volume> volume_;
    const VolumeRAM* ram_;
    size3_t dims_;
};




template<> IVW_CORE_API
Vector<1, double> VolumeDoubleSampler<1>::getVoxel(const size3_t &pos) const;

template<> IVW_CORE_API
Vector<2, double> VolumeDoubleSampler<2>::getVoxel(const size3_t &pos) const;

template<> IVW_CORE_API
Vector<3, double> VolumeDoubleSampler<3>::getVoxel(const size3_t &pos) const;

template<> IVW_CORE_API
Vector<4, double> VolumeDoubleSampler<4>::getVoxel(const size3_t &pos) const;


using VolumeSampler = VolumeDoubleSampler<4>;



template <typename T, typename P>
class TemplateVolumeSampler {
public:
    TemplateVolumeSampler(const VolumeRAM *ram)
        : data_(static_cast<const T*>(ram->getData())), dims_(ram->getDimensions()), ic_(dims_) {}

    TemplateVolumeSampler(const Volume *vol)
        : TemplateVolumeSampler(vol->getRepresentation<VolumeRAM>()) {}

    TemplateVolumeSampler(std::shared_ptr<const Volume> sharedVolume)
        : TemplateVolumeSampler(sharedVolume.get()) {
        sharedVolume_ = sharedVolume;
    }

    virtual ~TemplateVolumeSampler() {}

    T sample(P x, P y, P z) { return sample(Vector<3, P>(x, y, z)); }
    T sample(const Vector<3, P> &pos) {
        Vector<3, P> samplePos = pos *  Vector<3, P>(dims_ - size3_t(1));
        size3_t indexPos = size3_t(samplePos);
        Vector<3, P> interpolants = samplePos - Vector<3, P>(indexPos);

        T samples[8];
        samples[0] = getVoxel(indexPos);
        samples[1] = getVoxel(indexPos + size3_t(1, 0, 0));
        samples[2] = getVoxel(indexPos + size3_t(0, 1, 0));
        samples[3] = getVoxel(indexPos + size3_t(1, 1, 0));
        samples[4] = getVoxel(indexPos + size3_t(0, 0, 1));
        samples[5] = getVoxel(indexPos + size3_t(1, 0, 1));
        samples[6] = getVoxel(indexPos + size3_t(0, 1, 1));
        samples[7] = getVoxel(indexPos + size3_t(1, 1, 1));

        return Interpolation<T, P>::trilinear(samples, interpolants);
    }

private:
    T getVoxel(const size3_t &pos) {
        auto p = glm::clamp(pos, size3_t(0), dims_ - size3_t(1));
        return  data_[ic_(p)];
    }

    const T *data_;
    size3_t dims_;
    util::IndexMapper3D ic_;
    std::shared_ptr<const Volume> sharedVolume_;
};




}  // namespace

#endif  // IVW_VOLUMESAMPLER_H
