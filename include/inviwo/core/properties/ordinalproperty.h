/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
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

#ifndef IVW_ORDINALPROPERTY_H
#define IVW_ORDINALPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>
#include <string>
#include <sstream>

namespace inviwo {

template <typename T>
class OrdinalProperty : public TemplateProperty<T> {
public:
    InviwoPropertyInfo();

    OrdinalProperty(
        const std::string& identifier, const std::string& displayName,
        const T& value = Defaultvalues<T>::getVal(), const T& minValue = Defaultvalues<T>::getMin(),
        const T& maxValue = Defaultvalues<T>::getMax(),
        const T& increment = Defaultvalues<T>::getInc(),
        InvalidationLevel invalidationLevel = INVALID_OUTPUT,
        PropertySemantics semantics = PropertySemantics::Default);

    OrdinalProperty(const OrdinalProperty<T>& rhs);
    OrdinalProperty<T>& operator=(const OrdinalProperty<T>& that);
    OrdinalProperty<T>& operator=(const T& value);
    //virtual OrdinalProperty<T>* clone() const; // See ticket #642
    virtual ~OrdinalProperty();

    T getMinValue() const;
    T getMaxValue() const;
    T getIncrement() const;

    virtual void set(const T& value);  // Need to reimplement this to avoid compiler confusion with
                                       // set(const Property*)
    virtual void set(const Property* src);
    void setMinValue(const T& value);
    void setMaxValue(const T& value);
    void setIncrement(const T& value);

