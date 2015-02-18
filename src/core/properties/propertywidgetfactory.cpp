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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/properties/propertysemantics.h>

namespace inviwo {

PropertyWidgetFactory::PropertyWidgetFactory() {}

PropertyWidgetFactory::~PropertyWidgetFactory() {}


void PropertyWidgetFactory::registerObject(PropertyWidgetFactoryObject* propertyWidget) {
    std::string className = propertyWidget->getClassIdentifier();
    PropertySemantics sematics = propertyWidget->getSematics();
    std::pair<WidgetMap::const_iterator, WidgetMap::const_iterator> sameKeys;
    sameKeys = widgetMap_.equal_range(className);

    for (WidgetMap::const_iterator it = sameKeys.first; it != sameKeys.second; ++it) {
        if (sematics == it->second->getSematics()) {
            LogWarn("Adding a PropertyWidget for a Property (" << className
                    << ") and semantics (" << sematics << ") that is already registed.");
        }
    }

    widgetMap_.insert(std::make_pair(className, propertyWidget));
}

PropertyWidget* PropertyWidgetFactory::create(Property* property) {
    PropertySemantics sematics = property->getSemantics();
    std::pair<WidgetMap::const_iterator, WidgetMap::const_iterator> sameKeys;
    sameKeys = widgetMap_.equal_range(property->getClassIdentifierForWidget()); 

    for (WidgetMap::const_iterator it = sameKeys.first; it != sameKeys.second; ++it) {
        if (sematics == it->second->getSematics())
            return it->second->create(property);
    }

    for (WidgetMap::const_iterator it = sameKeys.first; it != sameKeys.second; ++it) {
        if (PropertySemantics::Default == it->second->getSematics()) {
            LogWarn("Requested property widget semantics ("<< sematics <<") for property ("
                    <<property->getClassIdentifier()<<") does not exist, returning default semantics.");
            return it->second->create(property);
        }
    }

    LogWarn("Can not find a property widget for property: " << property->getClassIdentifier() << "("<< sematics <<")");
    return 0;
}

IvwSerializable* PropertyWidgetFactory::create(const std::string &className) const {
    // Widgets are not serializable...
    return 0;
}

bool PropertyWidgetFactory::isValidType(const std::string &className) const {
    WidgetMap::const_iterator it = widgetMap_.find(className);

    if (it != widgetMap_.end())
        return true;
    else
        return false;
}

std::vector<PropertySemantics> PropertyWidgetFactory::getSupportedSemanicsForProperty(Property* property) {
    std::pair<WidgetMap::const_iterator, WidgetMap::const_iterator> sameKeys;
    sameKeys = widgetMap_.equal_range(property->getClassIdentifier());
    std::vector<PropertySemantics> semantics;

    for (WidgetMap::const_iterator it = sameKeys.first; it != sameKeys.second; ++it)
        semantics.push_back(it->second->getSematics());

    return semantics;
}

} // namespace
