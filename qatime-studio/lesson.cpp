#include "lesson.h"

Lesson::Lesson()
{

}

void Lesson::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
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

QString Lesson::ChinaLessonStatus()
{
	QString strChinaStatus;
	if (mStatus == "init")
	{
		strChinaStatus = "δ��ʼ";
	}
	else if (mStatus == "ready")
	{
		strChinaStatus = "������";
	}
	else if (mStatus == "teaching")
	{
		strChinaStatus = "������";
	}
	else if (mStatus == "paused")
	{
		strChinaStatus = "������";
	}
	else if (mStatus == "closed")
	{
		strChinaStatus = "�ѽ���";
	}
	else if (mStatus == "finished")
	{
		strChinaStatus = "�ѽ���";
	}
	else if (mStatus == "billing")
	{
		strChinaStatus = "������";
	}
	else if (mStatus == "completed")
	{
		strChinaStatus = "�ѽ���";
	}

	return strChinaStatus;
}