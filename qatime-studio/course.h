#ifndef COURSE_H
#define COURSE_H

#include <QJsonObject>
#include <QJsonArray>

class Course
{
public:
    Course();

    QString id();
    QString name() const;
	QString progress() const;
    int		status();
    void	readJson(const QJsonObject &json);
	QJsonArray JsonLesson();

private:
    int mId;
    QString mName;				// ����������
    int mStatus;
	QString mProgress;			// ����
	QJsonArray mJsonArray;		// �γ�json
};

#endif // COURSE_H
