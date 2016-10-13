#include "lesson.h"
#include "define.h"

Lesson::Lesson()
{

}

void Lesson::readJson(const QJsonObject &json)
{
	// �γ�ID
    mId = json["id"].toInt();

	// �γ�����
    mName = json["name"].toString();

	// �γ�״̬
	mStatus = json["status"].toString();
    
	// �Ͽ�ʱ��
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

QString Lesson::LessonID()
{
	return QString::number(mId);
}

QString Lesson::ChinaLessonStatus()
{
	QString strChinaStatus;
	if (mStatus == "init")
	{
		strChinaStatus = MSG_LESSON_STATUS_INIT;
	}
	else if (mStatus == "ready")
	{
		strChinaStatus = MSG_LESSON_STATUS_READY;
	}
	else if (mStatus == "teaching")
	{
		strChinaStatus = MSG_LESSON_STATUS_THEACHING;
	}
	else if (mStatus == "paused")
	{
		strChinaStatus = MSG_LESSON_STATUS_PAUSED;
	}
	else if (mStatus == "closed")
	{
		strChinaStatus = MSG_LESSON_STATUS_CLOSED;
	}
	else if (mStatus == "finished")
	{
		strChinaStatus = MSG_LESSON_STATUS_CLOSED;
	}
	else if (mStatus == "billing")
	{
		strChinaStatus = MSG_LESSON_STATUS_CLOSED;
	}
	else if (mStatus == "completed")
	{
		strChinaStatus = MSG_LESSON_STATUS_CLOSED;
	}

	return strChinaStatus;
}