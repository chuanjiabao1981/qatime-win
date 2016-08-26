#ifndef USER_H
#define USER_H

#include <QJsonObject>

class User
{
public:
    enum UserType {
        Teacher, Student
    };

    User();
    User(int id, const QString &name, UserType userType);

    int id();
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    User::UserType userType() const;
    void setUserType(UserType userType);

    void readJson(const QJsonObject &json);

private:
    int mId;
    QString mName;
    UserType mUserType;
};

#endif // USER_H
