#ifndef YXMember_H
#define YXMember_H

#include <QJsonObject>
#include <QJsonArray>

class YXMember
{
public:
    YXMember();

	void		readJsonToMember(const QJsonObject &json);
	QString		accid();
    QString		name();
	QString		url();

private:
	QString		mAccid;				// 成员用户ID
    QString		mName;				// 成员名字
	QString		mIconUrl;			// 成员头像url
	
};

#endif // YXMember_H
