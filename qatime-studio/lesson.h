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
	QString LessonID();
private:
    int mId;
    int mCourseId;
    QString mName;			// 课程名称
	QString mStatus;	// 课程状态
	QString mStartTime;		// 上课时间
};

#endif // LESSON_H
