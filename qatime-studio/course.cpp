#include "course.h"

Course::Course()
{

}


void Course::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
    mStatus = json["status"].toInt();
	mChatId = json["chat_team_id"].toString();

	// 进度
	int preset_lesson = json["preset_lesson_count"].toInt();
	int completed_lesson = json["completed_lesson_count"].toInt();
	mProgress = QString::number(completed_lesson);
	mProgress += "/";
	mProgress += QString::number(preset_lesson);

	// 课程
	mJsonArray = json["lessons"].toArray();

	// 推流地址
	mURL = json["push_address"].toString();
}

QString Course::url()
{
	return mURL;
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

int Course::status()
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