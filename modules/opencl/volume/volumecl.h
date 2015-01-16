/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#ifndef IVW_VOLUMECL_H
#define IVW_VOLUMECL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/volume/volumeclbase.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCL: public VolumeCLBase, public VolumeRepresentation {

public:
    VolumeCL(const DataFormatBase* format = DataFormatBase::get(), const void* data = NULL);
    VolumeCL(uvec3 dimensions, const DataFormatBase* format = DataFormatBase::get(), const void* data = NULL);
    virtual ~VolumeCL();
    VolumeCL(const VolumeCL& rhs);
    virtual void initialize() {};
    virtual void deinitialize();
    virtual VolumeCL* clone() const;

    void initialize(const void* voxels);

    virtual void setDimensions(uvec3 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(); }

    void upload(const void* data);
    /**
     * Download data to preallocated memory.
     *
     * @param data (void *) Preallocated pointer that will contain data after function returns.
     * @return (void)
     */
    void download(void* data) const;
    cl::ImageFormat getFormat() const { return imageFormat_;}

    virtual cl::Image3D& getEditable() { return *static_cast<cl::Image3D*>(clImage_); }
    virtual const cl::Image3D& get() const { return *const_cast<const cl::Image3D*>(static_cast<const cl::Image3D*>(clImage_)); }

protected:
    cl::ImageFormat imageFormat_;
};



} // namespace

namespace cl {

// Kernel argument specializations for VolumeCL type
// (enables calling cl::Queue::setArg with VolumeCL
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCL& value);

} // namespace cl

#endif // IVW_VOLUMECL_H
