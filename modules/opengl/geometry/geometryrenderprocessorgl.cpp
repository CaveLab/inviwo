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

#include "geometryrenderprocessorgl.h"
#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/geometrydrawerfactory.h>
#include <modules/opengl/rendering/meshdrawer.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <modules/opengl/shaderutils.h>
#include <modules/opengl/openglutils.h>

#include <limits>

namespace inviwo {

ProcessorClassIdentifier(GeometryRenderProcessorGL, "org.inviwo.GeometryRenderGL");
ProcessorDisplayName(GeometryRenderProcessorGL, "Geometry Renderer");
ProcessorTags(GeometryRenderProcessorGL, Tags::GL);
ProcessorCategory(GeometryRenderProcessorGL, "Geometry Rendering");
ProcessorCodeState(GeometryRenderProcessorGL, CODE_STATE_STABLE);

GeometryRenderProcessorGL::GeometryRenderProcessorGL()
    : Processor()
    , inport_("geometry.inport")
    , outport_("image.outport")
    , camera_("camera", "Camera")
    , centerViewOnGeometry_("centerView", "Center view on geometry")
    , setNearFarPlane_("setNearFarPlane","Calculate Near and Far Plane")
    , resetViewParams_("resetView", "Reset Camera")
    , trackball_(&camera_)
    , geomProperties_("geometry", "Geometry Rendering Properties")
    , cullFace_("cullFace", "Cull Face")
    , polygonMode_("polygonMode", "Polygon Mode")
    , renderPointSize_("renderPointSize", "Point Size", 1.0f, 0.001f, 15.0f, 0.001f)
    , renderLineWidth_("renderLineWidth", "Line Width", 1.0f, 0.001f, 15.0f, 0.001f)
    , lightingProperty_("lighting", "Lighting", &camera_)
    , layers_("layers", "Layers")
    , colorLayer_("colorLayer", "Color", true, INVALID_RESOURCES)
    , texCoordLayer_("texCoordLayer", "Texture Coordinates", false, INVALID_RESOURCES)
    , normalsLayer_("normalsLayer", "Normals (World Space)", false, INVALID_RESOURCES)
    , veiwNormalsLayer_("veiwNormalsLayer", "Normals (View space)", false, INVALID_RESOURCES)
    , shader_("geometryrendering.vert", "geometryrendering.frag", false) {

    addPort(inport_);
    addPort(outport_);
    addProperty(camera_);
    centerViewOnGeometry_.onChange(this, &GeometryRenderProcessorGL::centerViewOnGeometry);
    addProperty(centerViewOnGeometry_);
    setNearFarPlane_.onChange(this, &GeometryRenderProcessorGL::setNearFarPlane);
    addProperty(setNearFarPlane_);
    resetViewParams_.onChange([this]() { camera_.resetCamera(); });    addProperty(resetViewParams_);
    outport_.addResizeEventListener(&camera_);
    inport_.onChange(this, &GeometryRenderProcessorGL::updateDrawers);

    cullFace_.addOption("culldisable", "Disable", GL_NONE);
    cullFace_.addOption("cullfront", "Front", GL_FRONT);
    cullFace_.addOption("cullback", "Back", GL_BACK);
    cullFace_.addOption("cullfrontback", "Front & Back", GL_FRONT_AND_BACK);
    cullFace_.set(GL_NONE);

    polygonMode_.addOption("polypoint", "Points", GL_POINT);
    polygonMode_.addOption("polyline", "Lines", GL_LINE);
    polygonMode_.addOption("polyfill", "Fill", GL_FILL);
    polygonMode_.set(GL_FILL);
    polygonMode_.onChange(this, &GeometryRenderProcessorGL::changeRenderMode);

    geomProperties_.addProperty(cullFace_);
    geomProperties_.addProperty(polygonMode_);
    geomProperties_.addProperty(renderPointSize_);
    geomProperties_.addProperty(renderLineWidth_);

    float lineWidthRange[2];
    float increment;
    glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &increment);
    renderLineWidth_.setMinValue(lineWidthRange[0]);
    renderLineWidth_.setMaxValue(lineWidthRange[1]);
    renderLineWidth_.setIncrement(increment);

