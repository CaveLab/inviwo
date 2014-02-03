 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Erik Sund�n, Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/openglqt/canvasqt.h>


namespace inviwo {

QGLWidget* CanvasQt::sharedWidget_ = NULL;
QGLFormat CanvasQt::sharedFormat_ = QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::AlphaChannel | QGL::DepthBuffer | QGL::StencilBuffer);
bool CanvasQt::sharedInitialized_ = false;

CanvasQt::CanvasQt(QWidget* parent, uvec2 dim)
: QGLWidget(sharedFormat_, parent, sharedWidget_),
  CanvasGL(dim),
  swapBuffersAllowed_(false)
{
    if(sharedWidget_ && !sharedInitialized_){
        sharedWidget_->makeCurrent();
        initializeSquare();
        sharedInitialized_ = true;
    }

    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if (!sharedWidget_){
        QGLFormat f = this->format();
        f.setVersion(10, 0);  //Will choose highest available version
        f.setProfile(QGLFormat::CompatibilityProfile);
        setFormat(f);
        sharedFormat_ = f;
        sharedWidget_ = this;
        QGLWidget::glInit();
    }

    setAutoBufferSwap(false);
	setFocusPolicy(Qt::TabFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
}

void CanvasQt::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasQt::activate() {
    makeCurrent();
}

void CanvasQt::initializeGL() {
    initializeGLEW();
    QGLWidget::initializeGL();
    activate();
}

void CanvasQt::glSwapBuffers(){
    if(swapBuffersAllowed_){
        activate();
        QGLWidget::swapBuffers();
    }
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
    QGLWidget::updateGL();
}

void CanvasQt::paintGL() {
    swapBuffersAllowed_ = true;
    CanvasGL::update();
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
        EventConverterQt::getModifier(e), dimensions_);
    e->accept();
    Canvas::mousePressEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseReleaseEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        EventConverterQt::getModifier(e), dimensions_);
    e->accept();
    Canvas::mouseReleaseEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseMoveEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = NULL;
    if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
            EventConverterQt::getModifier(e), dimensions_);
        e->accept();
        Canvas::mouseMoveEvent(mouseEvent);
        delete mouseEvent;
    }
}

void CanvasQt::keyPressEvent(QKeyEvent* e) {
	if (!processorNetworkEvaluator_) return;
	KeyboardEvent* keyEvent = new KeyboardEvent( 
		EventConverterQt::getKeyButton(e),  
		EventConverterQt::getModifier(e),
		KeyboardEvent::KEY_STATE_PRESS);
    e->accept();
	Canvas::keyPressEvent(keyEvent);
    delete keyEvent;
}

void CanvasQt::keyReleaseEvent(QKeyEvent* e) {
	if (!processorNetworkEvaluator_) return;
	KeyboardEvent* keyEvent = new KeyboardEvent( 
		EventConverterQt::getKeyButton(e),  
		EventConverterQt::getModifier(e),
		KeyboardEvent::KEY_STATE_RELEASE);
    e->accept();
	Canvas::keyReleaseEvent(keyEvent);
    delete keyEvent;
}

} // namespace