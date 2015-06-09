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

#ifndef IVW_POINT_LIGHT_SOURCE_PROCESSOR_H
#define IVW_POINT_LIGHT_SOURCE_PROCESSOR_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/light/baselightsource.h>
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <modules/base/basemoduledefine.h>

namespace inviwo {

class PointLight;
class PointLightInteractionHandler;

class IVW_MODULE_BASE_API PointLightTrackball : public Trackball<PointLightInteractionHandler> {
public:
    InviwoPropertyInfo();

    PointLightTrackball(PointLightInteractionHandler* p);
    virtual ~PointLightTrackball() = default;
};

/*
* Enables light source to be placed relative to camera using middle mouse button or pan gesture with two fingers.
* Uses trackball interaction for all other types of interaction.
*/
class IVW_MODULE_BASE_API PointLightInteractionHandler : public InteractionHandler {
public:
    PointLightInteractionHandler(FloatVec3Property*, CameraProperty*);
    ~PointLightInteractionHandler(){};

    virtual std::string getClassIdentifier() const { return "org.inviwo.PointLightInteractionHandler"; }

    const PerspectiveCamera& getCamera() { return camera_->get(); }

    void invokeEvent(Event* event);
    void setHandleEventsOptions(int);

    /**
    * \brief Changes the direction of the light source, relative to the camera,
    * such that it acts as if it comes from the direction where the user clicked on the screen.
    *
    * Intersects a sphere covering the scene and places the light source
    * in the direction of the intersection point but at the same distance from the origin as before.
    * If the intersection is outside the sphere the light source will be placed perpendicular
    * to the camera at the same distance as before.
    *
    *
    * @param vec2 normalizedScreenCoord Coordinates in [0 1], where y coordinate is 0 at top of screen.
    */
    void setLightPosFromScreenCoords(const vec2& normalizedScreenCoord);

    // Update up vector when camera changes
    void onCameraChanged();

    // Necessary for trackball
    const vec3& getLookTo() const { return lookTo_; }
    const vec3& getLookFrom() const { return lightPosition_->get(); }
    const vec3& getLookUp() const { return lookUp_; }

    void setLookTo(vec3 lookTo) { lookTo_ = lookTo; }
    void setLookFrom(vec3 lookFrom) { lightPosition_->set(lookFrom); }
    void setLookUp(vec3 lookUp) { lookUp_ = lookUp; }

    const vec3 getLookFromMinValue() const { return lightPosition_->getMinValue(); }
    const vec3 getLookFromMaxValue() const { return lightPosition_->getMaxValue(); }

    const vec3 getLookToMinValue() const { return vec3(-std::numeric_limits < float >::max()); }
    const vec3 getLookToMaxValue() const { return vec3(std::numeric_limits < float >::max()); }

    void setLook(vec3 lookFrom, vec3 lookTo, vec3 lookUp) { lightPosition_->set(lookFrom); lookTo_ = lookTo; lookUp = lookUp; }
    void serialize(IvwSerializer& s) const;
    void deserialize(IvwDeserializer& d);

private:
    FloatVec3Property* lightPosition_;
    CameraProperty* camera_;
    vec3 lookUp_; ///< Necessary for trackball
    vec3 lookTo_; ///< Necessary for trackball
    PointLightTrackball trackball_;
    int interactionEventOption_;

};

/** \docpage{org.inviwo.Pointlightsource, Point light source}
 * ![](org.inviwo.Pointlightsource.png?classIdentifier=org.inviwo.Pointlightsource)
 *
 * ...
 * 
 * 
 * 
 * ### Properties
 *   * __Light power (%)__ ...
 *   * __Light radius__ ...
 *   * __Interaction Events__ ...
 *   * __Camera__ ...
 *   * __Light Parameters__ ...
 *   * __Color__ ...
 *   * __Light Source Position__ ...
 *   * __Enabled__ ...
 *
 */
class IVW_MODULE_BASE_API PointLightSourceProcessor : public Processor {
public:
    PointLightSourceProcessor();
    virtual ~PointLightSourceProcessor();

    InviwoProcessorInfo();

protected:
    virtual void process();

    void handleInteractionEventsChanged();



    /**
     * Update light source parameters. Transformation will be given in texture space.
     *
     * @param lightSource
     * @return
     */
    void updatePointLightSource(PointLight* lightSource);

private:
    DataOutport<LightSource> outport_;

    CompositeProperty lighting_;
    FloatProperty lightPowerProp_;
    FloatProperty lightSize_;
    FloatVec4Property lightDiffuse_;
    FloatVec3Property lightPosition_;
    BoolProperty lightEnabled_;
    CameraProperty camera_;
    OptionPropertyInt interactionEvents_;
    PointLightInteractionHandler* lightInteractionHandler_;
    PointLight* lightSource_;
};

} // namespace

#endif // IVW_POINT_LIGHT_SOURCE_PROCESSOR_H
