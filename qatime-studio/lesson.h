#ifndef LESSON_H
#define LESSON_H

#include <QJsonObject>

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
private:
    int mId;
    int mCourseId;
    QString mName;			// �γ�����
	QString mStatus;	// �γ�״̬
	QString mStartTime;		// �Ͽ�ʱ��
};

#endif // LESSON_H