    virtual void setCurrentStateAsDefault();
    virtual void resetToDefaultState();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    static uvec2 getDim() { return Defaultvalues<T>::getDim(); }

private:
    using TemplateProperty<T>::value_;
    ValueWrapper<T> minValue_;
    ValueWrapper<T> maxValue_;
    ValueWrapper<T> increment_;
};

// Scalar properties
typedef OrdinalProperty<float> FloatProperty;
typedef OrdinalProperty<int> IntProperty;
typedef OrdinalProperty<glm::i64> Int64Property;
typedef OrdinalProperty<double> DoubleProperty;

// Vector properties
typedef OrdinalProperty<vec2> FloatVec2Property;
typedef OrdinalProperty<vec3> FloatVec3Property;
typedef OrdinalProperty<vec4> FloatVec4Property;

typedef OrdinalProperty<dvec2> DoubleVec2Property;
typedef OrdinalProperty<dvec3> DoubleVec3Property;
typedef OrdinalProperty<dvec4> DoubleVec4Property;

typedef OrdinalProperty<ivec2> IntVec2Property;
typedef OrdinalProperty<ivec3> IntVec3Property;
typedef OrdinalProperty<ivec4> IntVec4Property;

typedef OrdinalProperty<size2_t> IntSize2Property;
typedef OrdinalProperty<size3_t> IntSize3Property;
typedef OrdinalProperty<size4_t> IntSize4Property;

// Matrix properties
typedef OrdinalProperty<mat2> FloatMat2Property;
typedef OrdinalProperty<mat3> FloatMat3Property;
typedef OrdinalProperty<mat4> FloatMat4Property;

typedef OrdinalProperty<dmat2> DoubleMat2Property;
typedef OrdinalProperty<dmat3> DoubleMat3Property;
typedef OrdinalProperty<dmat4> DoubleMat4Property;

template <typename T> PropertyClassIdentifier(OrdinalProperty<T>,  "org.inviwo." + Defaultvalues<T>::getName() + "Property");

template <typename T>
OrdinalProperty<T>::OrdinalProperty(const std::string& identifier, const std::string& displayName,
                                    const T& value, const T& minValue, const T& maxValue,
                                    const T& increment,
                                    InvalidationLevel invalidationLevel,
                                    PropertySemantics semantics)
    : TemplateProperty<T>(identifier, displayName, value, invalidationLevel, semantics)
    , minValue_("minvalue", minValue)
    , maxValue_("maxvalue", maxValue)
    , increment_("increment", increment) {

    // Invariant minValue_ < value_ < maxValue_
    // Assume minValue is correct.
    value_.value = glm::max(value_.value, minValue_.value);
    maxValue_.value = glm::max(maxValue_.value, value_.value);
}


template <typename T>
OrdinalProperty<T>::OrdinalProperty(const OrdinalProperty<T>& rhs)
    : TemplateProperty<T>(rhs)
    , minValue_(rhs.minValue_)
    , maxValue_(rhs.maxValue_)
    , increment_(rhs.increment_) {
}

template <typename T>
OrdinalProperty<T>& OrdinalProperty<T>::operator=(const OrdinalProperty<T>& that) {
    if (this != &that) {
        TemplateProperty<T>::operator=(that);
        minValue_ = that.minValue_;
        maxValue_ = that.maxValue_;
        increment_ = that.increment_;
    }
    return *this;
}
template <typename T>
OrdinalProperty<T>& OrdinalProperty<T>::operator=(const T& value) {
    TemplateProperty<T>::operator=(value);
    return *this;
}

// template <typename T>
// OrdinalProperty<T>* OrdinalProperty<T>::clone() const {
//     return new OrdinalProperty<T>(*this);
// }

template <typename T>
OrdinalProperty<T>::~OrdinalProperty() {}

template <typename T>
void OrdinalProperty<T>::set(const T& value) {
    TemplateProperty<T>::set(value);
}

template <typename T>
void OrdinalProperty<T>::set(const Property* srcProperty) {
    const OrdinalProperty<T>* templatedSrcProp =
        dynamic_cast<const OrdinalProperty<T>*>(srcProperty);
    if (templatedSrcProp) {
        this->minValue_.value = templatedSrcProp->minValue_.value;
        this->maxValue_.value = templatedSrcProp->maxValue_.value;
        this->increment_.value = templatedSrcProp->increment_.value;
    }

    TemplateProperty<T>::set(srcProperty);
}

template <typename T>
T OrdinalProperty<T>::getMinValue() const {
    return minValue_;
}

template <typename T>
T OrdinalProperty<T>::getMaxValue() const {
    return maxValue_;
}

template <typename T>
T OrdinalProperty<T>::getIncrement() const {
    return increment_;
}

template <typename T>
void OrdinalProperty<T>::setMinValue(const T& value) {
    if (value == minValue_) return;
    minValue_ = value;
    
    // Make sure min < value < max
    this->value_.value = glm::max(this->value_.value, minValue_.value);
    maxValue_.value = glm::max(maxValue_.value, minValue_.value);

    Property::propertyModified();
}

template <typename T>
void OrdinalProperty<T>::setMaxValue(const T& value) {
    if (value == maxValue_) return;
    maxValue_ = value;
    
    // Make sure min < value < max
    this->value_.value = glm::min(this->value_.value, maxValue_.value);
    minValue_.value = glm::min(minValue_.value, maxValue_.value);
    
    Property::propertyModified();
}

template <typename T>
void OrdinalProperty<T>::setIncrement(const T& value) {
    if (value == increment_) return;
    increment_ = value;
    Property::propertyModified();
}

template <typename T>
void OrdinalProperty<T>::resetToDefaultState() {
    minValue_.reset();
    maxValue_.reset();
    increment_.reset();
    TemplateProperty<T>::resetToDefaultState();
}

template <typename T>
void OrdinalProperty<T>::setCurrentStateAsDefault() {
    TemplateProperty<T>::setCurrentStateAsDefault();
    minValue_.setAsDefault();
    maxValue_.setAsDefault();
    increment_.setAsDefault();
}

template <typename T>
void OrdinalProperty<T>::serialize(IvwSerializer& s) const {
    minValue_.serialize(s, this->serializationMode_);
    maxValue_.serialize(s, this->serializationMode_);
    increment_.serialize(s, this->serializationMode_);
    TemplateProperty<T>::serialize(s);
}

template <typename T>
void OrdinalProperty<T>::deserialize(IvwDeserializer& d) {
    minValue_.deserialize(d);
    maxValue_.deserialize(d);
    increment_.deserialize(d);
    TemplateProperty<T>::deserialize(d);
}

}  // namespace

#endif  // IVW_ORDINALPROPERTY_H
