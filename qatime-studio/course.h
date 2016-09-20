#ifndef COURSE_H
#define COURSE_H

#include <QJsonObject>
#include <QJsonArray>

class Course
{
public:
    Course();

    QString		id();
    QString		name() const;
	QString		progress() const;
    int			status();
    void		readJson(const QJsonObject &json);
	QJsonArray  JsonLesson();
	QString		url();
	QString		ChatId();
private:
    int			mId;
    QString		mName;				// 辅导班名称
    int			mStatus;
	QString		mProgress;			// 进度
	QJsonArray	mJsonArray;			// 课程json
	QString		mURL;				// 课程推流的URL
	QString		mChatId;			// 云信会话ID
};

#endif // COURSE_H
