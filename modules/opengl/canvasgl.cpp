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

#include "canvasgl.h"
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/shader/shader.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/buffer/bufferobjectarray.h>
#include <modules/opengl/buffer/buffergl.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/rendering/meshdrawergl.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

const MeshGL* CanvasGL::screenAlignedRectGL_ = nullptr;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions)
    , imageGL_(nullptr)
    , image_(nullptr)
    , rectArray_(nullptr)
    , layerType_(COLOR_LAYER)
    , shader_(nullptr)
    , noiseShader_(nullptr)
    , singleChannel_(false)
    , previousRenderedLayerIdx_(0) 
{}

CanvasGL::~CanvasGL() {
    deinitialize();
}

void CanvasGL::initialize() {
    NO_SUPPORTED_GL_THEN_RETURN

    defaultGLState();
    shader_ = new Shader("img_texturequad.vert", "img_texturequad.frag");
    LGL_ERROR;
    noiseShader_ = new Shader("img_texturequad.vert", "img_noise.frag");
    LGL_ERROR;
    Canvas::initialize();
}

void CanvasGL::initializeSquare() {
    NO_SUPPORTED_GL_THEN_RETURN

    const Mesh* screenAlignedRectMesh = dynamic_cast<const Mesh*>(screenAlignedRect_);

    if (screenAlignedRectMesh) {
        screenAlignedRectGL_ = screenAlignedRectMesh->getRepresentation<MeshGL>();
        LGL_ERROR;
    }
}

void CanvasGL::deinitialize() {
    delete shader_;
    shader_ = nullptr;
    delete noiseShader_;
    noiseShader_ = nullptr;
    delete rectArray_;
    rectArray_ = nullptr;
    image_ = nullptr;
    imageGL_ = nullptr;
    Canvas::deinitialize();
}

void CanvasGL::defaultGLState(){
    NO_SUPPORTED_GL_THEN_RETURN
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    LGL_ERROR;
}

void CanvasGL::activate() {}

void CanvasGL::render(const Image* image, LayerType layerType, size_t idx) {
    image_ = image;
    layerType_ = layerType;
    pickingContainer_->setPickingSource(image_);    
    if (image_) {
        imageGL_ = image_->getRepresentation<ImageGL>();
        if (imageGL_ && imageGL_->getLayerGL(layerType_, idx)) {
            checkChannels(imageGL_->getLayerGL(layerType_, idx)->getDataFormat()->getComponents());
        } else {
            checkChannels(image_->getDataFormat()->getComponents());
        }
        renderLayer(idx);

        // Faster async download of textures sampled on interaction
        if (imageGL_->getDepthLayerGL())
            imageGL_->getDepthLayerGL()->getTexture()->downloadToPBO();
        if (pickingContainer_->pickingEnabled() && imageGL_->getPickingLayerGL())
            imageGL_->getPickingLayerGL()->getTexture()->downloadToPBO();
    } else {
        imageGL_ = nullptr;
        renderNoise();
    }
}

void CanvasGL::resize(uvec2 size) {
    imageGL_ = nullptr;
    pickingContainer_->setPickingSource(nullptr);
    Canvas::resize(size);
}

void CanvasGL::glSwapBuffers() {}

void CanvasGL::update() {
    renderLayer(previousRenderedLayerIdx_);
}

void CanvasGL::attachImagePlanRect(BufferObjectArray* arrayObject) {
    if (arrayObject) {
        arrayObject->bind();
        arrayObject->attachBufferObject(
            screenAlignedRectGL_->getBufferGL(0)->getBufferObject().get(),
            POSITION_ATTRIB);
        arrayObject->attachBufferObject(
            screenAlignedRectGL_->getBufferGL(1)->getBufferObject().get(),
            TEXCOORD_ATTRIB);
        arrayObject->unbind();
    }
}

void CanvasGL::singleDrawImagePlaneRect() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CanvasGL::multiDrawImagePlaneRect(int instances) {
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
}

