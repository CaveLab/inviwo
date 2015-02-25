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

#include "drawpoints.h"
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>

namespace inviwo {

ProcessorClassIdentifier(DrawPoints, "org.inviwo.DrawPoints");
ProcessorDisplayName(DrawPoints,  "Draw Points");
ProcessorTags(DrawPoints, Tags::GL);
ProcessorCategory(DrawPoints, "Drawing");
ProcessorCodeState(DrawPoints, CODE_STATE_STABLE);

DrawPoints::DrawPoints()
    : CompositeProcessorGL()
    , inport_("image.inport")
    , outport_("image.outport")
    , pointSize_("pointSize", "Point Size", 5, 1, 10)
    , pointColor_("pointColor", "Point Color", vec4(1.f))
    , clearButton_("clearButton", "Clear Drawing")
    , mouseDraw_("mouseDraw", "Draw Point",
                 new MouseEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_CTRL,
                                MouseEvent::MOUSE_STATE_ANY),
                 new Action(this, &DrawPoints::eventDraw))
    , keyEnableDraw_(
          "keyEnableDraw", "Enable Draw",
          new KeyboardEvent('D', InteractionEvent::MODIFIER_CTRL, KeyboardEvent::KEY_STATE_ANY),
          new Action(this, &DrawPoints::eventEnableDraw))
    , points_(nullptr)
    , pointRenderer_(nullptr)
    , pointShader_(nullptr)
    , drawModeEnabled_(false) {
    addPort(inport_);
    addPort(outport_);

    addProperty(pointSize_);
    pointColor_.setSemantics(PropertySemantics::Color);
    addProperty(pointColor_);
    clearButton_.onChange(this, &DrawPoints::clearPoints);
    addProperty(clearButton_);

    addProperty(mouseDraw_);
    addProperty(keyEnableDraw_);
}

DrawPoints::~DrawPoints() {}

void DrawPoints::initialize() {
    CompositeProcessorGL::initialize();
    pointShader_ = new Shader("img_color.frag");
    points_ = new Mesh(GeometryEnums::POINTS, GeometryEnums::NONE);
    points_->addAttribute(new Position2dBuffer());
    pointRenderer_ = new MeshRenderer(points_);
}

void DrawPoints::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete pointShader_;
    pointShader_ = nullptr;
    delete pointRenderer_;
    pointRenderer_ = nullptr;
    delete points_;
    pointRenderer_ = nullptr;
}

void DrawPoints::process() {
    utilgl::activateTargetAndCopySource(outport_, inport_, COLOR_ONLY);
    glPointSize(static_cast<float>(pointSize_.get()));
    pointShader_->activate();
    pointShader_->setUniform("color_", pointColor_.get());
    pointRenderer_->render();
    pointShader_->deactivate();
    glPointSize(1.f);
    utilgl::deactivateCurrentTarget();
    compositePortsToOutport(outport_, COLOR_ONLY, inport_);
}

void DrawPoints::addPoint(vec2 p) {
    if (points_)
        points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawPoints::clearPoints() {
    if (points_)
        points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

void DrawPoints::eventDraw(Event* event){
    if (!drawModeEnabled_)
        return;

    MouseEvent* mouseEvent = static_cast<MouseEvent*>(event);
    vec2 point = mouseEvent->posNormalized();
    point *= 2.f;
    point -= 1.f;
    point.y = -point.y;
    addPoint(point);
    invalidate(INVALID_OUTPUT);
}

void DrawPoints::eventEnableDraw(Event* event){
    KeyboardEvent* keyEvent = static_cast<KeyboardEvent*>(event);
    drawModeEnabled_ = (keyEvent->state() != KeyboardEvent::KEY_STATE_RELEASE);
}

} // inviwo namespace
