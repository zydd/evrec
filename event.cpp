#include <QApplication>
#include <QWidget>
#include "event.h"

QString objectPath(QObject *obj) {
    QString path;
    while (obj) {
        if (obj->objectName().isEmpty())
            return QString();
        path.prepend("/" + obj->objectName());
        obj = obj->parent();
    }
    return path;
}

QObject *findObject(QString path) {
    QStringList nodes = path.split('/',QString::SkipEmptyParts);
    if (nodes.isEmpty())
        return NULL;

    foreach (QWidget *parent, QApplication::topLevelWidgets()) {
        if (!parent->objectName().isEmpty()) {
            QList<QObject *> objects = parent->findChildren<QObject *>(nodes.last());

            if (nodes.size() == 1 && parent->objectName() == nodes.at(0))
                return parent;

            foreach (QObject *x, objects) {
                if (objectPath(x) == path)
                    return x;
            }
        }
    }
    return NULL;
}

Event::Event(Type type, QInputEvent *event, QString receiver, qint64 delay) {
    this->m_type = type;
    this->m_event = event;
    this->m_receiver = receiver;
    this->m_delay = delay;
}

qint64 Event::delay() const {
    return m_delay;
}

QInputEvent *Event::event() const {
    return m_event;
}

QString Event::path() const {
    return m_receiver;
}

static void operator << (QDataStream &out, const QTouchEvent::TouchPoint &touchPoint) {
    out << (qint32) touchPoint.id();
    out << touchPoint.isPrimary();
    out << touchPoint.lastNormalizedPos();
    out << touchPoint.lastPos();
    out << touchPoint.lastScenePos();
    out << touchPoint.lastScreenPos();
    out << touchPoint.normalizedPos();
    out << touchPoint.pos();
    out << touchPoint.pressure();
    out << touchPoint.rect();
    out << touchPoint.scenePos();
    out << touchPoint.sceneRect();
    out << touchPoint.screenPos();
    out << touchPoint.screenRect();
    out << touchPoint.startNormalizedPos();
    out << touchPoint.startPos();
    out << touchPoint.startScenePos();
    out << touchPoint.startScreenPos();
    out << (qint32) touchPoint.state();
}

static void operator >> (QDataStream &out, const QTouchEvent::TouchPoint &/*touchPoint*/) {
    qint32 id, state;
    bool isPrimary;
    QPointF lastNormalizedPos, lastPos, lastScenePos, lastScreenPos, normalizedPos, pos, scenePos, screenPos, startNormalizedPos, startPos, startScenePos, startScreenPos;
    QRectF rect, sceneRect, screenRect;
    qreal pressure;
    out >> id;
    out >> isPrimary;
    out >> lastNormalizedPos;
    out >> lastPos;
    out >> lastScenePos;
    out >> lastScreenPos;
    out >> normalizedPos;
    out >> pos;
    out >> pressure;
    out >> rect;
    out >> scenePos;
    out >> sceneRect;
    out >> screenPos;
    out >> screenRect;
    out >> startNormalizedPos;
    out >> startPos;
    out >> startScenePos;
    out >> startScreenPos;
    out >> state;
}

QDataStream &operator<< (QDataStream &out, const Event &ev) {
    out << (qint32) ev.m_type;
    out << ev.m_receiver;
    out << ev.m_delay;

    switch(ev.m_type) {
    case Event::ContextMenuEvent: {
        QContextMenuEvent *event = static_cast<QContextMenuEvent *>(ev.m_event);
        out << (qint32) event->reason();
        out << event->pos();
        out << event->globalPos();
        out << (qint32) event->modifiers();
        break;
    }
    case Event::KeyEvent: {
        QKeyEvent *event = static_cast<QKeyEvent *>(ev.m_event);
        out << (qint32) event->type();
        out << (qint32) event->key();
        out << (qint32) event->modifiers();
        out << event->text();
        out << event->isAutoRepeat();
        out << (qint32) event->count();
        break;
    }
    case Event::MouseEvent: {
        QMouseEvent *event = static_cast<QMouseEvent *>(ev.m_event);
        out << (qint32) event->type();
        out << event->pos();
        out << event->globalPos();
        out << (qint32) event->button();
        out << (qint32) event->buttons();
        out << (qint32) event->modifiers();
        break;
    }
    case Event::TabletEvent: {
        QTabletEvent *event = static_cast<QTabletEvent *>(ev.m_event);
        out << (qint32) event->type();
        out << event->pos();
        out << event->globalPos();
        out << event->hiResGlobalPos();
        out << (qint32) event->device();
        out << (qint32) event->pointerType();
        out << event->pressure();
        out << (qint32) event->xTilt();
        out << (qint32) event->yTilt();
        out << event->tangentialPressure();
        out << event->rotation();
        out << (qint32) event->z();
        out << (qint32) event->modifiers();
        out << event->uniqueId();
        break;
    }
    case Event::TouchEvent: {
        QTouchEvent *event = static_cast<QTouchEvent *>(ev.m_event);
        out << (qint32) event->type();
        out << (qint32) event->deviceType();
        out << (qint32) event->modifiers();
        out << (qint32) event->touchPointStates();
        out << event->touchPoints();
    }
    case Event::WheelEvent: {
        QWheelEvent *event = static_cast<QWheelEvent *>(ev.m_event);
        out << event->pos();
        out << event->globalPos();
        out << (qint32) event->delta();
        out << (qint32) event->buttons();
        out << (qint32) event->modifiers();
        out << (qint32) event->orientation();
        break;
    }
    }
    return out;
}

