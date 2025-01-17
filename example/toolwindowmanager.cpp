/****************************************************************************
**
** Copyright (C) 2014 Pavel Strakhov <ri@idzaaus.org>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QDebug>
#include "toolwindowmanager.h"
#include "ui_toolwindowmanager.h"

#include "terminalwidget.h"


ToolWindowManager::ToolWindowManager(QWidget *parent,int argc, char **argv) :
    QMainWindow(parent),
    ui(new Ui::ToolWindowManager)
{
    ui->setupUi(this);
 /*
    connect(ui->toolWindowManager, SIGNAL(toolWindowVisibilityChanged(QWidget*,bool)),
            this, SLOT(toolWindowVisibilityChanged(QWidget*,bool)));
 */
    QList<TerminalWidget*> toolWindows;
      TerminalWidget *b1 = new TerminalWidget(this,false);
        if ( argc>1) {
                b1->setShellProgram(argv[1]);
		QStringList larg ;
                for(int n=2;n < argc; n++) larg << argv[n];
                b1->setShellProgramArguments(larg);
                b1->startShellProgram();
        	b1->setWindowTitle(argv[1]);
        	b1->setObjectName(argv[1]);
        } else {
        b1->setWindowTitle("Term "+QString::number(1));
        b1->setObjectName("Term "+QString::number(1));
        }
	lstTerminal << b1;
        toolWindows << b1;
/*
    for (int i = 0; i < 1; i++) {
        //auto b1 = new TerminalWidget(this,false);
        TerminalWidget *b1 = new TerminalWidget(this,false);
        b1->setWindowTitle("Term "+QString::number(i));
        b1->setObjectName("Term "+QString::number(i));
        
        QAction *action = ui->menuToolWindows->addAction(QString::number(i));
        action->setData(i);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(toolWindowActionToggled(bool)));
        action->setCheckable(true);
        action->setChecked(true);
        actions << action;
        toolWindows << b1;
    } 
 */
    //auto t1 = new TerminalWidget(this,true);
    ui->toolWindowManager->addToolWindow(toolWindows[0], QToolWindowManager::LastUsedArea);
    //ui->toolWindowManager->addToolWindow(toolWindows[1], QToolWindowManager::LastUsedArea);
    /*
    ui->toolWindowManager->addToolWindow(toolWindows[2], QToolWindowManager::LastUsedArea);
    ui->toolWindowManager->addToolWindow(toolWindows[3],
            QToolWindowManager::ReferenceLeftOf, ui->toolWindowManager->areaFor(toolWindows[2]));
    ui->toolWindowManager->addToolWindow(toolWindows[4], QToolWindowManager::LastUsedArea);
    ui->toolWindowManager->addToolWindow(toolWindows[5],
            QToolWindowManager::ReferenceTopOf, ui->toolWindowManager->areaFor(toolWindows[4]));

    QToolButton* tabButton1 = new QToolButton();
    tabButton1->setText(tr("t1"));
    ui->toolWindowManager->setTabButton(toolWindows[2], QTabBar::LeftSide, tabButton1);

    QToolButton* tabButton2 = new QToolButton();
    tabButton2->setText(tr("t2"));

    ui->toolWindowManager->setTabButton(toolWindows[3], QTabBar::LeftSide, tabButton2);
     */
    resize(800, 600);
    on_actionRestoreState_triggered();
}

ToolWindowManager::~ToolWindowManager()
{
    delete ui;
}

void ToolWindowManager::toolWindowActionToggled(bool state)
{
    qDebug()<<" toolWindowActionToggled " <<state;
    int index = static_cast<QAction*>(sender())->data().toInt();
    QWidget *toolWindow = ui->toolWindowManager->toolWindows()[index];
    ui->toolWindowManager->moveToolWindow(toolWindow, state ?
                                              QToolWindowManager::LastUsedArea :
                                              QToolWindowManager::NoArea);

}

void ToolWindowManager::toolWindowVisibilityChanged(QWidget *toolWindow, bool visible)
{
    int index = ui->toolWindowManager->toolWindows().indexOf(toolWindow);
    actions[index]->blockSignals(true);
    actions[index]->setChecked(visible);
    actions[index]->blockSignals(false);
}

void ToolWindowManager::on_actionSaveState_triggered()
{
    QSettings settings;
    settings.setValue("toolWindowManagerState", ui->toolWindowManager->saveState());
    settings.setValue("geometry", saveGeometry());
}

void ToolWindowManager::on_actionRestoreState_triggered()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->toolWindowManager->restoreState(settings.value("toolWindowManagerState"));
}

void ToolWindowManager::on_actionClearState_triggered()
{
    QSettings settings;
    settings.remove("geometry");
    settings.remove("toolWindowManagerState");
    QApplication::quit();
}

void ToolWindowManager::on_actionClosableTabs_toggled(bool checked)
{
    ui->toolWindowManager->setTabsClosable(checked);
}

void ToolWindowManager::on_actionNewTerminal_triggered()
{
    TerminalWidget *b1 = new TerminalWidget(this,true);
    ui->toolWindowManager->addToolWindow(b1, QToolWindowManager::LastUsedArea);
}


void ToolWindowManager::receivedMessage(int instanceId, QByteArray message)
{
    qDebug() << "Received message from instance: " << instanceId;
    QStringList tmp = QString::fromUtf8( message ).split(" ");
    TerminalWidget *b1 = new TerminalWidget(this,false);
    if (tmp.length() >1 ) 
    {
      b1->setShellProgram(tmp[1]);
      QStringList larg ;
      for(int n=2;n < tmp.length(); n++) larg << tmp[n];
      b1->setShellProgramArguments(larg);
      b1->setWindowTitle(tmp[1]);
      b1->setObjectName(tmp[1]);
    }
    lstTerminal << b1;
    b1->startShellProgram();
    ui->toolWindowManager->addToolWindow(b1, QToolWindowManager::LastUsedArea);
}

