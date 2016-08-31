#ifndef LESSON_H
#define LESSON_H

#include <QJsonObject>

#pragma execution_character_set("utf-8")

class Lesson
{
public:
    Lesson();
    void readJson(const QJsonObject &json);
    int courseId();
    void setCourseId(int courseId);
    QString name();
	QString LessonTime();
	QString LessonStatus();
	QString ChinaLessonStatus();
private:
    int mId;
    int mCourseId;
    QString mName;			// �γ�����
	QString mStatus;	// �γ�״̬
	QString mStartTime;		// �Ͽ�ʱ��
};

#endif // LESSON_H
