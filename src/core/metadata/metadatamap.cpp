/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#include <inviwo/core/metadata/metadatamap.h>
#include <inviwo/core/io/serialization/serializable.h>

namespace inviwo {

MetaDataMap::MetaDataMap() {}

MetaDataMap::MetaDataMap(const MetaDataMap& inMap) {
    for (const auto& elem : inMap.metaData_) {
        if (elem.second) {
            metaData_[elem.first] = elem.second->clone();
        }
    }
}

MetaDataMap::~MetaDataMap() {
    removeAll();
}

MetaDataMap* MetaDataMap::clone() const {
    return new MetaDataMap(*this);
}

void MetaDataMap::add(const std::string &key, MetaData* metaData) {
    remove(key);
    metaData_[key] = metaData;
}

void MetaDataMap::remove(const std::string &key) {
    iterator it = metaData_.find(key);

    if (it != metaData_.end() && it->second) {
        delete it->second;
        metaData_.erase(it);
    }
}

void MetaDataMap::removeAll() {
    for (cIterator cIt = metaData_.begin(); cIt!=metaData_.end(); ++cIt)
        delete cIt->second;

    metaData_.clear();
}

void MetaDataMap::rename(const std::string &newKey, const std::string &oldKey) {
    MetaData* data = get(oldKey);

    if (data) {
        metaData_.erase(oldKey);
        add(newKey, data);
    }
}

std::vector<std::string> MetaDataMap::getKeys() const {
    std::vector<std::string> keys;

    for (const auto& elem : metaData_) keys.push_back(elem.first);

    return keys;
}

MetaData* MetaDataMap::get(const std::string &key) {
    cIterator it = metaData_.find(key);

    if (it!=metaData_.end())
        return it->second;
    return nullptr;
}

const MetaData* MetaDataMap::get(const std::string &key) const {
    cIterator it = metaData_.find(key);

    if (it!=metaData_.end())
        return const_cast<const MetaData*>(it->second);

    return nullptr;
}

MetaDataMap& MetaDataMap::operator=(const MetaDataMap& map) {
    removeAll();

    for (const auto& elem : map.metaData_) metaData_[elem.first] = elem.second->clone();

    return *this;
}

void MetaDataMap::serialize(Serializer& s) const {
    if (!metaData_.empty()) s.serialize("MetaDataMap", metaData_, "MetaDataItem");
}

void MetaDataMap::deserialize(Deserializer& d) {
    d.deserialize("MetaDataMap", metaData_, "MetaDataItem");
}

bool operator==(const MetaDataMap& lhs, const MetaDataMap& rhs) {
    typedef std::map<std::string, MetaData*>::const_iterator cIterator;

    if (lhs.metaData_.size() != rhs.metaData_.size()) {
        return false;
    }
    for (const auto& _cIt : lhs.metaData_) {
        cIterator elem = rhs.metaData_.find(_cIt.first);
        if (elem == rhs.metaData_.end()) {
            return false;
        }
        if (*(elem->second) != *(_cIt.second)) {
            return false;
        }
    }
    return true;
}

bool operator!=(const MetaDataMap& lhs, const MetaDataMap& rhs) {
    return !operator==(lhs, rhs);
}

} // namespace
