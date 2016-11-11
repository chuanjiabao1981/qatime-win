/** @file define.h
  * @brief postmessage传递消息的ID
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Xds
  * @date 2016/9/19
  */
//课程状态
#define MSG_LESSON_STATUS_INIT			"未开始"
#define MSG_LESSON_STATUS_READY			"待上课"
#define MSG_LESSON_STATUS_THEACHING		"直播中"
#define MSG_LESSON_STATUS_PAUSED		"暂停中"
#define MSG_LESSON_STATUS_CLOSED		"已直播"
#define MSG_LESSON_STATUS_FINISHED		"已结束"
#define MSG_LESSON_STATUS_BILLING		"已结束"
#define MSG_LESSON_STATUS_COMPLETED		"已结束"


#define MSG_START_COUNT					5000
#define MSG_CLIENT_RECEIVE				MSG_START_COUNT			//接收消息ID
#define MSG_CLIENT_RECORD				MSG_START_COUNT	+ 1		//接收历史消息
#define MSG_LOGIN						MSG_START_COUNT	+ 2		//登录消息
#define MSG_MEMBERS_INFO				MSG_START_COUNT	+ 3		//成员信息
#define MSG_VIDEO_CHANGE_SIZE			MSG_START_COUNT	+ 4		//改变窗口大小

// 和qatime_video交互用到的消息
struct VideoInfo
{
	std::string url; //推流地址
};

#define MSG_VIDEO_CUSTOM				4000		
#define MSG_VIDEO_HWND					MSG_VIDEO_CUSTOM + 1	//video窗口句柄
#define MSG_VIDEO_START_LIVE			MSG_VIDEO_CUSTOM + 2	//开始推流
#define MSG_VIDEO_STOP_LIVE				MSG_VIDEO_CUSTOM + 3	//结束推流
#define MSG_VIDEO_CAMERA				MSG_VIDEO_CUSTOM + 4	//摄像头
#define MSG_VIDEO_FULLSCREEN			MSG_VIDEO_CUSTOM + 5	//全屏桌面
#define MSG_VIDEO_OTHREAPP				MSG_VIDEO_CUSTOM + 6	//其他应用
#define MSG_VIDEO_SELECTAPP				MSG_VIDEO_CUSTOM + 7	//其他应用选取消息
#define MSG_DEVICE_VIDEO				MSG_VIDEO_CUSTOM + 8	//获取摄像头信息
#define MSG_DEVICE_AUDIO				MSG_VIDEO_CUSTOM + 9	//获取麦克风信息
#define MSG_DEVICE_AUDIO_CLOSE			MSG_VIDEO_CUSTOM + 10	//关闭或开启麦克风信息
#define MSG_WHITEBOARD					MSG_VIDEO_CUSTOM + 11	//白板
#define MSG_VIDEO_SUSPEND				MSG_VIDEO_CUSTOM + 12   //直播暂停
#define MSG_VIDEO_CONTINUE				MSG_VIDEO_CUSTOM + 13   //直播继续

// 直播摄像头
#define MSG_CAMERA_CUSTOM				4200	
#define MSG_CAMERA_HWND					MSG_CAMERA_CUSTOM + 1	//camera窗口句柄