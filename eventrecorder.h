#ifndef EVENTRECORDER_H
#define EVENTRECORDER_H

#include <QInputEvent>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QTimer>

#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include "event.h"

class EventRecorder : public QDialog {
    Q_OBJECT
public:
    EventRecorder(QWidget *parent = 0);
    bool eventFilter(QObject *rec, QEvent *ev);

private:
    QHBoxLayout *layout;
    QPushButton *buttonRecord;
    QPushButton *buttonPlay;
    QPushButton *buttonStop;
    QList<Event *> events;
    QElapsedTimer elapsedTime;
    QTimer timer;
    Event *currentEvent;

signals:

public slots:
    void record();
    void stop();
    void replay();
    void update();
    void save(QDataStream &ds);
    void load(QDataStream &ds);
};

#endif // EVENTRECORDER_H
