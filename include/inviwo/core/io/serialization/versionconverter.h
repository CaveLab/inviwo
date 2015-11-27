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

#ifndef IVW_VERSIONCONVERTER_H
#define IVW_VERSIONCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class IVW_CORE_API VersionConverter {
public:
    VersionConverter();
    virtual ~VersionConverter() {}
    virtual bool convert(TxElement* root) = 0;
};

class IVW_CORE_API NodeVersionConverter : public VersionConverter {
public:
    template <typename T>
    NodeVersionConverter(T* obj, bool (T::*fPtr)(TxElement*));
    NodeVersionConverter(std::function<bool(TxElement*)> fun);

    virtual ~NodeVersionConverter() {}
    virtual bool convert(TxElement* root);

private:
    std::function<bool(TxElement*)> fun_;
};

template <typename T>
inviwo::NodeVersionConverter::NodeVersionConverter(T* obj, bool (T::*fPtr)(TxElement*))
    : VersionConverter(), fun_(std::bind(fPtr, obj, std::placeholders::_1)) {}

class IVW_CORE_API TraversingVersionConverter : public VersionConverter {
public:
    template <typename T>
    TraversingVersionConverter(T* obj, bool (T::*fPtr)(TxElement*));
    TraversingVersionConverter(std::function<bool(TxElement*)> fun);
    virtual ~TraversingVersionConverter() {}
    virtual bool convert(TxElement* root);

private:
    bool traverseNodes(TxElement* node);
    std::function<bool(TxElement*)> fun_;
};

template <typename T>
TraversingVersionConverter::TraversingVersionConverter(T* obj, bool (T::*fPtr)(TxElement*))
    : VersionConverter(), fun_(std::bind(fPtr, obj, std::placeholders::_1)) {}

namespace util {
IVW_CORE_API bool xmlCopyMatchingSubPropsIntoComposite(TxElement* node,
                                                       const CompositeProperty& prop);
IVW_CORE_API bool xmlHasProp(TxElement* node, const Property& prop);
IVW_CORE_API std::vector<TxElement*> xmlGetMatchingElements(TxElement* processornode,
                                                            std::string key);
IVW_CORE_API bool xmlFindMatchingSubPropertiesForComposites(
    TxElement* node, const std::vector<const CompositeProperty*>& props);
IVW_CORE_API TxElement* xmlGetElement(TxElement* node, std::string path);

IVW_CORE_API bool xmlCopyMatchingCompositeProperty(TxElement* node, const CompositeProperty& prop);

IVW_CORE_API void renamePort(Deserializer& d,
                std::initializer_list<std::pair<const Port*, std::string>> rules);

IVW_CORE_API void renameProperty(Deserializer& d,
                    std::initializer_list<std::pair<const Property*, std::string>> rules);

}  // namespace

}  // namespace

#endif  // IVW_VERSIONCONVERTER_H
