/*
 * Copyright (c) 2015 Mark Liversedge (liversedge@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "WorkoutWindow.h"
#include "WorkoutWidget.h"
#include "WorkoutWidgetItems.h"

WorkoutWindow::WorkoutWindow(Context *context) :
    GcWindow(context), context(context), active(false)
{
    setContentsMargins(0,0,0,0);
    setProperty("color", GColor(CTRAINPLOTBACKGROUND));

    setControls(NULL);

    QVBoxLayout *layout = new QVBoxLayout(this);

    connect(context, SIGNAL(configChanged(qint32)), this, SLOT(configChanged(qint32)));

    // the workout scene
    workout = new WorkoutWidget(this, context);

    // add a scale
    powerscale = new WWPowerScale(workout, context);

    // add a line between the dots
    line = new WWLine(workout);

    // setup the toolbar
    toolbar = new QToolBar(this);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->setFloatable(true);
    toolbar->setIconSize(QSize(18,18));

    QIcon saveIcon(":images/toolbar/save.png");
    saveAct = new QAction(saveIcon, tr("Save"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
    toolbar->addAction(saveAct);

    toolbar->addSeparator();

    //XXX TODO
    //XXXHelpWhatsThis *helpToolbar = new HelpWhatsThis(toolbar);
    //XXXtoolbar->setWhatsThis(helpToolbar->getWhatsThisText(HelpWhatsThis::ChartRides_Editor));

    // undo and redo deliberately at a distance from the
    // save icon, since accidentally hitting the wrong
    // icon in that instance would be horrible
    QIcon undoIcon(":images/toolbar/undo.png");
    undoAct = new QAction(undoIcon, tr("Undo"), this);
    connect(undoAct, SIGNAL(triggered()), workout, SLOT(undo()));
    toolbar->addAction(undoAct);

    QIcon redoIcon(":images/toolbar/redo.png");
    redoAct = new QAction(redoIcon, tr("Redo"), this);
    connect(redoAct, SIGNAL(triggered()), workout, SLOT(redo()));
    toolbar->addAction(redoAct);
    
    toolbar->addSeparator();

    QIcon drawIcon(":images/toolbar/edit.png");
    drawAct = new QAction(drawIcon, tr("Draw"), this);
    connect(drawAct, SIGNAL(triggered()), this, SLOT(drawMode()));
    toolbar->addAction(drawAct);

    QIcon selectIcon(":images/toolbar/select.png");
    selectAct = new QAction(selectIcon, tr("Select"), this);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(selectMode()));
    toolbar->addAction(selectAct);

    // stretch the labels to the right hand side
    QWidget *empty = new QWidget(this);
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    toolbar->addWidget(empty);


    xlabel = new QLabel("00:00");
    toolbar->addWidget(xlabel);

    ylabel = new QLabel("150w");
    toolbar->addWidget(ylabel);

#if 0 // not yet!
    // get updates..
    connect(context, SIGNAL(telemetryUpdate(RealtimeData)), this, SLOT(telemetryUpdate(RealtimeData)));
    telemetryUpdate(RealtimeData());
#endif

    // WATTS and Duration for the cursor
    layout->addWidget(toolbar);
    layout->addWidget(workout);

    // make it look right
    saveAct->setEnabled(false);
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    configChanged(CONFIG_APPEARANCE);
}

void
WorkoutWindow::configChanged(qint32)
{
    setProperty("color", GColor(CTRAINPLOTBACKGROUND));
    QFontMetrics fm(workout->bigFont);
    xlabel->setFont(workout->bigFont);
    ylabel->setFont(workout->bigFont);
    xlabel->setFixedWidth(fm.boundingRect(" 00:00:00 ").width());
    ylabel->setFixedWidth(fm.boundingRect(" 1000w ").width());

    toolbar->setStyleSheet(QString("::enabled { background: %1; color: %2; border: 0px; } ").arg(GColor(CPLOTBACKGROUND).name())
                    .arg(GCColor::invertColor(GColor(CPLOTBACKGROUND)).name()));
    xlabel->setStyleSheet("color: darkGray;");
    ylabel->setStyleSheet("color: darkGray;");
    repaint();
}

void
WorkoutWindow::saveFile()
{
}

void
WorkoutWindow::drawMode()
{
}

void
WorkoutWindow::selectMode()
{
}
