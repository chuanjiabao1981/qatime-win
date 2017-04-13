#include "course.h"

Course::Course()
{

}


void Course::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
    mStatus = json["status"].toString();
	mChatId = json["chat_team_id"].toString();

	QString subject = json["subject"].toString();
	mGrade = json["grade"].toString();
	mGrade += subject;

	mTeacherName = json["teacher_name"].toString();
	mPic = json["publicize"].toString();
	// 进度
	int preset_lesson = json["preset_lesson_count"].toInt();
	int completed_lesson = json["completed_lesson_count"].toInt();
	mProgress = QString::number(completed_lesson);
	mProgress += "/";
	mProgress += QString::number(preset_lesson);

	// 课程
	mJsonArray = json["lessons"].toArray();

	// 白板推流地址
	mURL = json["board"].toString();

	// 摄像头推流地址
	mCameraURL = json["camera"].toString();
}

QString Course::url()
{
	return mURL;
}

QString Course::CameraURL()
{
	return mCameraURL;
}

QString Course::id()
{
    return QString::number(mId);
}

QString Course::name() const
{
    return mName;
}

QString Course::progress() const
{
	return mProgress;
}

QString Course::status()
{
    return mStatus;
}

QJsonArray Course::JsonLesson()
{
	return mJsonArray;
}

QString Course::ChatId()
{
	return mChatId;
}

QString Course::Grade()
{
	return mGrade;
}

QString Course::TeacherName()
{
	return mTeacherName;
}

QString Course::PicUrl()
{
	return mPic;
}