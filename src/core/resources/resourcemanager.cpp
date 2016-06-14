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

#include <inviwo/core/resources/resourcemanager.h>
#include <algorithm>

namespace inviwo {

struct ResourceComparer {
    ResourceComparer(const std::string& identifier) : identifier_(identifier) {}
    bool operator()(const Resource* resource) const { return *resource == identifier_; }
    const std::string& identifier_;
};

ResourceManager::ResourceManager() : ResourceManagerObservable() {
    resources_ = new std::vector<Resource*>();
}

ResourceManager::~ResourceManager() {
    for (auto& elem : *resources_) {
        delete elem;
    }
    delete resources_;
}

Resource* ResourceManager::getResource(const std::string& identifier) {
    std::vector<Resource*>::iterator it =
        std::find_if(resources_->begin(), resources_->end(), ResourceComparer(identifier));

    if (it != resources_->end())
        return *it;
    else
        return nullptr;
}

void ResourceManager::clearAllResources() {
    // Deallocate resources
    for (auto& elem : *resources_) {
        notifyResourceRemoved(elem);
        delete elem;
    }

    resources_->clear();
}

void ResourceManager::removeResource(Resource* resource) {
    std::vector<Resource*>::iterator it =
        std::find(resources_->begin(), resources_->end(), resource);

    if (it != resources_->end()) {
        notifyResourceRemoved(resource);
        delete *it;
        resources_->erase(it);
    }
}

void ResourceManager::removeResource(const std::string& identifier) {
    std::vector<Resource*>::iterator it =
        std::find_if(resources_->begin(), resources_->end(), ResourceComparer(identifier));

    if (it != resources_->end()) {
        notifyResourceRemoved(*it);
        delete *it;
        resources_->erase(it);
    }
}

}  // namespace
