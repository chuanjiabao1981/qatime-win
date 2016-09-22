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

	void		readJsonToAnnouncement(const QJsonObject &json);
	QString		announcement();
	QString		announcementTime();

private:
	QString		mAccid;				// 成员用户ID
    QString		mName;				// 成员名字
	QString		mIconUrl;			// 成员头像url
	
	QString		mAnnouncement;		// 公告内容
	QString		mTime;				// 公告时间
};

#endif // YXMember_H
