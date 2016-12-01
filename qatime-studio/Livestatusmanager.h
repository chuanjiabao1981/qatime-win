#ifndef LIVESTATUSMANAGER_H
#define LIVESTATUSMANAGER_H

#include <QObject>

class LiveStatusManager : public QObject
{
	Q_OBJECT

public:
	LiveStatusManager(QObject *parent);
	~LiveStatusManager();

private:
	
};

#endif // LIVESTATUSMANAGER_H
