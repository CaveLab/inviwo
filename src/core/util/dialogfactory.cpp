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

#include <inviwo/core/util/dialogfactory.h>

namespace inviwo {

DialogFactory::DialogFactory() {}

DialogFactory::~DialogFactory() {}

void DialogFactory::registerObject(DialogFactoryObject* dialog) {
    std::string className = dialog->getClassIdentifier();

    if (map_.find(className) != map_.end()) {
        LogWarn("Dialog already registed: " << className);
    }

    map_.insert(std::make_pair(className, dialog));
}


Dialog* DialogFactory::getDialog(const std::string &className) const {
    DialogMap::const_iterator it = map_.find(className);

    if (it != map_.end()) {
        return it->second->create();
    } else {
        return NULL;
    }
}

IvwSerializable* DialogFactory::create(const std::string &className) const {
    // Dialogs are not serializable...
    return 0;
}

bool DialogFactory::isValidType(const std::string &className) const {
    if (map_.find(className) != map_.end())
        return true;
    else
        return false;
}

} // namespace

