/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2016 Inviwo Foundation
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

#include <modules/vectorfieldvisualization/vectorfieldvisualizationmodule.h>
#include <modules/vectorfieldvisualization/processors/datageneration/rbfvectorfieldgenerator2d.h>
#include <modules/vectorfieldvisualization/processors/datageneration/rbfvectorfieldgenerator3d.h>
#include <modules/vectorfieldvisualization/processors/datageneration/seedpointgenerator.h>

#include <modules/vectorfieldvisualization/processors/3d/streamlines.h>
#include <modules/vectorfieldvisualization/processors/3d/pathlines.h>
#include <modules/vectorfieldvisualization/processors/3d/streamribbons.h>

#include <modules/vectorfieldvisualization/ports/seedpointsport.h>

#include <modules/vectorfieldvisualization/properties/streamlineproperties.h>
#include <modules/vectorfieldvisualization/properties/pathlineproperties.h>

namespace inviwo {

VectorFieldVisualizationModule::VectorFieldVisualizationModule(InviwoApplication* app)
    : InviwoModule(app, "VectorFieldVisualization") {
    registerProcessor<RBFVectorFieldGenerator2D>();
    registerProcessor<RBFVectorFieldGenerator3D>();
    registerProcessor<SeedPointGenerator>();

    registerProcessor<StreamLines>();
    registerProcessor<PathLines>();
    registerProcessor<StreamRibbons>();

    registerPort < SeedPointsOutport>("SeedPointsOutport");
    registerPort < SeedPointsInport>("SeedPointsInport");

    registerProperty<StreamLineProperties>();
    registerProperty<PathLineProperties>();
}


}  // namespace
