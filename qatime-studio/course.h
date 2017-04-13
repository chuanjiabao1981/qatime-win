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
    QString		mName;				// ����������
    QString		mStatus;
	QString		mProgress;			// ����
	QJsonArray	mJsonArray;			// �γ�json
	QString		mURL;				// �γ�������URL
	QString		mCameraURL;			// ����ͷ������ַ		
	QString		mChatId;			// ���ŻỰID
	QString     mGrade;				// ��Ŀ
	QString     mTeacherName;		// ��ʦ����
	QString		mPic;				// ������ͼƬ��ַ
};

#endif // COURSE_H
