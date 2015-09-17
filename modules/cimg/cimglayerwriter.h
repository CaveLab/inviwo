/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015 Inviwo Foundation
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

#ifndef IVW_CIMGLAYERWRITER_H
#define IVW_CIMGLAYERWRITER_H

#include <modules/cimg/cimgmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/datawriter.h>

namespace inviwo {

class Layer;

/** \brief Writer for Images files
 *
 */
class IVW_MODULE_CIMG_API CImgLayerWriter : public DataWriterType<Layer> {
public:
    CImgLayerWriter();
    CImgLayerWriter(const CImgLayerWriter& rhs);
    CImgLayerWriter& operator=(const CImgLayerWriter& that);
    virtual CImgLayerWriter* clone() const;
    virtual ~CImgLayerWriter() {};

    virtual void writeData(const Layer* data, const std::string filePath) const;
    virtual std::vector<unsigned char>* writeDataToBuffer(const Layer* data, std::string& type) const;
    virtual bool writeDataToRepresentation(const DataRepresentation* src, DataRepresentation* dst) const;
};

} // namespace

#endif // IVW_CIMGLAYERWRITER_H
