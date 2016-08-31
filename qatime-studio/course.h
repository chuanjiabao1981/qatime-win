#ifndef COURSE_H
#define COURSE_H

#include <QJsonObject>

class Course
{
public:
    Course();

    QString id();
    QString name() const;
	QString progress() const;
    int status();

    void readJson(const QJsonObject &json);

private:
    int mId;
    QString mName;				// 辅导班名称
    int mStatus;
	QString mProgress;			// 进度
};

#endif // COURSE_H
