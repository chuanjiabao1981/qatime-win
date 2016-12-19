#include "webobjectinstance.h"

WebObjectInstance *WebObjectInstance::instance()
{
    static WebObjectInstance obj;
    return &obj;
}

void WebObjectInstance::slotJsUpdated(int x, int y)
{
    emit signalCursorMoved(x,y);
}

void WebObjectInstance::slotJsClick(const QVariant& var)
{
	emit signClick(var);
}

void WebObjectInstance::slotCplusplusUpdate(const QVariant &var)
{

}

WebObjectInstance::WebObjectInstance(QObject *parent) : QObject(parent)
{

}
