#include "lesson.h"

Lesson::Lesson()
{

}

void Lesson::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
	mStatus = json["status"].toString();
    
	// ÉÏ¿ÎÊ±¼ä
	mStartTime = json["class_date"].toString();
	mStartTime += " ";
	mStartTime += json["live_time"].toString();
}

int Lesson::courseId()
{
    return mCourseId;
}

void Lesson::setCourseId(int courseId)
{
    mCourseId = courseId;
}

QString Lesson::name()
{
    return mName;
}

QString Lesson::LessonTime()
{
	return mStartTime;
}

QString Lesson::LessonStatus()
{
	return mStatus;
}