    renderLineWidth_.setVisible(false);
    renderPointSize_.setVisible(false);

    addProperty(geomProperties_);
    addProperty(lightingProperty_);
    addProperty(trackball_);

    addProperty(layers_);
    layers_.addProperty(colorLayer_);
    layers_.addProperty(texCoordLayer_);
    layers_.addProperty(normalsLayer_);
    layers_.addProperty(veiwNormalsLayer_);

    setAllPropertiesCurrentStateAsDefault();
}

GeometryRenderProcessorGL::~GeometryRenderProcessorGL() {}

void GeometryRenderProcessorGL::initializeResources() {
    // shading defines
    utilgl::addShaderDefines(&shader_, lightingProperty_);
    int layerID = 0;

    if (colorLayer_.get()) {
        shader_.getFragmentShaderObject()->addShaderDefine("COLOR_LAYER");
        layerID++;
    } else {
        shader_.getFragmentShaderObject()->removeShaderDefine("COLOR_LAYER");
    }

    if (texCoordLayer_.get()) {
        shader_.getFragmentShaderObject()->addShaderDefine("TEXCOORD_LAYER");
        shader_.getFragmentShaderObject()->addOutDeclaration("tex_coord_out");
        layerID++;
    } else {
        shader_.getFragmentShaderObject()->removeShaderDefine("TEXCOORD_LAYER");
    }

    if (normalsLayer_.get()) {
        shader_.getFragmentShaderObject()->addShaderDefine("NORMALS_LAYER");
        shader_.getFragmentShaderObject()->addOutDeclaration("normals_out");
        layerID++;
    } else {
        shader_.getFragmentShaderObject()->removeShaderDefine("NORMALS_LAYER");
    }

    if (veiwNormalsLayer_.get()) {
        shader_.getFragmentShaderObject()->addShaderDefine("VIEW_NORMALS_LAYER");
        shader_.getFragmentShaderObject()->addOutDeclaration("view_normals_out");
        layerID++;
    } else {
        shader_.getFragmentShaderObject()->removeShaderDefine("VIEW_NORMALS_LAYER");
    }

    for (size_t i = outport_.getData()->getNumberOfColorLayers(); i < layerID; i++) {
        outport_.getData()->addColorLayer(outport_.getData()->getColorLayer(0)->clone());
    }

    shader_.build();
}

void GeometryRenderProcessorGL::changeRenderMode() {
    switch (polygonMode_.get()) {
        case GL_FILL: {
            renderLineWidth_.setVisible(false);
            renderPointSize_.setVisible(false);
            break;
        }
        case GL_LINE: {
            renderLineWidth_.setVisible(true);
            renderPointSize_.setVisible(false);
            break;
        }
        case GL_POINT: {
            renderLineWidth_.setVisible(false);
            renderPointSize_.setVisible(true);
            break;
        }
    }
}