Event::Event(QDataStream &out) {
    qint32 type;
    out >> type;
    out >> m_receiver;
    out >> m_delay;
    m_type = (Type) type;
    switch(m_type) {
    case ContextMenuEvent: {
        qint32 reason, modifiers;
        QPoint pos, globalPos;

        out >> reason;
        out >> pos;
        out >> globalPos;
        out >> modifiers;
        m_event = new QContextMenuEvent((QContextMenuEvent::Reason) reason,pos,globalPos,(Qt::KeyboardModifiers) modifiers);
        break;
    }
    case KeyEvent: {
        qint32 type, key, modifiers, count;
        QString text;
        bool isAutoRepeat;

        out >> type;
        out >> key;
        out >> modifiers;
        out >> text;
        out >> isAutoRepeat;
        out >> count;
        m_event = new QKeyEvent((QEvent::Type) type,key,(Qt::KeyboardModifiers) modifiers,text,isAutoRepeat,count);
        break;
    }
    case MouseEvent: {
        qint32 type, button, buttons, modifiers;
        QPoint pos, globalPos;

        out >> type;
        out >> pos;
        out >> globalPos;
        out >> button;
        out >> buttons;
        out >> modifiers;
        m_event = new QMouseEvent((QEvent::Type) type,pos,globalPos,(Qt::MouseButton) button,(Qt::MouseButton) buttons,(Qt::KeyboardModifiers) modifiers);
        break;
    }
    case TabletEvent: {
        qint32 type, device, pointerType, xTilt, yTilt, z, modifiers;
        qint64 uniqueId;
        qreal pressure, tangentialPressure, rotation;
        QPoint pos, globalPos;
        QPointF hiResGlobalPos;

        out >> type;
        out >> pos;
        out >> globalPos;
        out >> hiResGlobalPos;
        out >> device;
        out >> pointerType;
        out >> pressure;
        out >> xTilt;
        out >> yTilt;
        out >> tangentialPressure;
        out >> rotation;
        out >> z;
        out >> modifiers;
        out >> uniqueId;
        m_event = new QTabletEvent((QEvent::Type) type,pos,globalPos,hiResGlobalPos,device,pointerType,pressure,xTilt,yTilt,tangentialPressure,rotation,z,(Qt::KeyboardModifiers) modifiers,uniqueId);
        break;
    }
    case TouchEvent: {
        qint32 type, deviceType, modifiers, touchPointStates;
        QList<QTouchEvent::TouchPoint> touchPoints;

        out >> type;
        out >> deviceType;
        out >> modifiers;
        out >> touchPointStates;
        out >> touchPoints;
        m_event = new QTouchEvent((QEvent::Type) type,(QTouchEvent::DeviceType) deviceType,(Qt::KeyboardModifiers) modifiers,(Qt::TouchPointState) touchPointStates,touchPoints);
    }
    case WheelEvent: {
        qint32 delta, buttons, modifiers, orientation;
        QPoint pos, globalPos;

        out >> pos;
        out >> globalPos;
        out >> delta;
        out >> buttons;
        out >> modifiers;
        out >> orientation;
        m_event = new QWheelEvent(pos,globalPos,delta,(Qt::MouseButton) buttons,(Qt::KeyboardModifiers) modifiers,(Qt::Orientation) orientation);
        break;
    }
    }
}

Event::~Event() {
    delete m_event;
}
