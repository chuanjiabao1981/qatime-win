#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QObject>
#include <QMap>

#include "nim_cpp_rts.h"
#include "nim_cpp_vchat.h"

/****************************************************************白板会话相关回调***********************************************************************************/
void CallbackStartChannel(nim::NIMResCode res_code, const std::string& session_id, int channel_type, const std::string& uid);
void CallbackStartNotify(const std::string& session_id, int channel_type, const std::string& uid, const std::string& custom_info);
void CallbackCreateConf(nim::NIMResCode res_code);
void CallbackJoinConf(nim::NIMResCode res_code, const std::string& session_id, __int64 channel_id, const std::string& custom_info);
void CallbackAck(nim::NIMResCode res_code, const std::string& session_id, int channel_type, bool accept);
void CallbackAckNotify(const std::string& session_id, int channel_type, bool accept, const std::string& uid);
void CallbackSyncAckNotify(const std::string& session_id, int channel_type, bool accept);
void CallbackConnectNotify(const std::string& session_id, int channel_type, int code, const std::string& json);
void CallbackMemberNotify(const std::string& session_id, int channel_type, const std::string& uid, int code);
void CallbackHangup(nim::NIMResCode res_code, const std::string& session_id);
void CallbackHangupNotify(const std::string& session_id, const std::string& uid);
void CallbackControl(nim::NIMResCode res_code, const std::string& session_id, const std::string& info);
void CallbackControlNotify(const std::string& session_id, const std::string& info, const std::string& uid);
void CallbackRecData(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data);

/****************************************************************音视频相关回调***********************************************************************************/
void CallbackNetDetect(int code, nim::NetDetectCbInfo info);								/**< 网络探测回调模板 */
void CallbackMp4Opt(bool ret, int code, const std::string& file, __int64 time);	   			/**< MP4录制事件通知回调模板 */
void CallbackAudioRecord(bool ret, int code, const std::string& file, __int64 time);		/**< 音频录制事件通知回调模板 */
void CallbackOptCall(bool ret, int code, const std::string& json_extension);				/**< 操作回调模板 */
void CallbackOpt2Call( int code, __int64 channel_id, const std::string& json_extension);	    /**< 音视频加入模板 */
void CallbackOpt2CreateCall(int code, __int64 channel_id, const std::string& json_extension);	    /**< 音视频创建 */

//采集的视频数据
void CallbackVideoCaptureData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data);
//接收的视频数据
void CallbackVideoRecData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data);
//采集的音频数据
void CallbackAudioCaptureData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data);
//接收的音频数据
void CallbackAudioRecData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data);
//音视频通话中状态返回的回调
void CallbackVChatCb(nim::NIMVideoChatSessionType type, __int64 channel_id, int code, const char *json, const void*);

/****************************************************************音视频设备回调***********************************************************************************/
void CallbackDeviceDevpath(bool ret, nim::NIMDeviceType type, const char *json_extension, const void *user_data);//遍历设备回调
void CallbackStartDevice(nim::NIMDeviceType type, bool ret, const char *json_extension, const void *user_data);//启动设备异步回调

typedef struct DeviceInfo
{
	int type;
	QString name;
	QString path;
}DevInfo;

typedef QMap< QString, DevInfo > DeviceInfoMap;

class IMInterface : public QObject
{
	Q_OBJECT

public:
	~IMInterface();

	static IMInterface *getInstance();
	static void destroy();

	/****************************************************************白板相关***********************************************************************************/
	void initWhiteBoard();								//初始化白板
	void destroyRts();									//清理白板

	void initRtsCallback();								//初始化白板相关回调

	//void createRtsRoom(int channel_type, const QString& uid, const QString& apns, const QString& custom_info, bool data_record, bool audio_record);  //创建一对一白板直播房间
	void createRtsRoom(const std::string &name, const std::string &custom_info = "");			//创建多人白板直播房间
	void joinRtsRoom(const std::string &name, bool record = false);									//加入多人白板直播房间  record 服务器是否记录白板数据
	void SendData(const std::string& session_id, int channel_type, const std::string& data, const std::string& uid = "");		//发送白板数据

