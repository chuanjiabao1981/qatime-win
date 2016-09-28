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

