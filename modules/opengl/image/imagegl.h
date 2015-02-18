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

#ifndef IVW_IMAGEGL_H
#define IVW_IMAGEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/glwrap/bufferobjectarray.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class Shader;
class IVW_MODULE_OPENGL_API ImageGL : public ImageRepresentation {

public:
    ImageGL();
    ImageGL(const ImageGL& rhs);
    virtual ~ImageGL();

    void initialize();
    void deinitialize();
    virtual ImageGL* clone() const;

    void reAttachAllLayers(ImageType type = ALL_LAYERS);

    void activateBuffer(ImageType type = ALL_LAYERS);
    void deactivateBuffer();

    virtual bool copyAndResizeRepresentation(DataRepresentation*) const;
    bool updateFrom(const ImageGL*);

    FrameBufferObject* getFBO();
    const FrameBufferObject* getFBO() const;

    LayerGL* getLayerGL(LayerType, size_t idx = 0);
    const LayerGL* getLayerGL(LayerType, size_t idx = 0) const;

    LayerGL* getColorLayerGL(size_t idx = 0);
    LayerGL* getDepthLayerGL();
    LayerGL* getPickingLayerGL();

    const LayerGL* getColorLayerGL(size_t idx = 0) const;
    const LayerGL* getDepthLayerGL() const;
    const LayerGL* getPickingLayerGL() const;

    GLenum getPickingAttachmentID() const;

    void updateExistingLayers() const;
    void renderImagePlaneRect() const;

protected:
    virtual void update(bool editable);

private:
    std::vector<LayerGL*> colorLayersGL_; //< non-owning reference
    LayerGL* depthLayerGL_;               //< non-owning reference  
    LayerGL* pickingLayerGL_;             //< non-owning reference

    mutable BufferObjectArray* rectArray_;
    FrameBufferObject* frameBufferObject_;
    GLenum pickingAttachmentID_;
    Shader* shader_;

    bool initialized_;
};

} // namespace

#endif // IVW_IMAGEGL_H
