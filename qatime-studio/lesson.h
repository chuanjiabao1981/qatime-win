#ifndef LESSON_H
#define LESSON_H

#include <QJsonObject>

#pragma execution_character_set("utf-8")

class Lesson
{
public:
    Lesson();
    void readJson(const QJsonObject &json);
    int courseId();
    void setCourseId(int courseId);
    QString name();
	QString LessonTime();
	QString LessonStatus();
	QString ChinaLessonStatus();
	QString LessonID();
	QString CourseID();
	QString CourseName();
	QString Date();

	QString BoardUrl();
	QString CameraUrl();
private:
    int mId;
    int mCourseId;
    QString mName;			// 课程名称
	QString mStatus;		// 课程状态
	QString mStartTime;		// 上课时间
	QString mCourseID;		// 辅导班ID
	QString mCourseName;	// 辅导班名字
	QString	mDate;			// 日期 （2017-2-28）

	QString	mBoardUrl;		// 白板URL
	QString mCameraUrl;		// 摄像头URL
};

#endif // LESSON_H
