/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2017 Inviwo Foundation
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

#include <modules/qtwidgets/properties/transferfunctioneditorcontrolpoint.h>
#include <modules/qtwidgets/properties/transferfunctioncontrolpointconnection.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>
#include <modules/qtwidgets/properties/transferfunctioneditor.h>
#include <modules/qtwidgets/properties/transferfunctioneditorview.h>
#include <warn/push>
#include <warn/ignore/all>
#include <QTextStream>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QKeyEvent>
#include <warn/pop>

namespace inviwo {

TransferFunctionEditorControlPoint::TransferFunctionEditorControlPoint(
    TransferFunctionDataPoint* datapoint, const DataMapper& dataMap, float size)
    : QGraphicsItem()
    , left_(nullptr)
    , right_(nullptr)
    , size_(size)
    , showLabel_(false)
    , isEditingPoint_(false)
    , dataPoint_(datapoint)
    , dataMap_(dataMap)
    , currentPos_() {
    setFlags(ItemIgnoresTransformations | ItemIsFocusable | ItemIsMovable | ItemIsSelectable |
             ItemSendsGeometryChanges);
    setZValue(1);
    setAcceptHoverEvents(true);
    datapoint->addObserver(this);
}

void TransferFunctionEditorControlPoint::paint(QPainter* painter,
                                               const QStyleOptionGraphicsItem* options,
                                               QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen = QPen();
    pen.setWidthF(3);
    pen.setCosmetic(true);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    isSelected() ? pen.setColor(QColor(213, 79, 79)) : pen.setColor(QColor(66, 66, 66));
    QBrush brush = QBrush(QColor::fromRgbF(dataPoint_->getRGBA().r,
                                           dataPoint_->getRGBA().g,
                                           dataPoint_->getRGBA().b));
    painter->setPen(pen);
    painter->setBrush(brush);
    int c = static_cast<int>(-size_ / 2.0f);
    int s = static_cast<int>(size_);
    painter->drawEllipse(c, c, s, s);

    if (showLabel_) {
        QString label;
        QTextStream labelStream(&label);
        labelStream.setRealNumberPrecision(3);
        labelStream << "a("
                    << dataMap_.valueRange.x +
                           dataPoint_->getPos().x * (dataMap_.valueRange.y - dataMap_.valueRange.x)
                    << ")=";
        labelStream << dataPoint_->getRGBA().a;

        Qt::AlignmentFlag align;
        if (dataPoint_->getPos().x > 0.5f) {
            align = Qt::AlignRight;
        } else {
            align = Qt::AlignLeft;
        }
        QRectF rect = calculateLabelRect();

        pen.setColor(QColor(46, 46, 46));
        painter->setPen(pen);
        QFont font;
        font.setPixelSize(14);
        painter->setFont(font);
        painter->drawText(rect, align, label);
    }
}

QRectF TransferFunctionEditorControlPoint::boundingRect() const {
    float bBoxSize = size_ + 5.0f;
    if (showLabel_) {
        QRectF rect = calculateLabelRect();
        return rect.united(QRectF(-bBoxSize / 2.0, -bBoxSize / 2.0f, bBoxSize, bBoxSize));
    } else {
        return QRectF(-bBoxSize / 2.0, -bBoxSize / 2.0f, bBoxSize, bBoxSize);
    }
}

QPainterPath TransferFunctionEditorControlPoint::shape() const {
    QPainterPath path;
    path.addEllipse(QRectF(-size_ / 2.0, -size_ / 2.0f, size_, size_));
    return path;
}

void TransferFunctionEditorControlPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    prepareGeometryChange();
    size_ += 5.0f;
    showLabel_ = true;
    update();
}

void TransferFunctionEditorControlPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    prepareGeometryChange();
    size_ -= 5.0f;
    showLabel_ = false;
    update();
}

