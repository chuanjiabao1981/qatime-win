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
