#include "lesson.h"
#include "define.h"

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
// 	if (mStatus == "finished")
//   	mStatus = "ready";
// 	if (mStatus=="completed")
// 		mStatus = "ready";
// 	if (mStatus == "closed")
// 		mStatus = "ready";
    
	// 上课时间
	mStartTime = json["live_time"].toString();
	mStartTime = mStartTime.mid(0, 5);

	// 辅导班ID
	mCourseID = json["course_id"].toString();

	// 辅导班名字
	mCourseName = json["course_name"].toString();

	// 日期
	mDate = json["class_date"].toString();

	// 白板推流地址
	mBoardUrl = json["board_pull_stream"].toString();

	// 摄像头推流地址
	mCameraUrl = json["camera_pull_stream"].toString();
}

QString Lesson::BoardUrl()
{
	return mBoardUrl;
}

QString Lesson::CameraUrl()
{
	return mCameraUrl;
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
		strChinaStatus = MSG_LESSON_STATUS_FINISHED;
	}
	else if (mStatus == "billing")
	{
		strChinaStatus = MSG_LESSON_STATUS_BILLING;
	}
	else if (mStatus == "completed")
	{
		strChinaStatus = MSG_LESSON_STATUS_COMPLETED;
	}
	else if (mStatus == "missed")
	{
		strChinaStatus = MSG_LESSON_STATUS_MISSED;
	}

	return strChinaStatus;
}

QString Lesson::Date()
{
	return mDate;
}

QString Lesson::CourseName()
{
	return mCourseName;
}

QString Lesson::CourseID()
{
	return mCourseID;
}