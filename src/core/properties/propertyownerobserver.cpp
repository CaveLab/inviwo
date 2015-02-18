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

#include <inviwo/core/properties/propertyownerobserver.h>

namespace inviwo {

void PropertyOwnerObservable::notifyObserversWillAddProperty(Property* property,
                                                             size_t index) const {
    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
        static_cast<PropertyOwnerObserver*>(*it)->onWillAddProperty(property, index);
    }
}

void PropertyOwnerObservable::notifyObserversDidAddProperty(Property* property,
                                                            size_t index) const {
    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
        static_cast<PropertyOwnerObserver*>(*it)->onDidAddProperty(property, index);
    }
}

void PropertyOwnerObservable::notifyObserversWillRemoveProperty(Property* property,
                                                                size_t index) const {
    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
        static_cast<PropertyOwnerObserver*>(*it)->onWillRemoveProperty(property, index);
    }
}

void PropertyOwnerObservable::notifyObserversDidRemoveProperty(Property* property,
                                                               size_t index) const {
    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
        static_cast<PropertyOwnerObserver*>(*it)->onDidRemoveProperty(property, index);
    }
}

}  // namespace
