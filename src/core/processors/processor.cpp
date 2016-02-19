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

#include <inviwo/core/processors/processor.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/interactionevent.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/stdextensions.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

std::unordered_set<std::string> Processor::usedIdentifiers_;

Processor::Processor()
    : PropertyOwner()
    , ProcessorObservable()
    , processorWidget_(nullptr)
    , identifier_("")
    , network_(nullptr) {
    createMetaData<ProcessorMetaData>(ProcessorMetaData::CLASS_IDENTIFIER);
}

Processor::~Processor() {
    usedIdentifiers_.erase(identifier_);
}

void Processor::addPort(Inport* port, const std::string& portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    inports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
    notifyObserversProcessorPortAdded(this, port);
}

void Processor::addPort(Inport& port, const std::string& portDependencySet) {
    addPort(&port, portDependencySet);
}

void Processor::addPort(Outport* port, const std::string& portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    outports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
    notifyObserversProcessorPortAdded(this, port);
}

void Processor::addPort(Outport& port, const std::string& portDependencySet) {
    addPort(&port, portDependencySet);
}

std::string Processor::setIdentifier(const std::string& identifier) {
    if (identifier == identifier_) return identifier_;  // nothing changed

    usedIdentifiers_.erase(identifier_);  // remove old identifier

    std::string baseIdentifier = identifier;
    std::string newIdentifier = identifier;
    int i = 2;

    auto parts = splitString(identifier, ' ');
    if (parts.size() > 1 &&
        util::all_of(parts.back(), [](const char& c) { return std::isdigit(c); })) {
        i = std::stoi(parts.back());
        baseIdentifier = joinString(parts.begin(), parts.end() - 1, " ");
        newIdentifier = baseIdentifier;
    }

    while (usedIdentifiers_.find(newIdentifier) != usedIdentifiers_.end()) {
        newIdentifier = baseIdentifier + " " + toString(i++);
    }

    usedIdentifiers_.insert(newIdentifier);
    identifier_ = newIdentifier;

    notifyObserversIdentifierChange(this);
    return identifier_;
}

std::string Processor::getIdentifier() {
    if (identifier_.empty()) setIdentifier(getDisplayName());
    return identifier_;
}

void Processor::setProcessorWidget(std::unique_ptr<ProcessorWidget> processorWidget) {
    processorWidget_ = std::move(processorWidget);
}

ProcessorWidget* Processor::getProcessorWidget() const { return processorWidget_.get(); }

bool Processor::hasProcessorWidget() const { return (processorWidget_ != nullptr); }

void Processor::setNetwork(ProcessorNetwork* network) { network_ = network; }

ProcessorNetwork* Processor::getNetwork() const { return network_; }

Port* Processor::getPort(const std::string& identifier) const {
    for (auto port : inports_) if (port->getIdentifier() == identifier) return port;
    for (auto port : outports_) if (port->getIdentifier() == identifier) return port;
    return nullptr;
}

Inport* Processor::getInport(const std::string& identifier) const {
    for (auto port : inports_) if (port->getIdentifier() == identifier) return port;
    return nullptr;
}

Outport* Processor::getOutport(const std::string& identifier) const {
    for (auto port : outports_) if (port->getIdentifier() == identifier) return port;
    return nullptr;
}

const std::vector<Inport*>& Processor::getInports() const { return inports_; }

const std::vector<Outport*>& Processor::getOutports() const { return outports_; }

std::vector<Port*> Processor::getPortsByDependencySet(
    const std::string& portDependencySet) const {
    return portDependencySets_.getGroupedData(portDependencySet);
}

std::vector<std::string> Processor::getPortDependencySets() const {
    return portDependencySets_.getGroupKeys();
}

std::string Processor::getPortDependencySet(Port* port) const {
    return portDependencySets_.getKey(port);
}

std::vector<Port*> Processor::getPortsInSameSet(Port* port) const {
    return portDependencySets_.getGroupedData(portDependencySets_.getKey(port));
}

