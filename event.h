#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QInputEvent>
#include <QDebug>
#include <QCoreApplication>
#include <QStringList>
#include <QDataStream>

class Event {
public:
    enum Type {
        ContextMenuEvent = 1, KeyEvent, MouseEvent, TabletEvent, TouchEvent, WheelEvent
    };

    Event(Type m_type, QInputEvent *m_event, QString m_receiver, qint64 m_delay = 100);
    Event(QDataStream &out);
    qint64 delay() const;
    QInputEvent *event() const;
    QString path() const;
    friend QDataStream &operator << (QDataStream &out, const Event &ev);
//    friend QDataStream operator >> (QDataStream &out, Event &ev);
    ~Event();

private:
    qint64 m_delay;
    QInputEvent *m_event;
    QString m_receiver;
    Type m_type;
};

QString objectPath(QObject *obj);
QObject *findObject(QString path);

#endif // EVENT_H
