/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2017 Inviwo Foundation
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

#ifndef IVW_ORDINALMINMAXTEXTTROPERTYWIDGETQT_H
#define IVW_ORDINALMINMAXTEXTTROPERTYWIDGETQT_H

#include <modules/qtwidgets/qtwidgetsmoduledefine.h>
#include <modules/qtwidgets/inviwoqtutils.h>
#include <modules/qtwidgets/properties/propertywidgetqt.h>
#include <modules/qtwidgets/properties/propertysettingswidgetqt.h>
#include <modules/qtwidgets/editablelabelqt.h>
#include <modules/qtwidgets/ordinaleditorwidget.h>
#include <inviwo/core/properties/minmaxproperty.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

class IVW_MODULE_QTWIDGETS_API BaseOrdinalMinMaxTextPropertyWidgetQt : public PropertyWidgetQt {

#include <warn/push>
#include <warn/ignore/all>
    Q_OBJECT
#include <warn/pop>

public:
    BaseOrdinalMinMaxTextPropertyWidgetQt(Property* property);
    virtual ~BaseOrdinalMinMaxTextPropertyWidgetQt();
    
    virtual void updateFromProperty() = 0;
    virtual QMenu* getContextMenu();
    
public slots:
    virtual void updateFromMin() = 0;
    virtual void updateFromMax() = 0;
    virtual void showSettings() = 0;
    virtual void showContextMenu(const QPoint& pos);    
    
protected:
    void generateWidget();
    virtual void makeEditorWidgets() = 0;    

    PropertySettingsWidgetQt* settingsWidget_;    
    
    BaseOrdinalEditorWidget* min_;
    BaseOrdinalEditorWidget* max_;
    EditableLabelQt* label_;

    QMenu* contextMenu_;
    QAction* settingsAction_;
    void generatesSettingsWidget();
};



template <typename BT, typename T>
class OrdinalMinMaxTextPropertyWidgetQt : public BaseOrdinalMinMaxTextPropertyWidgetQt {

public:
    OrdinalMinMaxTextPropertyWidgetQt(MinMaxProperty<T>* property);
    virtual ~OrdinalMinMaxTextPropertyWidgetQt();

    virtual void updateFromProperty() override;
    virtual void updateFromMin() override;
    virtual void updateFromMax() override;

    typedef glm::tvec2<T, glm::defaultp> V;
    
protected:
    virtual void makeEditorWidgets() override;

    virtual void showSettings() override {
        if (!this->settingsWidget_) {
            this->settingsWidget_ =
                new TemplateMinMaxPropertySettingsWidgetQt<BT, T>(minMaxProperty_, this);
        }
        this->settingsWidget_->showWidget();
    }
    
    MinMaxProperty<T>* minMaxProperty_;
    
    OrdinalEditorWidget<T>* min_;
    OrdinalEditorWidget<T>* max_;
};

typedef OrdinalMinMaxTextPropertyWidgetQt<double, double> DoubleMinMaxTextPropertyWidgetQt;
typedef OrdinalMinMaxTextPropertyWidgetQt<double, float> FloatMinMaxTextPropertyWidgetQt;
typedef OrdinalMinMaxTextPropertyWidgetQt<int, int> IntMinMaxTextPropertyWidgetQt;


template <typename BT, typename T>
OrdinalMinMaxTextPropertyWidgetQt<BT, T>::OrdinalMinMaxTextPropertyWidgetQt(MinMaxProperty<T>* property)
    : BaseOrdinalMinMaxTextPropertyWidgetQt(property)
    , minMaxProperty_(property) {
    
    BaseOrdinalMinMaxTextPropertyWidgetQt::generateWidget();
    updateFromProperty();
}

template <typename BT, typename T>
OrdinalMinMaxTextPropertyWidgetQt<BT, T>::~OrdinalMinMaxTextPropertyWidgetQt() {
}

template <typename BT, typename T>
void OrdinalMinMaxTextPropertyWidgetQt<BT, T>::makeEditorWidgets(){
    min_ = new OrdinalEditorWidget<T>();
    max_ = new OrdinalEditorWidget<T>();
    
    BaseOrdinalMinMaxTextPropertyWidgetQt::min_ = min_;
    BaseOrdinalMinMaxTextPropertyWidgetQt::max_ = max_;
}

template <typename BT, typename T>
void OrdinalMinMaxTextPropertyWidgetQt<BT, T>::updateFromProperty() {
    V val = minMaxProperty_->get();
    V range = minMaxProperty_->getRange();
    T inc = minMaxProperty_->getIncrement();
    T sep = minMaxProperty_->getMinSeparation();
    
    min_->blockSignals(true);
    max_->blockSignals(true);

    min_->setRange(range.x, range.y-sep);
    max_->setRange(range.x+sep, range.y);
    
    min_->initValue(val.x);
    max_->initValue(val.y);

    min_->setIncrement(inc);
    max_->setIncrement(inc);

    min_->blockSignals(false);
    max_->blockSignals(false);
}


template <typename BT, typename T>
void OrdinalMinMaxTextPropertyWidgetQt<BT, T>::updateFromMin() {
    T min = min_->getValue();
    T sep = minMaxProperty_->getMinSeparation();
    V range = minMaxProperty_->get();
    
    if (std::abs(min - range.x) > glm::epsilon<T>()) {
        range.x = min;
        
        if (range.y-range.x < sep) {
            range.y = range.x + sep;
            max_->blockSignals(true);
            max_->setValue(range.y);
            max_->blockSignals(false);
        }
        
        minMaxProperty_->setInitiatingWidget(this);
        minMaxProperty_->set(range);
        minMaxProperty_->clearInitiatingWidget();
    }
}

template <typename BT, typename T>
void OrdinalMinMaxTextPropertyWidgetQt<BT, T>::updateFromMax() {
    T max = max_->getValue();
    T sep = minMaxProperty_->getMinSeparation();
    V range = minMaxProperty_->get();
    
    if (std::abs(max - range.y) > glm::epsilon<T>()) {
        range.y = max;
        
        if (range.y - range.x < sep) {
            range.x = range.y - sep;
            min_->blockSignals(true);
            min_->setValue(range.x);
            min_->blockSignals(false);
        }
        
        minMaxProperty_->setInitiatingWidget(this);
        minMaxProperty_->set(range);
        minMaxProperty_->clearInitiatingWidget();
    }
}

} // namespace

#endif // IVW_ORDINALMINMAXTEXTTROPERTYWIDGETQT_H