void GeometryRenderProcessorGL::process() {
    if (!inport_.hasData()) return;

    utilgl::activateAndClearTarget(outport_);
    shader_.activate();

    utilgl::setShaderUniforms(&shader_, camera_, "camera_");
    utilgl::setShaderUniforms(&shader_, lightingProperty_, "light_");

    utilgl::GlBoolState depthTest(GL_DEPTH_TEST, true);
    utilgl::CullFaceState culling(cullFace_.get());
    utilgl::PolygonModeState polygon(polygonMode_.get(), renderLineWidth_, renderPointSize_);

    for (auto& drawer : drawers_) {
        utilgl::setShaderUniforms(&shader_, *(drawer.second->getGeometry()), "geometry_");
        drawer.second->draw();
    }

    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

void GeometryRenderProcessorGL::centerViewOnGeometry() {
    std::vector<const Geometry*> geometries = inport_.getVectorData();
    if (geometries.empty()) return;

    vec3 worldMin(std::numeric_limits<float>::max());
    vec3 worldMax(std::numeric_limits<float>::lowest());

    for (auto geom : geometries) {
        const Mesh* mesh = dynamic_cast<const Mesh*>(geom);
        if (mesh) {
            vec3 minPos(std::numeric_limits<float>::max());
            vec3 maxPos(std::numeric_limits<float>::lowest());
            for (auto buff : mesh->getBuffers()) {
                if (buff->getBufferType() == POSITION_ATTRIB) {
                    const Position3dBufferRAM* posbuff = dynamic_cast<const Position3dBufferRAM*>(
                        buff->getRepresentation<BufferRAM>());

                    if (posbuff) {
                        const std::vector<vec3>* pos = posbuff->getDataContainer();
                        for (const auto& p : *pos) {
                            minPos = glm::min(minPos, p);
                            maxPos = glm::max(maxPos, p);
                        }
                    }
                }
            }

            mat4 trans = mesh->getCoordinateTransformer().getDataToWorldMatrix();

            worldMin = glm::min(worldMin, (trans * vec4(minPos, 1.f)).xyz());
            worldMax = glm::max(worldMax, (trans * vec4(maxPos, 1.f)).xyz());
        }
    }
    camera_.setLook(camera_.getLookFrom(), 0.5f * (worldMin + worldMax), camera_.getLookUp());
}

void GeometryRenderProcessorGL::setNearFarPlane() {
    std::vector<const Geometry*> geometries = inport_.getVectorData();
    if (geometries.empty()) return;

    const Mesh* geom = dynamic_cast<const Mesh*>(geometries[0]);

    if (geom == nullptr) {
        return;
    }

    const Position3dBufferRAM* posBuffer = dynamic_cast<const Position3dBufferRAM*>(
        geom->getAttributes(0)->getRepresentation<BufferRAM>());

    if (posBuffer == nullptr) {
        return;
    }

    const std::vector<vec3>* pos = posBuffer->getDataContainer();

    if (pos->empty()) {
        return;
    }

    float nearDist, farDist;
    nearDist = std::numeric_limits<float>::infinity();
    farDist = 0;
    vec3 nearPos, farPos;
    vec3 camPos = (geom->getCoordinateTransformer().getWorldToModelMatrix() * vec4(camera_.getLookFrom(), 1.0)).xyz();
    for (auto& po : *pos) {
        auto d = glm::distance2(po, camPos);
        if (d < nearDist){
            nearDist = d;
            nearPos = po;
        }
        if (d > farDist){
            farDist = d;
            farPos = po;
        }
    }

    mat4 m =  camera_.viewMatrix() * geom->getCoordinateTransformer().getModelToWorldMatrix();



    camera_.setNearPlaneDist(std::max(0.0f, 0.99f*std::abs((m * vec4(nearPos, 1.0f)).z)));
    camera_.setFarPlaneDist(std::max(0.0f, 1.01f*std::abs((m * vec4(farPos, 1.0f)).z)));
    
}

void GeometryRenderProcessorGL::updateDrawers() {
    auto changed = inport_.getChangedOutports();
    DrawerMap temp;
    std::swap(temp, drawers_);

    std::map<const Outport*, std::vector<const Geometry*>> data;
    for (auto& elem : inport_.getSourceVectorData()) {
        data[elem.first].push_back(elem.second);
    }

    for (auto elem : data) {
        auto ibegin = temp.lower_bound(elem.first);
        auto iend = temp.upper_bound(elem.first);

        if (util::contains(changed, elem.first) || ibegin == temp.end() ||
            elem.second.size() != std::distance(ibegin, iend)) {  // data is changed or new.

            for (auto geo : elem.second) {
                GeometryDrawer* renderer = GeometryDrawerFactory::getPtr()->create(geo);
                if (renderer) {
                    drawers_.emplace(
                        std::make_pair(elem.first, std::unique_ptr<GeometryDrawer>(renderer)));
                }
            }
        } else {  // reuse the old data.
            drawers_.insert(std::make_move_iterator(ibegin), std::make_move_iterator(iend));
        }
    }
}

}  // namespace
