#include "Member.h"

YXMember::YXMember()
{
	mAccid  = "";
	mName   = "";
	mIconUrl = "";
}

void YXMember::readJsonToMember(const QJsonObject &json)
{
	mAccid = json["accid"].toString();
    mName = json["name"].toString();
	mIconUrl = json["icon"].toString();
}

QString YXMember::url()
{
	return mIconUrl;
}

QString YXMember::accid()
{
	return mAccid;
}

QString YXMember::name() 
{
    return mName;
}

void YXMember::readJsonToAnnouncement(const QJsonObject &json)
{
	mAnnouncement = json["announcement"].toString();
	mTime = json["edit_at"].toString();
}

QString YXMember::announcement()
{
	return mAnnouncement;
}

QString YXMember::announcementTime()
{
	return mTime;
}