void CanvasGL::renderLayer(size_t idx) {
    previousRenderedLayerIdx_ = idx;
    if (imageGL_) {
        const LayerGL* layerGL = imageGL_->getLayerGL(layerType_,idx);
        if (layerGL) {
            TextureUnit textureUnit;
            layerGL->bindTexture(textureUnit.getEnum());
            renderTexture(textureUnit.getUnitNumber());
            layerGL->unbindTexture();
            return;
        } else {
            renderNoise();
        }
    }
    if (!image_) renderNoise();
}

void CanvasGL::renderNoise() {
    if (!noiseShader_)
        return;
    activate();
    glViewport(0, 0, getScreenDimensions().x, getScreenDimensions().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    noiseShader_->activate();
    drawRect();
    noiseShader_->deactivate();
    glSwapBuffers();
    activateDefaultRenderContext();
}

void CanvasGL::renderTexture(int unitNumber) {
    if (!shader_)
        return;
    activate();
    glViewport(0, 0, getScreenDimensions().x, getScreenDimensions().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    drawRect();
    shader_->deactivate();
    glDisable(GL_BLEND);
    glSwapBuffers();
    activateDefaultRenderContext();
}

void CanvasGL::drawRect() {
    delete rectArray_;
    rectArray_ = new BufferObjectArray();
    rectArray_->bind();
    rectArray_->attachBufferObject(
        screenAlignedRectGL_->getBufferGL(0)->getBufferObject().get(),
        POSITION_ATTRIB);
    rectArray_->attachBufferObject(
        screenAlignedRectGL_->getBufferGL(1)->getBufferObject().get(),
        TEXCOORD_ATTRIB);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    rectArray_->unbind();
}

void CanvasGL::checkChannels(std::size_t channels) {
    if (!singleChannel_ && channels == 1) {
        shader_->getFragmentShaderObject()->addShaderDefine("SINGLE_CHANNEL");
        shader_->getFragmentShaderObject()->build();
        shader_->link();
        singleChannel_ = true;
    } else if (singleChannel_ && channels == 4) {
        shader_->getFragmentShaderObject()->removeShaderDefine("SINGLE_CHANNEL");
        shader_->getFragmentShaderObject()->build();
        shader_->link();
        singleChannel_ = false;
    }
}

const LayerRAM* CanvasGL::getDepthLayerRAM() const{
    if (image_) {
        const Layer* depthLayer = image_->getDepthLayer();
        if (depthLayer) {
            return depthLayer->getRepresentation<LayerRAM>();
        }
        else
            return nullptr;
    }
    else
        return nullptr;
}

double CanvasGL::getDepthValueAtCoord(ivec2 coord, const LayerRAM* depthLayerRAM) const{
    const LayerRAM* dlr = depthLayerRAM;
    if (!dlr)
        dlr = getDepthLayerRAM();

    if (dlr) {
        auto screenDims(getScreenDimensions());
        auto depthDims = dlr->getDimensions();
        coord = glm::max(coord, ivec2(0));

        double depthScreenRatioX = static_cast<double>(depthDims.x) / static_cast<double>(screenDims.x);
        double depthScreenRatioY = static_cast<double>(depthDims.y) / static_cast<double>(screenDims.y);
        size2_t coordDepth;
        coordDepth.x = static_cast<size_t>(depthScreenRatioX*static_cast<double>(coord.x));
        coordDepth.y = static_cast<size_t>(depthScreenRatioY*static_cast<double>(coord.y));
        depthDims -= 1;
        coordDepth = glm::min(coordDepth, depthDims);

        double depthValue = dlr->getValueAsSingleDouble(coordDepth);

        // Convert to normalized device coordinates
        return 2.0*depthValue - 1.0;
    }
    else
        return 1.0;
}

void CanvasGL::enableDrawImagePlaneRect() {
        screenAlignedRectGL_->enable();
}

void CanvasGL::disableDrawImagePlaneRect() {
    screenAlignedRectGL_->disable();
}

void  CanvasGL::setProcessorWidgetOwner(ProcessorWidget* widget) {
    //Clear internal state
    image_ = nullptr;
    imageGL_ = nullptr;
    pickingContainer_->setPickingSource(nullptr);
    Canvas::setProcessorWidgetOwner(widget);
}

}  // namespace
