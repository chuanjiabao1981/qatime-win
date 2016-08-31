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
    QString mName;				// ����������
    int mStatus;
	QString mProgress;			// ����
};

#endif // COURSE_H