	void setSessionID(const std::string &sessionID);	//设置会话ID
	const std::string &getSessionID();					//获取会话ID

	void setRtsRoomName(const std::string &roomName);	//设置白板会话房间名
	const std::string &getRtsRoomName();				//获取白板会话房间名

	void setRtsData(const std::string & data);			//设置接收到的白板数据
	const std::string &getData();						//获取接收到的白板数据

	void setLastError(const QString &error);					//设置最后一次错误信息
	const QString &getLastError();								//获取最后一次错误信息

	/****************************************************************音视频相关***********************************************************************************/
	void initVChat();									//初始化音视频
	void destroyVChat();								//清理音视频

	void initVChatCallback();							//初始化音视频相关回调

	void createVChatRoom(const std::string &name, const std::string &custom_info = "");								//创建多人音视频房间
	//json_extension 可选 如{"custom_video":0, "custom_audio":0, "video_quality":0, "session_id":"1231sda"}
	void joinVChatRoom(int chatMode, const std::string &name, const std::string &json_extension = "");				//加入多人音视频房间  chatMode 通话模式：1为语音，2为视频

	/****************************************************************音视频设备相关***********************************************************************************/
	void EnumDeviceDevpath(int deviceType);				//遍历设备	deviceType  设备类型   0 麦克风 2 扬声器 3 摄像头
	void startDevice(int type, const std::string& device_path, unsigned fps, int width, int height);		//启动设备
	void endDevice(int type);																				//关闭设备

	void addDeviceInfo(const DevInfo &dInfo);					//添加一个设备信息
	void addDeviceInfo(int type, const char *json_string);		//根据json字符串解析设备信息并添加至设备列表
	const DeviceInfoMap &getDeviceInfos();						//获取设备列表
	void setAudioChange(int volumn, bool captrue);				//设置声音大小， capture true 标识设置麦克风音量，false 标识设置播放音量
	void SetCustomData(bool bVideo);							//设置发送数据模式
	void CustomVideoData(__int64 time, const char* data, int size, int width, int height);	//自定义数据发送

signals:
	/****************************************************************白板相关***********************************************************************************/
	void createRtsRoomSuccessfully(const std::string&);
	void rtsRoomExistence();
	void joinRtsRoomSuccessfully(const std::string&, __int64, const std::string&);
	void rtsDataReceived(const std::string &);

	/****************************************************************音视频相关***********************************************************************************/
	void createVChatRoomSuccessfully();
	void joinVChatSuccessfully();
	void vChatRoomExistence();
	void joinVChatRoomSuccessfully(const std::string&, __int64, const std::string&);

	/****************************************************************音视频设备相关***********************************************************************************/
	void deviceInfos(int);											//设备已遍历完成   参数为设备类型
	void startDeviceSuccessfully(int);								//设备启动成功     参数为设备类型

	void VideoCapture(const char*, unsigned int, unsigned int, unsigned int);	//video采集的数据
	void RecVideoCapture(const char*, unsigned int, unsigned int, unsigned int);	//接收的video数据
	void hasError(const QString &);

private:
	IMInterface(QObject *parent = 0);

private:
	static IMInterface *mInstace;

	/****************************************************************白板相关***********************************************************************************/
	std::string mRtsSession_id;		//白板会话ID
	std::string mRtsRoomName;		//白板房间名
	std::string mRtsData;			//白板数据

	bool mRtsRoomCreated;	//是否已创建RtsRoom
	bool mRtsRoomJoined;	//是否已加入RtsRoom
	bool mRtsRecord;		//是否记录白板数据

	/****************************************************************音视频相关***********************************************************************************/
	bool mVChatInitSuccess; //音视频是否初始化成功

	std::string mVChatSession_id;	//音视频会话ID
	std::string mVChatRoomName;		//音视频房间名

	QString mLastError;

	DeviceInfoMap mDeviceInfoMap;	//设备类别
};

#endif // WHITEBOARD_H
