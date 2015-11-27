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
#include <inviwo/core/interaction/events/eventpropagator.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/processors/processorinfo.h>
#include <inviwo/core/processors/processorstate.h>
#include <inviwo/core/processors/processortags.h>
#include <inviwo/core/util/group.h>

namespace inviwo {

class Event;
class InteractionHandler;
class ProcessorWidget;
class ResizeEvent;
class ProcessorNetwork;

#define InviwoProcessorInfo()                                                             \
    virtual const ProcessorInfo getProcessorInfo() const override {                       \
        return ProcessorInfo(CLASS_IDENTIFIER, DISPLAY_NAME, CATEGORY, CODE_STATE, TAGS); \
    }                                                                                     \
    static const std::string CLASS_IDENTIFIER;                                            \
    static const std::string CATEGORY;                                                    \
    static const std::string DISPLAY_NAME;                                                \
    static const CodeState CODE_STATE;                                                    \
    static const Tags TAGS

#define ProcessorClassIdentifier(T, classIdentifier) \
    const std::string T::CLASS_IDENTIFIER = classIdentifier;
#define ProcessorDisplayName(T, displayName) const std::string T::DISPLAY_NAME = displayName;
#define ProcessorTags(T, tags) const Tags T::TAGS = tags;
#define ProcessorCategory(T, category) const std::string T::CATEGORY = category;
#define ProcessorCodeState(T, codeState) const CodeState T::CODE_STATE = codeState;

/**
 * \defgroup processors Processors
 * \class Processor
 *
 * \brief A processor generally performs operation on input data and outputs the new result.
 *
 * It can hold arbitrary number of inports and outports, as well as properties which can be used
 * to customize the processors behavior.
 *
 * A typical flow for processing Processor 1 is shown below.
 *
 *     ┌─────────────┐
 *     │             │
 *     │ Processor 2 │
 *     │ ┌─┐         │◀────────────────────────────────────────── Outport 2
 *     └─┴┬┴─────────┘
 *        └────────────────┐
 *                       ┌┬▼┬──────────┐ ◀─────────────────────── Inport 1
 *                       │└─┘          │
 *                       │ Processor 1 │
 *                       │┌─┐          │ ◀─────────────────────── Outport 1
 *                       └┴┬┴──────────┘
 *                         └────────────────┐
 *                                        ┌┬▼┬──────────┐ ◀────── Inport 0
 *                                        │└─┘          │
 *                                        │ Processor 0 │
 *                                        │┌─┐          │ ◀────── Outport 0
 *                                        └┴─┴──────────┘
 *
 *       Evaluator         Processor 1         Inport 1           Outport 2
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           ├─────!isValid?────▶                  │                  │
 *           ◀────────No────────┤                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           ├────isReady?──────▶                  │                  │
 *           │                  ├─────isValid?─────▶                  │
 *           │                  │                  ├────isValid?──────▶
 *           │                  │                  │                  ├─────┐
 *           │                  │                  │                  │  data?
 *           │                  │                  │                  │  valid?
 *           │                  │                  │                  ◀─Yes─┘
 *           │                  │                  ◀──────Yes─────────┤
 *           │                  ◀───────Yes────────┤                  │
 *           ◀──────Yes─────────┤                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │    Inv. Level    │                  │                  │
 *           ├─────────>────────▶                  │                  │
 *           │   INV.RESOURCES  │                  │                  │
 *           │                  │                  │                  │
 *           ◀───────Yes────────┤                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           ├──Init. Resources ▶                  │                  │
 *           ◀───────Done───────┤                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           ├────GetInports────▶                  │                  │
 *           │                  ├──callOnChange────▶                  │
 *           │                  ◀──────Done────────┤                  │
 *           ◀───────Done───────┤                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           │                  │                  │                  │
 *           ├────Process───────▶                  │                  │
 *           │                  ├─────GetData──────▶                  │
 *           │                  │                  ├────GetData───────▶
 *           │                  │                  ◀──────data────────┤
 *           │                  ◀───────data───────┤                  │
 *           │                  │                  │                  │
 *           │                  │                  ▼                  ▼
 *           │                  │
 *           │                  │              Outport 1
 *           │                  │                  │
 *           │                  ├─────SetData──────▶
 *           │                  ◀───────Done───────┤
 *           ◀───────Done───────┤                  │
 *           │                  │                  ▼
 *           │                  │
 *           │                  │              Inport 1
 *           ├─────SetValid─────▶    SetChanged    │
 *           │                  ├─────(false)──────▶
 *           │                  ◀───────Done───────┤
 *           │                  │                  │
 *           │                  │                  ▼
 *           │                  │
 *           │                  │              Outport 1          Inport 0
 *           │                  │                  │                  │
 *           │                  ├───SetValid───────▶                  │
 *           │                  │                  ├─────SetValid─────▶
 *           │                  │                  │                  ├──────┐
 *           │                  │                  │                  │ SetChanged
 *           │                  │                  │                  │   (true)
 *           │                  │                  │                  ◀──────┘
 *           │                  │                  ◀──────Done────────┤
 *           │                  ◀─────Done─────────┤                  │
 *           ◀───────Done───────┤                  │                  │
 *           ▼                  ▼                  ▼                  ▼
 *
 *
 */
class IVW_CORE_API Processor : public PropertyOwner,
                               public MetaDataOwner,
                               public ProcessorObservable,
                               public EventPropagator {
public:
    Processor();
    virtual ~Processor();

    //  Should be implemented by all inheriting classes;
    virtual const ProcessorInfo getProcessorInfo() const = 0;

    std::string getClassIdentifier() const { return getProcessorInfo().classIdentifier; }
    std::string getDisplayName() const { return getProcessorInfo().displayName; }
    std::string getCategory() const { return getProcessorInfo().category; }
    CodeState getCodeState() const { return getProcessorInfo().codeState; }
    Tags getTags() const { return getProcessorInfo().tags; }

    /**
     * Sets the identifier of the Processor. If there already exist a processor with that identifier
     * it will append a number, starting at 2 to ensure uniqueness of identifiers.
     * @param identifier the new identifier
     * @return The identifier that was set including eventual appended number
     */
    std::string setIdentifier(const std::string& identifier);
    std::string getIdentifier();
    virtual std::vector<std::string> getPath() const override;

    void setProcessorWidget(ProcessorWidget* processorWidget);
    ProcessorWidget* getProcessorWidget() const;
    bool hasProcessorWidget() const;

    void setNetwork(ProcessorNetwork* network);
    ProcessorNetwork* getNetwork() const;

    /**
     * Initialize is called once before the first time the process function of the processor
     * is called. It is called by the processor network evaluator. Override to add resource
     * allocation in your processor. Make sure to call the base class initialize first.
     */
    virtual void initialize();

    /*
     *    Deinitialize is called once before the processor is deleted by the processor network
     *    Override to delete resources allocated in initialize. Make sure to call the base class
     *    deinitialize last.
     */
    virtual void deinitialize();
    bool isInitialized() const;

    /**
     * InitializeResources is called whenever a property with InvalidationLevel::InvalidResources
     * is changes.
     */
    virtual void initializeResources() {}

    Port* getPort(const std::string& identifier) const;
    Inport* getInport(const std::string& identifier) const;
    Outport* getOutport(const std::string& identifier) const;

    const std::vector<Inport*>& getInports() const;
    const std::vector<Outport*>& getOutports() const;

    std::vector<std::string> getPortDependencySets() const;
    std::vector<Port*> getPortsByDependencySet(const std::string& portDependencySet) const;
    std::string getPortDependencySet(Port* port) const;
    std::vector<Port*> getPortsInSameSet(Port* port) const;

    bool allInportsConnected() const;
    bool allInportsAreReady() const;

    virtual bool isEndProcessor() const;
    virtual bool isReady() const;

    /**
     *    Called when the network is evaluated and the processor is ready and not valid.
     *    The work of the processor should be done here.
     */
    virtual void process() {}

    /**
     *    Called when the network is evaluated and the processor is neither ready or valid.
     */
    virtual void doIfNotReady() {}

    /**
     *    Called by the network after Processor::process has been called.
     *    This will set the following to valid
     *    * The processor
     *    * All properties
     *    * All outports and their connected inports.
     * It will also set is't inports "changed" to false.
     */
    virtual void setValid() override;

    /**
     * Triggers invalidation.
     * Perform only full reimplementation of this function, meaning never call
     * Proccessor::invalidate()
     * in your reimplemented invalidation function.
     * The general scheme is that the processor will invalidate is self and it's outports
     * the outports will in turn invalidate their connected inports, which will invalidate there
     * processors. Hence all processors that depend on this one in the network will be invalidated.
     */
    virtual void invalidate(InvalidationLevel invalidationLevel,
                            Property* modifiedProperty = nullptr) override;

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

    virtual void invokeEvent(Event* event) override;
    virtual void propagateEvent(Event* event) override;
    virtual bool propagateResizeEvent(ResizeEvent* event, Outport* source) override;

    // Override from the property owner
    virtual Processor* getProcessor() override { return this; }
    virtual const Processor* getProcessor() const override { return this; }

    virtual void serialize(IvwSerializer& s) const override;
    virtual void deserialize(IvwDeserializer& d) override;

    static const std::string getCodeStateString(CodeState state);

protected:
    void enableInvalidation();
    void disableInvalidation();

    void addPort(Inport* port, const std::string& portDependencySet = "default");
    void addPort(Inport& port, const std::string& portDependencySet = "default");

    void addPort(Outport* port, const std::string& portDependencySet = "default");
    void addPort(Outport& port, const std::string& portDependencySet = "default");

    virtual void performEvaluateRequest();

    ProcessorWidget* processorWidget_;  //< non-owning reference, the widget is owned by the editor.

private:
    std::string identifier_;
    std::vector<Inport*> inports_;
    std::vector<Outport*> outports_;
    std::vector<InteractionHandler*> interactionHandlers_;

    Group<std::string, Port*> portDependencySets_;
    static std::unordered_set<std::string> usedIdentifiers_;

    bool initialized_;
    bool invalidationEnabled_;
    InvalidationLevel invalidationRequestLevel_;
    ProcessorNetwork* network_;
};

}  // namespace

#endif  // IVW_PROCESSOR_H
