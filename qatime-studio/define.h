/** @file define.h
  * @brief postmessage传递消息的ID
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author Xds
  * @date 2016/9/19
  */
#define MSG_START_COUNT					5000
#define MSG_CLIENT_RECEIVE				MSG_START_COUNT			//接收消息ID
#define MSG_CLIENT_RECORD				MSG_START_COUNT	+ 1		//接收历史消息
#define MSG_LOGIN						MSG_START_COUNT	+ 2		//登录消息
#define MSG_MEMBERS_INFO				MSG_START_COUNT	+ 3		//成员信息


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