QVariant TransferFunctionEditorControlPoint::itemChange(GraphicsItemChange change,
                                                        const QVariant& value) {
    TransferFunctionEditor* tfe = qobject_cast<TransferFunctionEditor*>(scene());

    if (change == QGraphicsItem::ItemPositionChange && tfe) {
        // constrain positions to valid view positions
        currentPos_ = value.toPointF();
        QRectF rect = scene()->sceneRect();

        int moveMode = tfe->getMoveMode();

        if (!rect.contains(currentPos_)) {
            currentPos_.setX(qMin(rect.right(), qMax(currentPos_.x(), rect.left())));
            currentPos_.setY(qMin(rect.bottom(), qMax(currentPos_.y(), rect.top())));
        }

        float d = 2.0f * static_cast<float>(rect.width()) * std::numeric_limits<float>::epsilon();

        if (left_) {
            if (left_->left_ && *(left_->left_) > *this) {
                switch (moveMode) {
                    case 0:  // Free
                        break;
                    case 1:  // Restrict
                        currentPos_.setX(left_->left_->getCurrentPos().x() + d);
                        break;
                    case 2:  // Push
                        left_->left_->setPos(
                            QPointF(currentPos_.x() - d, left_->left_->getCurrentPos().y()));
                        break;
                }

                tfe->updateConnections();
            } else {
                left_->updateShape();
            }
        }
        if (right_) {
            if (right_->right_ && *(right_->right_) < *this) {
                switch (moveMode) {
                    case 0:  // Free
                        break;
                    case 1:  // Restrict
                        currentPos_.setX(right_->right_->getCurrentPos().x() - d);
                        break;
                    case 2:  // Push
                        right_->right_->setPos(
                            QPointF(currentPos_.x() + d, right_->right_->getCurrentPos().y()));
                        break;
                }
                tfe->updateConnections();
            } else {
                right_->updateShape();
            }
        }

        // update the associated transfer function data point
        if (!isEditingPoint_) {
            isEditingPoint_ = true;
            dataPoint_->setPosA(
                vec2(static_cast<float>( currentPos_.x() / rect.width()), static_cast<float>(currentPos_.y() / rect.height())),
                static_cast<float>(currentPos_.y() / rect.height()));
            isEditingPoint_ = false;
        }

        // return the constraint position
        return currentPos_;
    } else if (change == QGraphicsItem::ItemSceneHasChanged) {
        onTransferFunctionPointChange(dataPoint_);
    }

    return QGraphicsItem::itemChange(change, value);
}

void TransferFunctionEditorControlPoint::onTransferFunctionPointChange(
    const TransferFunctionDataPoint* p) {
    if (!isEditingPoint_) {
        isEditingPoint_ = true;
        QRectF rect = scene()->sceneRect();
        QPointF newpos(p->getPos().x * rect.width(), p->getPos().y * rect.height());
        if (newpos != pos()) setPos(newpos);
        isEditingPoint_ = false;
    }
}

QRectF TransferFunctionEditorControlPoint::calculateLabelRect() const {
    QRectF rect;
    if (dataPoint_->getPos().x > 0.5f) {
        rect.setX(-0.5 * size_ - textWidth_);
    } else {
        rect.setX(0.5 * size_);
    }
    if (dataPoint_->getPos().y > 0.5f) {
        rect.setY(0.5 * size_);
    } else {
        rect.setY(-0.5 * size_ - textHeight_);
    }
    rect.setHeight(textHeight_);
    rect.setWidth(textWidth_);
    return rect;
}

void TransferFunctionEditorControlPoint::setDataMap(const DataMapper& dataMap) {
    dataMap_ = dataMap;
}

inviwo::DataMapper TransferFunctionEditorControlPoint::getDataMap() const {
    return dataMap_;
}

void TransferFunctionEditorControlPoint::setDataPoint(TransferFunctionDataPoint* dataPoint) {
    dataPoint_ = dataPoint;
}

TransferFunctionDataPoint* TransferFunctionEditorControlPoint::getPoint() const {
    return dataPoint_;
}

void TransferFunctionEditorControlPoint::setPos(const QPointF & pos) {
    currentPos_ = pos;
    QGraphicsItem::setPos(pos);
}

const QPointF& TransferFunctionEditorControlPoint::getCurrentPos() const{
    return currentPos_;
}


bool operator==(const TransferFunctionEditorControlPoint& lhs,
                const TransferFunctionEditorControlPoint& rhs) {
    return *lhs.dataPoint_ == *rhs.dataPoint_;
}

bool operator!=(const TransferFunctionEditorControlPoint& lhs,
                const TransferFunctionEditorControlPoint& rhs) {
    return !operator==(lhs, rhs);
}

bool operator<(const TransferFunctionEditorControlPoint& lhs,
               const TransferFunctionEditorControlPoint& rhs) {
    return lhs.currentPos_.x() < rhs.currentPos_.x();
}

bool operator>(const TransferFunctionEditorControlPoint& lhs,
               const TransferFunctionEditorControlPoint& rhs) {
    return rhs < lhs;
}

bool operator<=(const TransferFunctionEditorControlPoint& lhs,
                const TransferFunctionEditorControlPoint& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const TransferFunctionEditorControlPoint& lhs,
                const TransferFunctionEditorControlPoint& rhs) {
    return !(lhs < rhs);
}

const double TransferFunctionEditorControlPoint::textHeight_ = 20;
const double TransferFunctionEditorControlPoint::textWidth_ = 180;

}  // namespace



