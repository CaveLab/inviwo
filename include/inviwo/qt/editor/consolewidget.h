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

#ifndef IVW_CONSOLEWIDGET_H
#define IVW_CONSOLEWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/core/util/logcentral.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>

class QTextEdit;
class QLabel;
class QKeyEvent;

namespace inviwo {

class IVW_QTEDITOR_API ConsoleWidget : public InviwoDockWidget, public Logger {
    #include <warn/push>
    #include <warn/ignore/all>
    Q_OBJECT
    #include <warn/pop>
public:
    ConsoleWidget(QWidget* parent);
    ~ConsoleWidget();

    virtual void log(std::string logSource, LogLevel logLevel, LogAudience audience, const char* fileName,
             const char* functionName, int lineNumber, std::string logMsg) override;

    virtual void logProcessor(std::string processorIdentifier, LogLevel level, LogAudience audience,
                              std::string msg, const char* file, const char* function,
                              int line) override;

    virtual void logNetwork(LogLevel level, LogAudience audience, std::string msg, const char* file,
                            const char* function, int line) override;

private:
    void keyPressEvent(QKeyEvent* keyEvent) override;


    QTextEdit* textField_;
    /// Log level colors
    QColor infoTextColor_;
    QColor warnTextColor_;
    QColor errorTextColor_;

    QLabel* errorsLabel_;
    QLabel* warningsLabel_;
    QLabel* infoLabel_;
    unsigned int numErrors_;
    unsigned int numWarnings_;
    unsigned int numInfos_;

public slots:
    void logMessage(LogLevel level, QString message);
    void logMessage(int level, QString message);
    void showContextMenu(const QPoint& pos);
    void clear();

signals:
    void logMessageSignal(int logLevel, QString message);
    void clearSignal();
};

}  // namespace

#endif  // IVW_CONSOLELISTWIDGET_H