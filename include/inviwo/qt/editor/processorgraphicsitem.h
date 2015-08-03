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

#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include <inviwo/core/processors/processorobserver.h>
#include <inviwo/core/util/clock.h>
#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/editorgrapicsitem.h>
#include <inviwo/qt/widgets/labelgraphicsitem.h>

#include <QEvent>

namespace inviwo {

class Processor;
class ProcessorProgressGraphicsItem;
class ProcessorStatusGraphicsItem;
class ProcessorLinkGraphicsItem;
class ProcessorInportGraphicsItem;
class ProcessorOutportGraphicsItem;
class ProcessorMetaData;
class Port;
class Inport;
class Outport;

class IVW_QTEDITOR_API ProcessorGraphicsItem : public EditorGraphicsItem,
                                               public ProcessorObserver,
                                               public LabelGraphicsItemObserver {
public:
    ProcessorGraphicsItem(Processor* processor);
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const;

    ProcessorInportGraphicsItem* getInportGraphicsItem(Inport* port);
    ProcessorOutportGraphicsItem* getOutportGraphicsItem(Outport* port);
    ProcessorLinkGraphicsItem* getLinkGraphicsItem() const;
    ProcessorStatusGraphicsItem* getStatusItem() const;

    void editProcessorName();
    void onLabelGraphicsItemChange();
    bool isEditingProcessorName();

    void snapToGrid();

    // override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + ProcessorGraphicsType };
    int type() const { return Type; }

    virtual void showToolTip(QGraphicsSceneHelpEvent* event);

    // ProcessorObserver overrides
    virtual void onProcessorIdentifierChange(Processor*);
    virtual void onProcessorPortAdded(Processor*, Port*);
    #if IVW_PROFILING
    virtual void onProcessorAboutToProcess(Processor*);
    virtual void onProcessorFinishedProcess(Processor*);
    void resetTimeMeasurements();
    #endif

    void setHighlight(bool val);

protected:
    void setIdentifier(QString text);
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void addInport(Inport *port);
    void addOutport(Outport *port);

private:
    Processor* processor_;
    LabelGraphicsItem* nameLabel_;
    LabelGraphicsItem* classLabel_;
    ProcessorMetaData* processorMeta_;

    ProcessorProgressGraphicsItem* progressItem_;
    ProcessorStatusGraphicsItem* statusItem_;
    ProcessorLinkGraphicsItem* linkItem_;

    std::map<Inport*, ProcessorInportGraphicsItem*> inportItems_;
    std::map<Outport*, ProcessorOutportGraphicsItem*> outportItems_;
    
    bool highlight_;
    qreal inportX,inportY;
    qreal outportX,outportY;
    
    #if IVW_PROFILING 
    size_t processCount_;
    LabelGraphicsItem* countLabel_;
    double maxEvalTime_;
    double evalTime_;
    double totEvalTime_;
    Clock clock_;
    #endif
};

}  // namespace

#endif  // IVW_PROCESSORGRAPHICSITEM_H