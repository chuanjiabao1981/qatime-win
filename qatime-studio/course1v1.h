#ifndef Course1v1_H
#define Course1v1_H

#include <QJsonObject>
#include <QJsonArray>

class Course1v1
{
public:
    Course1v1();

    QString		id();
    QString		name() const;
	QString		progress() const;
	QString		status();
    void		readJson(const QJsonObject &json);
	QJsonArray  JsonLesson();
	QString		url();
	QString		CameraURL();
	QString		ChatId();
	QString		Grade();
	QString		TeacherName();
	QString		PicUrl();
private:
    int			mId;
    QString		mName;				// 辅导班名称
    QString		mStatus;
	QString		mProgress;			// 进度
	QJsonArray	mJsonArray;			// 课程json
	QString		mURL;				// 课程推流的URL
	QString		mCameraURL;			// 摄像头推流地址		
	QString		mChatId;			// 云信会话ID
	QString     mGrade;				// 科目
	QString     mTeacherName;		// 老师名字
	QString		mPic;				// 辅导班图片地址
};

#endif // Course1v1_H
