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

#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

struct CanRenderGeometry {
public:
    CanRenderGeometry(const Geometry* geom): geom_(geom) {};
    bool operator()(const GeometryRenderer* renderer)
    { return renderer->canRender(geom_); }
private:
    const Geometry* geom_;
};

GeometryRendererFactory::GeometryRendererFactory() {
}

void GeometryRendererFactory::registerObject(GeometryRenderer* renderer) {
    renderers_.insert(renderer);
}

GeometryRenderer* GeometryRendererFactory::create(const Geometry* geom) const {
    std::set<GeometryRenderer*>::const_iterator it = std::find_if(renderers_.begin(), renderers_.end(), CanRenderGeometry(geom));

    if (it != renderers_.end())
        return (*it)->create(geom);
    else
        return NULL;
};

} // namespace

