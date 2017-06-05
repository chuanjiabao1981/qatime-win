#include "Course1v1.h"

Course1v1::Course1v1()
{

}


void Course1v1::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
    mStatus = json["status"].toString();
	mChatId = json["chat_team_id"].toString();

	mGrade = json["grade"].toString();
	mTeacherName = json["teacher_name"].toString();
	 mPic = json["publicize_info_url"].toString();
	// ½ø¶È
	int preset_lesson = json["lessons_count"].toInt();
	int completed_lesson = json["completed_lesson_count"].toInt();
	mProgress = QString::number(completed_lesson);
	mProgress += "/";
	mProgress += QString::number(preset_lesson);
}

QString Course1v1::url()
{
	return mURL;
}

QString Course1v1::CameraURL()
{
	return mCameraURL;
}

QString Course1v1::id()
{
    return QString::number(mId);
}

QString Course1v1::name() const
{
    return mName;
}

QString Course1v1::progress() const
{
	return mProgress;
}

QString Course1v1::status()
{
    return mStatus;
}

QJsonArray Course1v1::JsonLesson()
{
	return mJsonArray;
}

QString Course1v1::ChatId()
{
	return mChatId;
}

QString Course1v1::Grade()
{
	return mGrade;
}

QString Course1v1::TeacherName()
{
	return mTeacherName;
}

QString Course1v1::PicUrl()
{
	return mPic;
}