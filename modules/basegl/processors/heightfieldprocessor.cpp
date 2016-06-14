/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2016 Inviwo Foundation
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

#include "heightfieldprocessor.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <inviwo/core/datastructures/light/directionallight.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>

namespace inviwo {

const ProcessorInfo HeightFieldProcessor::processorInfo_{
    "org.inviwo.HeightFieldRenderGL",  // Class identifier
    "Height Field Renderer",           // Display name
    "Heightfield",                     // Category
    CodeState::Experimental,           // Code state
    Tags::GL,                          // Tags
};
const ProcessorInfo HeightFieldProcessor::getProcessorInfo() const { return processorInfo_; }

HeightFieldProcessor::HeightFieldProcessor()
    : MeshRenderProcessorGL()
    , inportHeightfield_("heightfield", true)
    , inportTexture_("texture", true)
    , inportNormalMap_("normalmap", true)
    , heightScale_("heightScale", "Height Scale", 1.0f, 0.0f, 10.0f)
    , terrainShadingMode_("terrainShadingMode", "Terrain Shading") {
    shader_ = Shader("heightfield.vert", "heightfield.frag", false);
    shader_.onReload([this]() { invalidate(InvalidationLevel::InvalidResources); });

    inportHeightfield_.onChange(this, &HeightFieldProcessor::heightfieldChanged);
    addPort(inportHeightfield_);
    addPort(inportTexture_);
    addPort(inportNormalMap_);

    addProperty(heightScale_);

    terrainShadingMode_.addOption("shadingConstant", "Constant Color",
                                  HeightFieldShading::ConstantColor);
    terrainShadingMode_.addOption("shadingColorTex", "Color Texture",
                                  HeightFieldShading::ColorTexture);
    terrainShadingMode_.addOption("shadingHeightField", "Heightfield Texture",
                                  HeightFieldShading::HeightField);
    terrainShadingMode_.set(HeightFieldShading::ConstantColor);
    terrainShadingMode_.setCurrentStateAsDefault();
    addProperty(terrainShadingMode_);
}

HeightFieldProcessor::~HeightFieldProcessor() {}

void HeightFieldProcessor::process() {
    int terrainShadingMode = terrainShadingMode_.get();

    shader_.activate();

    // bind input textures
    TextureUnit heightFieldUnit, colorTexUnit, normalTexUnit;

    if (inportHeightfield_.isReady()) {
        utilgl::bindColorTexture(inportHeightfield_, heightFieldUnit.getEnum());
    } else if (terrainShadingMode == HeightFieldShading::HeightField) {
        // switch to flat shading since color texture is not available
        terrainShadingMode = HeightFieldShading::ConstantColor;
    }

    if (inportTexture_.isReady()) {
        utilgl::bindColorTexture(inportTexture_, colorTexUnit.getEnum());
    } else if (terrainShadingMode == HeightFieldShading::ColorTexture) {
        // switch to flat shading since heightfield texture is not available
        terrainShadingMode = HeightFieldShading::ConstantColor;
    }

    bool normalMapping = inportNormalMap_.isReady();
    if (normalMapping) {
        utilgl::bindColorTexture(inportNormalMap_, normalTexUnit.getEnum());
    }

    shader_.setUniform("inportHeightfield_", heightFieldUnit.getUnitNumber());
    shader_.setUniform("inportTexture_", colorTexUnit.getUnitNumber());
    shader_.setUniform("inportNormalMap_", normalTexUnit.getUnitNumber());
    shader_.setUniform("terrainShadingMode_", terrainShadingMode);
    shader_.setUniform("normalMapping_", (normalMapping ? 1 : 0));
    shader_.setUniform("heightScale_", heightScale_.get());

    // render mesh
    MeshRenderProcessorGL::process();
    TextureUnit::setZeroUnit();
}

void HeightFieldProcessor::heightfieldChanged() {
    if (!inportHeightfield_.isConnected()) return;

    // std::ostringstream str;
    // const Image *img = inportHeightfield_.getData();
    // const DataFormatBase* format = img->getDataFormat();

    // str << "Heightfield Port Properties:"
    //    << "\ndim: " << glm::to_string(img->getDimensions())
    //    << "\nType: " << img->getImageType()
    //    << "\nNum Color Layers: " << img->getNumberOfColorLayers()
    //    << std::endl << std::endl
    //    << "Format:"
    //    << "\nName: " << format->getString()
    //    << "\nComponents: " << format->getComponents()
    //    ;

    // LogInfo(str.str());
}

} // namespace

