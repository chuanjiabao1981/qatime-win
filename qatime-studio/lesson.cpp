#include "lesson.h"

Lesson::Lesson()
{

}

void Lesson::readJson(const QJsonObject &json)
{
	// 课程ID
    mId = json["id"].toInt();

	// 课程名字
    mName = json["name"].toString();

	// 课程状态
	mStatus = json["status"].toString();
    
	// 上课时间
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
		strChinaStatus = "未开始";
	}
	else if (mStatus == "ready")
	{
		strChinaStatus = "待上课";
	}
	else if (mStatus == "teaching")
	{
		strChinaStatus = "进行中";
	}
	else if (mStatus == "paused")
	{
		strChinaStatus = "暂停中";
	}
	else if (mStatus == "closed")
	{
		strChinaStatus = "进行中";
	}
	else if (mStatus == "finished")
	{
		strChinaStatus = "已结束";
	}
	else if (mStatus == "billing")
	{
		strChinaStatus = "已结束";
	}
	else if (mStatus == "completed")
	{
		strChinaStatus = "已结束";
	}


	return strChinaStatus;
}