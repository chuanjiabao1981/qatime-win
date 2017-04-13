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
// 	if (mStatus == "finished")
//   	mStatus = "ready";
// 	if (mStatus=="completed")
// 		mStatus = "ready";
// 	if (mStatus == "closed")
// 		mStatus = "ready";
    
	// �Ͽ�ʱ��
	mStartTime = json["live_time"].toString();
	mStartTime = mStartTime.mid(0, 5);

	// ������ID
	mCourseID = json["course_id"].toString();

	// ����������
	mCourseName = json["course_name"].toString();

	// ����
	mDate = json["class_date"].toString();

	// �װ�������ַ
	mBoardUrl = json["board_pull_stream"].toString();

	// ����ͷ������ַ
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