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

#ifndef IVW_FREEIMAGEREADER_H
#define IVW_FREEIMAGEREADER_H

#include <modules/freeimage/freeimagemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/datareader.h>

namespace inviwo {

class Layer;
class LayerDisk;

/** \brief Reader for Images files
 *
 */
class IVW_MODULE_FREEIMAGE_API FreeImageReader : public DataReaderType<Layer> {
public:
    FreeImageReader();
    FreeImageReader(const FreeImageReader& rhs);
    FreeImageReader& operator=(const FreeImageReader& that);
    virtual FreeImageReader* clone() const;
    virtual ~FreeImageReader() {}

    virtual Layer* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;
};

}  // namespace

#endif  // IVW_FREEIMAGEREADER_H
