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

#ifndef IVW_TOUCHEVENT_H
#define IVW_TOUCHEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

class IVW_CORE_API TouchPoint : public IvwSerializable {
public:
    TouchPoint() {};
    TouchPoint(vec2 pos, vec2 posNormalized, vec2 prevPos, vec2 prevPosNormalized);
    virtual ~TouchPoint() {};
    /** 
     * \brief Retrieve position in screen coordinates [0 dim-1]^2
     * Coordinate system:
     *     (0,0)    --     (width-1,0)
     *       |                   |
     * (0,height-1) -- (width-1,height-1)
     * @return vec2 
     */
    vec2 getPos() const { return pos_; }
    void setPos(vec2 val) { pos_ = val; }
    /**
    * \brief Retrieve position normalized to the size of the screen [0 1]^2.
    * Coordinate system:
    * (0,0)--(1,0)
    *   |      |
    * (0,1)--(1,1)
    * @return vec2
    */
    vec2 getPosNormalized() const { return posNormalized_; }
    void setPosNormalized(vec2 val) { posNormalized_ = val; }
    /**
    * \brief Retrieve the previous event position in screen coordinates [0 dim-1]^2
    * Coordinate system:
    *     (0,0)    --     (width-1,0)
    *       |                   |
    * (0,height-1) -- (width-1,height-1)
    * @return vec2
    */
    vec2 getPrevPos() const { return prevPos_; }
    void setPrevPos(vec2 val) { prevPos_ = val; }
    /**
    * \brief Retrieve the previous position normalized to the size of the screen [0 1]^2.
    * Coordinate system:
    * (0,0)--(1,0)
    *   |      |
    * (0,1)--(1,1)
    * @return vec2
    */
    vec2 getPrevPosNormalized() const { return prevPosNormalized_; }
    void setPrevPosNormalized(vec2 val) { prevPosNormalized_ = val; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
protected:
    vec2 pos_;
    vec2 posNormalized_;

    vec2 prevPos_;
    vec2 prevPosNormalized_;

};

class IVW_CORE_API TouchEvent : public InteractionEvent {
public:
    enum TouchState {
        TOUCH_STATE_NONE = 0,
        TOUCH_STATE_STARTED = 1,
        TOUCH_STATE_UPDATED = 2,
        TOUCH_STATE_ENDED = 4,
        TOUCH_STATE_ANY = TOUCH_STATE_STARTED | TOUCH_STATE_UPDATED | TOUCH_STATE_ENDED
    };
    TouchEvent(TouchEvent::TouchState state, uvec2 canvasSize = uvec2(0));
    TouchEvent(std::vector<TouchPoint> touchPoints, TouchEvent::TouchState state, uvec2 canvasSize);

    virtual TouchEvent* clone() const;
    virtual ~TouchEvent();

    std::vector<TouchPoint> getTouchPoints() const { return touchPoints_; }
    void setTouchPoints(std::vector<TouchPoint> val) { touchPoints_ = val; }

    inline uvec2 canvasSize() const { return canvasSize_; }

    /** 
     * \brief Computes average position. Returns vec2(0) if no touch points exist.
     * 
     * @return vec2 sum(touch points) / nPoints
     */
    vec2 getCenterPoint() const;

    /**
    * \brief Computes average normalized position. Returns vec2(0) if no touch points exist.
    *
    * @return vec2 sum(touch points) / nPoints
    */
    vec2 getCenterPointNormalized() const;

    /**
    * \brief Computes previous average normalized position. Returns vec2(0) if no touch points exist.
    *
    * @return vec2 sum(touch points) / nPoints
    */
    vec2 getPrevCenterPointNormalized() const;

    inline TouchEvent::TouchState state() const { return state_; }

    virtual std::string getClassIdentifier() const { return "org.inviwo.TouchEvent"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    virtual bool matching(const Event* aEvent) const;
    virtual bool matching(const TouchEvent* aEvent) const;
    virtual bool equalSelectors(const Event* aEvent) const;

private:
    std::vector<TouchPoint> touchPoints_;
    uvec2 canvasSize_;

    TouchEvent::TouchState state_;
};

} // namespace

#endif // IVW_TOUCHEVENT_H