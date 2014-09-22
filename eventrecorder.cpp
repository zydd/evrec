#include <QPushButton>
#include "eventrecorder.h"

EventRecorder::EventRecorder(QWidget *parent):
    QDialog(parent), currentEvent(NULL)
{
    setWindowTitle(tr("Recorder"));
    buttonRecord = new QPushButton(tr("&Record"));
    buttonPlay = new QPushButton(tr("&Play"));
    buttonPlay->setDisabled(true);
    buttonStop = new QPushButton(tr("&Stop"));
    buttonStop->setDisabled(true);

    layout = new QHBoxLayout;
    layout->addWidget(buttonRecord);
    layout->addWidget(buttonPlay);
    layout->addWidget(buttonStop);
    this->setLayout(layout);

    connect(buttonRecord,SIGNAL(pressed()),SLOT(record()));
    connect(buttonPlay,SIGNAL(pressed()),SLOT(replay()));
    connect(buttonStop,SIGNAL(pressed()),SLOT(stop()));

    elapsedTime.start();
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));

    QFile file("EvRec");
    if (file.open(QFile::ReadOnly)) {
        QDataStream ds(&file);
        load(ds);
        file.close();
    }
}

bool EventRecorder::eventFilter(QObject *rec, QEvent *ev) {
//    if (ev->type() == QEvent::MouseMove)
//        return false;
    Event::Type evtype;
    QInputEvent *newev;
    if (dynamic_cast<QMouseEvent*>(ev)) {
        evtype = Event::MouseEvent;
        newev = new QMouseEvent(*static_cast<QMouseEvent*>(ev));
    } else if (dynamic_cast<QKeyEvent*>(ev)) {
        evtype = Event::KeyEvent;
        newev = new QKeyEvent(*static_cast<QKeyEvent*>(ev));
    } else if (dynamic_cast<QWheelEvent*>(ev)) {
        evtype = Event::WheelEvent;
        newev = new QWheelEvent(*static_cast<QWheelEvent*>(ev));
    } else if (dynamic_cast<QContextMenuEvent*>(ev)) {
        evtype = Event::ContextMenuEvent;
        newev = new QContextMenuEvent(*static_cast<QContextMenuEvent*>(ev));
    } else if (dynamic_cast<QTabletEvent*>(ev)) {
        evtype = Event::TabletEvent;
        newev = new QTabletEvent(*static_cast<QTabletEvent*>(ev));
   } else  if (dynamic_cast<QTouchEvent*>(ev)) {
        evtype = Event::TouchEvent;
        newev = new QTouchEvent(*static_cast<QTouchEvent*>(ev));
    } else {
        return false;
    }

    QString path = objectPath(rec);

    if (!path.isEmpty()) {
        events.append(new Event(evtype, newev, path, elapsedTime.elapsed()));
        elapsedTime.restart();
    }
    return false;
}

void EventRecorder::replay() {
    stop();
    if (! events.isEmpty()) {
        currentEvent = events.first();
        timer.start(currentEvent->delay());

        buttonPlay->setDisabled(true);
        buttonRecord->setDisabled(true);
        buttonStop->setEnabled(true);
    }
}

void EventRecorder::update() {
    qApp->postEvent(findObject(currentEvent->path()),currentEvent->event());
    events.removeFirst();

    if(!events.isEmpty()) {
        currentEvent = events.first();
        timer.start(currentEvent->delay());
    } else {
        currentEvent = NULL;
        buttonStop->setDisabled(true);
        buttonRecord->setEnabled(true);
        QFile file("EvRec");
        if (file.open(QFile::ReadOnly)) {
            QDataStream ds(&file);
            load(ds);
            file.close();
        }
    }
}

void EventRecorder::record() {
    qApp->installEventFilter(this);
    events.clear();
    elapsedTime.restart();

    buttonRecord->setDisabled(true);
    buttonPlay->setDisabled(true);
    buttonStop->setEnabled(true);
}

void EventRecorder::stop() {
    qApp->removeEventFilter(this);
    timer.stop();

    buttonRecord->setEnabled(true);
    buttonStop->setDisabled(true);
    if (!events.isEmpty()) {
        QFile file("EvRec");
        file.open(QFile::WriteOnly);
        QDataStream ds(&file);
        save(ds);
        file.close();
        buttonPlay->setEnabled(true);
    }
}

void EventRecorder::save(QDataStream &ds) {
    foreach(Event *x, events)
        ds << *x;
}

void EventRecorder::load(QDataStream &ds) {
    events.clear();
    while(! ds.atEnd())
        events.append(new Event(ds));
    if (!events.isEmpty())
        buttonPlay->setEnabled(true);
}
