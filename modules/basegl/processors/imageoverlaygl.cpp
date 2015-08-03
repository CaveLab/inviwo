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

#include "imageoverlaygl.h"
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/interaction/events/gestureevent.h>
#include <inviwo/core/interaction/events/touchevent.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <modules/opengl/textureutils.h>
#include <modules/opengl/openglutils.h>

namespace inviwo {

ProcessorClassIdentifier(ImageOverlayGL, "org.inviwo.ImageOverlayGL");
ProcessorDisplayName(ImageOverlayGL, "Image Overlay");
ProcessorTags(ImageOverlayGL, Tags::GL);
ProcessorCategory(ImageOverlayGL, "Image Operation");
ProcessorCodeState(ImageOverlayGL, CODE_STATE_EXPERIMENTAL);

OverlayProperty::OverlayProperty(std::string identifier, std::string displayName,
                                 InvalidationLevel invalidationLevel, PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics)
    , size_("size", "Size", vec2(0.48f), vec2(0.0f), vec2(1.0f), vec2(0.01f))
    , pos_("position", "Position", vec2(0.25f), vec2(0.0f), vec2(1.0f), vec2(0.01f))
    , anchorPos_("anchor", "Anchor", vec2(0.0f), vec2(-1.0f), vec2(1.0f), vec2(0.01f))
    , blendMode_("blendMode", "Blending Mode") {
    blendMode_.addOption("replace", "Replace", BlendMode::Replace);
    blendMode_.addOption("over", "Blend", BlendMode::Over);
    blendMode_.setSelectedValue(BlendMode::Over);
    blendMode_.setCurrentStateAsDefault();

    addProperty(size_);
    addProperty(pos_);
    addProperty(anchorPos_);
    addProperty(blendMode_);
}

void OverlayProperty::updateViewport(vec2 destDim) {
    vec2 pos(pos_.get());
    vec2 size(size_.get());
    vec2 anchor(anchorPos_.get());

    // consider anchor position
    vec2 shift = 0.5f * size * (anchorPos_.get() + vec2(1.0f, 1.0f));
    pos.x -= shift.x;
    // negate y axis
    pos.y = 1.f - (pos.y + shift.y);

    pos *= destDim;
    size *= destDim;
    viewport_ = ivec4(pos.x, pos.y, size.x, size.y);
}

ImageOverlayGL::ImageOverlayGL()
    : Processor()
    , inport_("inport")
    , overlayPort_("overlay")
    , outport_("outport")
    , overlayInteraction_("overlayInteraction", "Overlay Interaction", false)
    , overlayProperty_("overlay", "Overlay")
    , shader_("img_texturequad.vert", "img_copy.frag")
    , viewManager_()
    , currentDim_(0u, 0u) {

    shader_.onReload([this]() { invalidate(INVALID_RESOURCES); });

    addPort(inport_);
    addPort(overlayPort_);

    overlayPort_.onConnect([this]() {
        ResizeEvent e(currentDim_);
        propagateResizeEvent(&e, &outport_);
    });

    overlayPort_.onDisconnect([this]() {
        ResizeEvent e(currentDim_);
        propagateResizeEvent(&e, &outport_);
    });

    addPort(outport_);

    addProperty(overlayInteraction_);
    addProperty(overlayProperty_);

    overlayProperty_.onChange(this, &ImageOverlayGL::onStatusChange);
}

ImageOverlayGL::~ImageOverlayGL() {}

void ImageOverlayGL::propagateEvent(Event* event) {
    invokeEvent(event);

    if (overlayInteraction_.get() && overlayPort_.isConnected()) {
        std::unique_ptr<Event> newEvent(viewManager_.registerEvent(event));
        int activeView = viewManager_.getActiveView();

        if (newEvent && activeView >= 0) {
            overlayPort_.propagateEvent(newEvent.get(), overlayPort_.getConnectedOutport());
            if (newEvent->hasBeenUsed()) event->markAsUsed();
        } else {
            inport_.propagateEvent(event);
        }

    } else {
        inport_.propagateEvent(event);
    }
}

bool ImageOverlayGL::isReady() const { return inport_.isReady(); }

bool ImageOverlayGL::propagateResizeEvent(ResizeEvent* resizeEvent, Outport* source) {
    updateViewports(resizeEvent->size(), true);

    if (inport_.isConnected()) {
        inport_.propagateResizeEvent(resizeEvent);
    }

    if (overlayPort_.isConnected()) {
        ResizeEvent e(uvec2(viewManager_[0].z, viewManager_[0].w));
        overlayPort_.propagateResizeEvent(
            &e, static_cast<ImageOutport*>(overlayPort_.getConnectedOutport()));
    }

    return false;
}

void ImageOverlayGL::onStatusChange() {
    ResizeEvent e(currentDim_);
    propagateResizeEvent(&e, &outport_);
}

void ImageOverlayGL::process() {
    utilgl::activateTargetAndCopySource(outport_, inport_, inviwo::COLOR_DEPTH_PICKING);

    if (overlayPort_.hasData()) {  // draw overlay
        utilgl::BlendModeState blendMode(static_cast<GLint>(overlayProperty_.blendMode_.get()),
                                         GL_ONE_MINUS_SRC_ALPHA);
        utilgl::DepthFuncState(GL_ALWAYS);

        TextureUnit colorUnit, depthUnit, pickingUnit;
        shader_.activate();
        shader_.setUniform("color_", colorUnit.getUnitNumber());
        shader_.setUniform("depth_", depthUnit.getUnitNumber());
        shader_.setUniform("picking_", pickingUnit.getUnitNumber());
        utilgl::bindTextures(overlayPort_, colorUnit, depthUnit, pickingUnit);

        ivec4 viewport = overlayProperty_.viewport_;
        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
        utilgl::singleDrawImagePlaneRect();

        // restore viewport
        ivec2 dim = outport_.getData()->getDimensions();
        glViewport(0, 0, dim.x, dim.y);
    }

    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

void ImageOverlayGL::updateViewports(ivec2 dim, bool force) {
    if (!force && (currentDim_ == dim)) return;  // no changes

    overlayProperty_.updateViewport(dim);
    viewManager_.clear();
    if (overlayPort_.isConnected()) {
        viewManager_.push_back(overlayProperty_.viewport_);
    }
    currentDim_ = dim;
}

}  // namespace
