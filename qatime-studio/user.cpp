#include "user.h"
#include <QJsonObject>

User::User()
{

}

User::User(int id, const QString &name, User::UserType userType) :
    mId(id),
    mName(name),
    mUserType(userType)
{
}

int User::id()
{
    return mId;
}

void User::setId(int id)
{
    mId = id;
}

QString User::name() const
{
    return mName;
}

void User::setName(const QString &name)
{
    mName = name;
}

User::UserType User::userType() const
{
    return mUserType;
}

void User::setUserType(UserType userType)
{
    mUserType = userType;
}

void User::readJson(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
}
