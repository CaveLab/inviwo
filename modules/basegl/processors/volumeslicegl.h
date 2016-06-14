/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2016 Inviwo Foundation
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

#ifndef IVW_VOLUMESLICEGL_H
#define IVW_VOLUMESLICEGL_H

#include <modules/basegl/baseglmoduledefine.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometrytype.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <modules/opengl/shader/shader.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class Mesh;

/** \docpage{org.inviwo.VolumeSliceGL, Volume Slice}
 * ![](org.inviwo.VolumeSliceGL.png?classIdentifier=org.inviwo.VolumeSliceGL)
 *
 * ...
 * 
 * ### Inports
 *   * __volume__ ...
 * 
 * ### Outports
 *   * __outport__ ...
 * 
 * ### Properties
 *   * __mouseShiftSlice___ ...
 *   * __Z Volume Position__ ...
 *   * __Position Selection__ ...
 *   * __Handle interaction events__ ...
 *   * __Scale__ ...
 *   * __Enable Transfer Function__ ...
 *   * __Indicator Color__ ...
 *   * __Horizontal Flip__ ...
 *   * __Alpha Offset__ ...
 *   * __Gesture Slice Shift__ ...
 *   * __X Volume Position__ ...
 *   * __Y Volume Position__ ...
 *   * __Angle__ ...
 *   * __Fill Color__ ...
 *   * __Volume Texture Wrapping__ ...
 *   * __Mouse Set Marker__ ...
 *   * __stepSliceDown___ ...
 *   * __Vertical Flip__ ...
 *   * __stepSliceUp___ ...
 *   * __Transformations__ ...
 *   * __Plane Normal__ ...
 *   * __Show Position Indicator__ ...
 *   * __Enable Picking__ ...
 *   * __Transfer function__ ...
 *   * __Rotation (ccw)__ ...
 *   * __World Position__ ...
 *   * __Transfer Function__ ...
 *   * __Slice along axis__ ...
 *   * __Plane Position__ ...
 *
 */
class IVW_MODULE_BASEGL_API VolumeSliceGL : public Processor {
public:
    VolumeSliceGL();
    virtual ~VolumeSliceGL();

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

    virtual void initializeResources() override;

    // Overridden to be able to turn off interaction events.
    virtual void invokeEvent(Event*) override;

    bool positionModeEnabled() const { return posPicking_.get(); }

    // override to do member renaming.
    virtual void deserialize(Deserializer& d) override;

protected:
    virtual void process() override;

    void shiftSlice(int);

    void modeChange();
    void planeSettingsChanged();
    void updateMaxSliceNumber();

    void renderPositionIndicator();
    void updateIndicatorMesh();

    // updates the selected position, pos is given in normalized viewport coordinates, i.e. [0,1]
    void setVolPosFromScreenPos(vec2 pos);
    vec2 getScreenPosFromVolPos();

    void invalidateMesh();

    void sliceChange();
    void positionChange();
    void rotationModeChange();

private:
    void eventShiftSlice(Event*);
    void eventSetMarker(Event*);
    void eventStepSliceUp(Event*);
    void eventStepSliceDown(Event*);
    void eventGestureShiftSlice(Event*);

    VolumeInport inport_;
    ImageOutport outport_;
    Shader shader_;
    Shader indicatorShader_;

    CompositeProperty trafoGroup_;
    CompositeProperty pickGroup_;
    CompositeProperty tfGroup_;

    OptionPropertyInt sliceAlongAxis_;  // Axis enum
    IntProperty sliceX_;
    IntProperty sliceY_;
    IntProperty sliceZ_;

    FloatVec3Property worldPosition_;

    FloatVec3Property planeNormal_;
    FloatVec3Property planePosition_;
    FloatProperty imageScale_;
    OptionPropertyInt rotationAroundAxis_;  // Clockwise rotation around slice axis
    FloatProperty imageRotation_;
    BoolProperty flipHorizontal_;
    BoolProperty flipVertical_;
    OptionPropertyInt volumeWrapping_; 
    FloatVec4Property fillColor_;

    BoolProperty posPicking_;
    BoolProperty showIndicator_;
    FloatVec4Property indicatorColor_;

    BoolProperty tfMappingEnabled_;
    TransferFunctionProperty transferFunction_;
    FloatProperty tfAlphaOffset_;

    BoolProperty handleInteractionEvents_;

    EventProperty mouseShiftSlice_;
    EventProperty mouseSetMarker_;

    EventProperty stepSliceUp_;
    EventProperty stepSliceDown_;

    EventProperty gestureShiftSlice_;

    std::unique_ptr<Mesh> meshCrossHair_;
   
    bool meshDirty_;
    bool updating_;

    mat4 sliceRotation_;
    mat4 inverseSliceRotation_; // Used to calculate the slice "z position" from the plain point. 
    size3_t volumeDimensions_;
    mat4 texToWorld_;
};
}

#endif  // IVW_VOLUMESLICEGL_H
