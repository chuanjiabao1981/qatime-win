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
#define MSG_LESSON_STATUS_MISSED		"待补课"

#define TIP_GET_RTMP_ERROR				"加入直播间异常，请稍后再试！"
#define TIP_START_LIVE_ERROR			"服务器异常，请稍后再试！"


#define MSG_START_COUNT					5000
#define MSG_CLIENT_RECEIVE				MSG_START_COUNT			//接收消息ID
#define MSG_CLIENT_RECORD				MSG_START_COUNT	+ 1		//接收历史消息
#define MSG_LOGIN						MSG_START_COUNT	+ 2		//登录消息
#define MSG_MEMBERS_INFO				MSG_START_COUNT	+ 3		//成员信息
#define MSG_VIDEO_CHANGE_SIZE			MSG_START_COUNT	+ 4		//改变窗口大小
#define MSG_MEMBER_INFO					MSG_START_COUNT	+ 5		//单个成员信息
#define MSG_PICTURE_PROCESS				MSG_START_COUNT	+ 6		//图片发送进度
#define MSG_SEND_MSG_STATUS				MSG_START_COUNT	+ 7		//发送消息状态
#define MSG_SEND_AUDIO_MSG				MSG_START_COUNT	+ 8		//发送语音消息停止
#define MSG_SEND_AUDIO_FINISH_MSG		MSG_START_COUNT	+ 9		//发送语音录制完成消息
#define MSG_LOAD_AUDIO_SUC_MSG			MSG_START_COUNT	+ 10	//发送下载成功消息
#define MSG_LOAD_AUDIO_FAIL_MSG			MSG_START_COUNT	+ 11	//发送下载失败消息
#define MSG_UPDATE_MSG					MSG_START_COUNT	+ 12	//发送更新消息

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
#define MSG_DEVICE_BACK					MSG_VIDEO_CUSTOM + 14	//推黑色背景流
#define MSG_VIDEO_LIVE_SUC				MSG_VIDEO_CUSTOM + 15   //直播开始成功
#define MSG_VIDEO_PATH					MSG_VIDEO_CUSTOM + 16	//接收播放路径

// 直播摄像头
#define MSG_CAMERA_CUSTOM				4200	
#define MSG_CAMERA_HWND					MSG_CAMERA_CUSTOM + 1	//camera窗口句柄