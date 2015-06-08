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

#include <inviwo/core/interaction/cameratrackball.h>
#include <inviwo/core/datastructures/camera.h>
//#include <glm/gtx/decomposition.hpp>
// Dependencies
#include <glm/mat4x4.hpp>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace inviwo {
PropertyClassIdentifier(CameraTrackball, "org.inviwo.Trackball");

CameraTrackball::CameraTrackball(CameraProperty* cameraProp)
    : Trackball(cameraProp, &(cameraProp->get()))
    , cameraProp_(cameraProp)
{
}

CameraTrackball::~CameraTrackball() {}

CameraTrackball* CameraTrackball::clone() const {
    return new CameraTrackball(*this);
}

//
//void CameraTrackball::onLookFromChanged() {
//    // Don't allow zooming such that the lookAt point is further away then the far plane.
//    float maxDistance = cameraProp_->getFarPlaneDist() - (cameraProp_->getFarPlaneDist()*0.3f);
//    float dist = glm::distance(cameraProp_->getLookTo(), cameraProp_->getLookFrom());
//    if (maxDistance < dist)
//        cameraProp_->setLookFrom(
//            cameraProp_->getLookTo() +
//            (glm::normalize(cameraProp_->getLookFrom() - cameraProp_->getLookTo()) * maxDistance));
//
//    //cameraProp_->updateViewMatrix();
//}

}  // namespace