void Processor::invalidate(InvalidationLevel invalidationLevel, Property* modifiedProperty) {
    notifyObserversInvalidationBegin(this);
    PropertyOwner::invalidate(invalidationLevel, modifiedProperty);
    if (!isValid()) {
        for (auto& port : outports_) port->invalidate(InvalidationLevel::InvalidOutput);
    }
    notifyObserversInvalidationEnd(this);

    if (!isValid() && isEndProcessor()) {
        performEvaluateRequest();
    }
}

bool Processor::isEndProcessor() const { return outports_.empty(); }

bool Processor::isReady() const { return allInportsAreReady(); }

bool Processor::allInportsAreReady() const {
    return util::all_of(
        inports_, [](Inport* p) { return (p->isOptional() && !p->isConnected()) || p->isReady(); });
}

bool Processor::allInportsConnected() const {
    return util::all_of(inports_, [](Inport* p) { return p->isConnected(); });
}

void Processor::addInteractionHandler(InteractionHandler* interactionHandler) {
    util::push_back_unique(interactionHandlers_, interactionHandler);
}

void Processor::removeInteractionHandler(InteractionHandler* interactionHandler) {
    util::erase_remove(interactionHandlers_, interactionHandler);
}

bool Processor::hasInteractionHandler() const { return !interactionHandlers_.empty(); }

const std::vector<InteractionHandler*>& Processor::getInteractionHandlers() const {
    return interactionHandlers_;
}

void Processor::serialize(Serializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("identifier", identifier_, true);

    s.serialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");
    s.serialize("InPorts", inports_, "InPort");
    s.serialize("OutPorts", outports_, "OutPort");

    PropertyOwner::serialize(s);
    MetaDataOwner::serialize(s);
}

void Processor::deserialize(Deserializer& d) {
    std::string identifier;
    d.deserialize("identifier", identifier, true);
    setIdentifier(identifier);  // Need to use setIdentifier to make sure we get a unique id.

    d.deserialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");

    StandardIdentifier<Port> inportIdentifier;
    d.deserialize("InPorts", inports_, "InPort", inportIdentifier);
    d.deserialize("OutPorts", outports_, "OutPort", inportIdentifier);

    for (auto elem : inports_) {
        elem->setProcessor(this);
    }
    for (auto elem : outports_) {
        elem->setProcessor(this);
    }

    PropertyOwner::deserialize(d);
    MetaDataOwner::deserialize(d);
}

void Processor::setValid() {
    PropertyOwner::setValid();
    for (auto inport : inports_) inport->setChanged(false);
    for (auto outport : outports_) outport->setValid();
}

void Processor::performEvaluateRequest() { notifyObserversRequestEvaluate(this); }

void Processor::invokeEvent(Event* event) {
    PropertyOwner::invokeEvent(event);
    for (auto elem : interactionHandlers_) elem->invokeEvent(event);
}

void Processor::propagateEvent(Event* event, Outport* source) {
    if (event->hasVisitedProcessor(this)) return;
    event->markAsVisited(this);
    
    invokeEvent(event);
    if (event->hasBeenUsed()) return;

    for (auto inport : getInports()) {
        inport->propagateEvent(event);
        if (event->hasBeenUsed()) return;
    }
}

void Processor::propagateResizeEvent(ResizeEvent* resizeEvent, Outport* source) {
    if (resizeEvent->hasVisitedProcessor(this)) return;
    resizeEvent->markAsVisited(this);

    for (auto port : getPortsInSameSet(source)) {
        if (auto imageInport = dynamic_cast<ImagePortBase*>(port)) {
            imageInport->propagateResizeEvent(resizeEvent);
        }
    }
}

const std::string Processor::getCodeStateString(CodeState state) {
    std::stringstream ss;
    ss << state;
    return ss.str();
}

std::vector<std::string> Processor::getPath() const {
    std::vector<std::string> path;
    path.push_back(identifier_);
    return path;
}

}  // namespace
