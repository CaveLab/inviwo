 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
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

#ifndef IVW_VOLUME_MAX_OPERATION_CL_H
#define IVW_VOLUME_MAX_OPERATION_CL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>

#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/kernelowner.h>
#include <modules/opencl/buffer/bufferclbase.h>
#include <modules/opencl/volume/volumeclbase.h>

#include <modules/basecl/baseclmoduledefine.h>


namespace inviwo {

/** \docpage{org.inviwo.VolumeMaxCL, Volume Max}
 * Computes the maximum data value for each sub-region of the input volume.
 *
 * ### Inports
 *   * __VolumeInport__ Volume input.
 *
 * ### Outports
 *   * __VolumeOutport__ Volume output of size: input volume dimension / region size, uint8.
 * 
 * ### Properties
 *   * __Region size__ Size of each sub region to compute maximum value for.
 *   * __Work group size__ OpenCL work group size (performance)
 *   * __Use OpenGL sharing__ Share data with OpenGL (performance and compability).
 */

/**
 * \brief Computes the maximum data value for each sub-region of the input volume.
 *
 */
class IVW_MODULE_BASECL_API VolumeMaxCLProcessor : public Processor, public ProcessorKernelOwner {

public:
    VolumeMaxCLProcessor();
    ~VolumeMaxCLProcessor();

    InviwoProcessorInfo();

    void initialize();

    void buildKernel();

    void deinitialize();


protected:
    virtual void process();
    void executeVolumeOperation(const Volume* volume, const VolumeCLBase* volumeCL, VolumeCLBase* volumeOutCL, const uvec3& outDim, const svec3& globalWorkGroupSize,
                                const svec3& localWorkgroupSize);
private:
    VolumeInport inport_;
    VolumeOutport outport_;

    IntProperty volumeRegionSize_;
    IntVec3Property workGroupSize_;
    BoolProperty useGLSharing_;

    bool supportsVolumeWrite_; // Does the OpenCL device support volume writes?
    Buffer* tmpVolume_; // Used if writing to a volume is not supported
    cl::Kernel* kernel_;

};

}

#endif // IVW_VOLUME_MAX_OPERATION_CL_H


