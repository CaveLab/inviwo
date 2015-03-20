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

#ifndef IVW_PROCESSOR_H
#define IVW_PROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/processorobserver.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/processors/processorstate.h>
#include <inviwo/core/processors/processortags.h>
#include <inviwo/core/util/group.h>

namespace inviwo {

class Event;
class InteractionHandler;
class ProcessorWidget;

#define InviwoProcessorInfo()                                                   \
    virtual std::string getClassIdentifier() const { return CLASS_IDENTIFIER; } \
    virtual std::string getDisplayName() const { return DISPLAY_NAME; }         \
    virtual std::string getCategory() const { return CATEGORY; }                \
    virtual CodeState getCodeState() const { return CODE_STATE; }               \
    virtual Tags getTags() const { return TAGS; }                               \
    static const std::string CLASS_IDENTIFIER;                                  \
    static const std::string CATEGORY;                                          \
    static const std::string DISPLAY_NAME;                                      \
    static const CodeState CODE_STATE;                                          \
    static const Tags TAGS;

#define ProcessorClassIdentifier(T, classIdentifier) \
    const std::string T::CLASS_IDENTIFIER = classIdentifier;
#define ProcessorDisplayName(T, displayName) const std::string T::DISPLAY_NAME = displayName;
#define ProcessorTags(T, tags) const Tags T::TAGS = tags;
#define ProcessorCategory(T, category) const std::string T::CATEGORY = category;
#define ProcessorCodeState(T, codeState) const CodeState T::CODE_STATE = codeState;

/**
 * \class Processor
 *
 * \brief A processor generally performs operation on input data and outputs the new result.
 *
 * It can hold arbitrary number of inports and outports, as well as properties which can be used
 * to customize the processors behavior.
 */
class IVW_CORE_API Processor : public PropertyOwner,
                               public MetaDataOwner,
                               public ProcessorObservable {
public:
    Processor();
    virtual ~Processor();

    InviwoProcessorInfo();

    /**
     * Sets the identifier of the Processor. If there already exist a processor with that identifier
     * it will append a number, starting at 2 to ensure uniqueness of identifiers.
     * @param the new identifier
     * @return The identifier that was set including eventual appended number
     */
    std::string setIdentifier(const std::string& identifier);
    std::string getIdentifier();
    virtual std::vector<std::string> getPath() const;

    void setProcessorWidget(ProcessorWidget* processorWidget);
    ProcessorWidget* getProcessorWidget() const;
    bool hasProcessorWidget() const;


    /**
     * Initialize is called once before the first time the process function of the processor
     * is called. It is called by the processor network evaluator. Override to add resource
     * allocation in your processor. Make sure to call the base class initialize first.
     */
    virtual void initialize();

    /*
     *	Deinitialize is called once before the processor is deleted by the processor network
     *	Override to delete resources allocated in initialize. Make sure to call the base class 
     *	deinitialize last.
     */  
    virtual void deinitialize();
    bool isInitialized() const;

    Port* getPort(const std::string& identifier) const;
    Inport* getInport(const std::string& identifier) const;
    Outport* getOutport(const std::string& identifier) const;

    const std::vector<Inport*>& getInports() const;
    const std::vector<Outport*>& getOutports() const;

    virtual const std::vector<Inport*>& getInports(Event*) const;

    std::vector<std::string> getPortDependencySets() const;
    std::vector<Port*> getPortsByDependencySet(const std::string& portDependencySet) const;
    std::string getPortDependencySet(Port* port) const;

    bool allInportsConnected() const;
    bool allInportsAreReady() const;

    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    virtual void process();

    virtual void doIfNotReady() {}

    // Triggers invalidation.
    // Perform only full reimplementation of this function, meaning never call
    // Proccessor::invalidate()
    // in your reimplemented invalidation function.
    virtual void invalidate(InvalidationLevel invalidationLevel,
                            Property* modifiedProperty = nullptr);

    // Triggers invalidation of successors.
    // Perform only full reimplementation of this function, meaning never call
    // Proccessor::invalidateSuccesors()
    // in your reimplemented invalidation function.
    virtual void invalidateSuccesors(InvalidationLevel invalidationLevel,
                                     Property* modifiedProperty = nullptr);

    virtual void setValid();
    virtual void initializeResources() {}  // reload shaders etc. here

    /**
     * Adds the interaction handler such that it receives events propagated
     * to the processor. Will not add the interaction handler if it has been added before.
     * @note The processor will not take ownership of the interaction handler.
     * @see InteractionHandler
     * @param interactionHandler Interaction handler to be added.
     */
    void addInteractionHandler(InteractionHandler* interactionHandler);
    /**
     * Remove the interaction handler such that events are not propagated.
     * The interaction handler will not be deleted by the processor.
     * @param interactionHandler Interaction handler to be removed.
     */
    void removeInteractionHandler(InteractionHandler* interactionHandler);
    bool hasInteractionHandler() const;
    const std::vector<InteractionHandler*>& getInteractionHandlers() const;
    virtual void invokeInteractionEvent(Event* event);

    // Override from the property owner
    virtual Processor* getProcessor() { return this; }
    virtual const Processor* getProcessor() const { return this; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    static const std::string getCodeStateString(CodeState state);

protected:
    void enableInvalidation();
    void disableInvalidation();

    void addPort(Inport* port, const std::string& portDependencySet = "default");
    void addPort(Inport& port, const std::string& portDependencySet = "default");

    void addPort(Outport* port, const std::string& portDependencySet = "default");
    void addPort(Outport& port, const std::string& portDependencySet = "default");

    virtual void performEvaluateRequest();

    ProcessorWidget* processorWidget_; //< non-owning reference, the widget is owned by the editor.

private:
    std::string identifier_;
    std::vector<Inport*> inports_;
    std::vector<Outport*> outports_;
    std::vector<InteractionHandler*> interactionHandlers_;

    Group<std::string, Port*> portDependencySets_;
    static std::set<std::string> usedIdentifiers_;

    bool initialized_;
    bool invalidationEnabled_;
    InvalidationLevel invalidationRequestLevel_;
};

}  // namespace

#endif  // IVW_PROCESSOR_H
