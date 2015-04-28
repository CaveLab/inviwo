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

#ifndef IVW_DATAINPORT_H
#define IVW_DATAINPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/singleinport.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

template<typename T>
class DataOutport;

template<typename T>
class DataInport : public SingleInport {
public:
    DataInport(std::string identifier);
    virtual ~DataInport();

    virtual uvec3 getColorCode() const override;
    virtual std::string getClassIdentifier() const override;

    virtual bool canConnectTo(Port* port) const override;
    virtual void connectTo(Outport* port) override;
    virtual bool isReady() const override;

    virtual const T* getData() const;
    bool hasData() const;
    virtual std::string getContentInfo() const;
};

template<typename T>
std::string inviwo::DataInport<T>::getClassIdentifier() const  {
    return port_traits<T>::class_identifier() + "Inport";
}

template <typename T>
DataInport<T>::DataInport(std::string identifier)
    : SingleInport(identifier) {
}

template <typename T>
uvec3 DataInport<T>::getColorCode() const { return port_traits<T>::color_code(); }

template <typename T>
DataInport<T>::~DataInport() {}

template <typename T>
bool DataInport<T>::canConnectTo(Port* port) const {
    if ( dynamic_cast<DataOutport<T>*>(port) &&  port->getProcessor() != getProcessor())
        return true;
    else
        return false;
}

template <typename T>
void DataInport<T>::connectTo(Outport* port) {
    DataOutport<T>* dataPort = dynamic_cast<DataOutport<T>*>(port);
    ivwAssert(dataPort!=nullptr, "Trying to connect incompatible ports.")

    if (dataPort != nullptr)
        SingleInport::connectTo(port);
    else
        LogWarn("Trying to connect incompatible ports.");
}
template <typename T>
bool DataInport<T>::isReady() const { return SingleInport::isReady() && hasData(); }

template <typename T>
const T* DataInport<T>::getData() const {
    if (isConnected()) {
        // Safe to static cast since we are unable to connect other outport types.
        return static_cast< DataOutport<T>* >(connectedOutport_)->getConstData();
    } else
        return nullptr;
}

template <typename T>
bool DataInport<T>::hasData() const {
    if (isConnected()) {
        // Safe to static cast since we are unable to connect other outport types.
        return static_cast< DataOutport<T>* >(connectedOutport_)->hasData();
    } else {
        return false;
    }
}

template <typename T>
std::string DataInport<T>::getContentInfo() const {
    if (hasData()) {
        std::string info = port_traits<T>::data_info(getData());
        if (!info.empty()) {
            return info;
        } else {
            return "No information available for: " + util::class_identifier<T>();
        }
    } else {
        return "Port has no data";
    }
}

} // namespace

#endif // IVW_DATAINPORT_H
