/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _NLSS_API_H_
#define _NLSS_API_H_

#include "nlss_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

/*******************************�豸����API����ȡ�ɹ��ɼ�����Ƶ�豸�б�*************************************/
/**
*  ��ȡ���ö�ý���豸�б�����
*
*  @param  iVideoDeviceNum: ��Ƶ�豸���� ����
*  @param  iAudioDeviceNum����Ƶ�豸���� ����
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET  Nlss_GetFreeDevicesNum(NLSS_OUT int *iVideoDeviceNum, NLSS_OUT int *iAudioDeviceNum);
/**
*  ��ȡ���ö�ý���豸�б����ƣ���ʱֻ֧��DShow�ɼ�����Ƶ
*
*  @param  pLSVideoDevices: ��Ƶ�豸��Ϣ ����
*  @param  pLSAudioDevices����Ƶ�豸��Ϣ ����
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET  Nlss_GetFreeDeviceInf(NLSS_OUT ST_NLSS_INDEVICE_INF *pstVideoDevices, NLSS_OUT ST_NLSS_INDEVICE_INF* pstAudioDevices);


/*******************************��ʼ���Ͳ�������API*************************************************************/
/**
*  ����ֱ������ʵ������������ֻ��������һ�������ֱ������ֻ����һ��
*
*  @param  paLogpath:    ��־����·�����������Ϊ�գ����ڵ�ǰ����Ŀ¼���� �����
*  @param  phNLSService: ֱ������ʵ�� ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_Create(const char *paLogpath, NLSS_OUT _HNLSSERVICE *phNLSService);

/**
*  ��ȡsdk�汾��
*
*  @param  ppaVersion: �汾�ţ���Ҫ����free�����ͷţ�����
*
*  @return ��
*/
EXPORTS_API   void     Nlss_GetVersionNo(NLSS_OUT char **ppaVersion);

/**
*  ����ֱ������ʵ�������Գ����˳�������
*
*  @param  hNLSService: ֱ������ʵ�������
*/
EXPORTS_API  void       Nlss_Destroy(_HNLSSERVICE hNLSService);

/**
*  ��ȡֱ��Ĭ�ϲ���
*  @param  hNLSService: ֱ������ʵ�������
*  @param  enVideoInType�� ������Ƶ�����ͣ����
*  @param  enAudioInType�� ������Ƶ�����ͣ����
*  @param  pstParam: ֱ������������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_GetDefaultParam(_HNLSSERVICE hNLSService, NLSS_OUT ST_NLSS_PARAM *pstParam);
/**
*  ��ʼ��ֱ������
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstParam: ֱ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_InitParam(_HNLSSERVICE hNLSService, ST_NLSS_PARAM *pstParam);

/**
*  ������Ƶ������ʾ����������Ϊ��������д16,9.Ĭ������±���ԭ���ߴ�
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  iWideUnit iHeightUnit: �ֱ�Ϊ���߱�����λ������Ϊ��������д16,9.
*/
EXPORTS_API   void     Nlss_SetVideoDisplayRatio(_HNLSSERVICE hNLSService, int iWideUnit, int iHeightUnit);

/**
*  ������Ƶˮӡ��Ĭ������ˮӡ
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstWaterParam: ˮӡ�����ṹ��.
*/
EXPORTS_API   void     Nlss_SetVideoWaterMark(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_WATER_PARAM *pstWaterParam);

/**
*  ������Ƶ��ͼ�ĵĻص�
*
*  @param  pFunVideoSamplerCB ��Ƶ��ͼͼ��ص�
*/
EXPORTS_API   void     Nlss_SetVideoSamplerCB(_HNLSSERVICE hNLSService, PFN_NLSS_VIDEOSAMPLER_CB pFunVideoSamplerCB);

/**
*  ����ֱ��������״̬�ص�
*
*  @param  pFunStatusNty ֱ��״̬֪ͨ����
*/
EXPORTS_API   void     Nlss_SetStatusCB(_HNLSSERVICE hNLSService, PFN_NLSS_STATUS_NTY pFunStatusNty);

/**
*  ���Nlss_InitParam���õ�ֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstParam: ֱ������
*
*  @return ��
*/
EXPORTS_API   void    Nlss_UninitParam(_HNLSSERVICE hNLSService);

/*******************************��ƵԤ��API*************************************************************/
/**
*  ����Ƶ�ɼ�����Ҫ����ƵԤ��ǰ���ã�����Ҫ�ı�ɼ��豸ʱ��ʱ����Ҫ��ֹͣ�����´�
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_StartVideoCapture(_HNLSSERVICE hNLSService);

/**
*  �ر���Ƶ�ɼ�
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API   void     Nlss_StopVideoCapture(_HNLSSERVICE hNLSService);

/**
*  ����ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_StartVideoPreview(_HNLSSERVICE hNLSService);

/**
*  ��ͣ��ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void      Nlss_PauseVideoPreview(_HNLSSERVICE hNLSService);

/**
*  �ָ���ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void      Nlss_ResumeVideoPreview(_HNLSSERVICE hNLSService);

/**
*  ֹͣ��ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_StopVideoPreview(_HNLSSERVICE hNLSService);

/*******************************ֱ������API*************************************************************/

/**
*  ����ֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_StartLiveStream(_HNLSSERVICE hNLSService);

/**
*  ֱֹͣ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_StopLiveStream(_HNLSSERVICE hNLSService);

/**
*  ��ͣ��Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_PauseVideoLiveStream(_HNLSSERVICE hNLSService);
/**
*  �ָ���Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ResumeVideoLiveStream(_HNLSSERVICE hNLSService);

/**
*  ��ͣ��Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_PauseAudioLiveStream(_HNLSSERVICE hNLSService);

/**
*  �ָ���Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ResumeAudioLiveStream(_HNLSSERVICE hNLSService);

/*******************************ֱ��������ʽ2 API: ����ֱ���ӿ�*************************************************************/
/**
*  �û�ָ����Ƶ���������ͽӿ�
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pcVideoData: �û�ָ����Ƶ�����ݣ����
*  @param  iLen:        ��Ƶ�����ݳ��ȣ����
*  @param  penErrCode:  ��Ƶ�����ʹ����룬������ֵΪNLSS_ERR����ֵ��Ч ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API NLSS_RET    Nlss_SendCustomVideoData(_HNLSSERVICE hNLSService, char *pcVideoData, int iLen, NLSS_OUT EN_NLSS_ERRCODE *penErrCode);

/**
*  �û�ָ����Ƶ���������ͽӿڣ�֧��ֱ����������Ƶ�����ز���
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pcAudioData: �û�ָ����Ƶ�����ݣ����
*  @param  iLen:        ��Ƶ�����ݳ��ȣ����
*  @param  iSampleRate: ��Ƶ�������ʣ�����֧���м�仯���
*  @param  penErrCode:  ��Ƶ�����ʹ����룬������ֵΪNLSS_ERR����ֵ��Ч ������
*
*  @return NLSS_RET     NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_SendCustomAudioData(_HNLSSERVICE hNLSService, char *pcAudioData, int iLen, int iSampleRate, NLSS_OUT EN_NLSS_ERRCODE *penErrCode);

/**
*  ��ȡֱ������״̬��Ϣ
*
*  @param  hNLSService:   ֱ������ʵ�������
*  @param  pstatistics:  ֱ������ͳ����Ϣ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_GetStaticInfo(_HNLSSERVICE hNLSService, NLSS_OUT ST_NLSS_STATS *pstStats);

/**
*  ��ȡ��ץȡwindowsӦ����Ŀ
*
*  @param  iAppWindNum: Ӧ������ ����
*/
EXPORTS_API void GetAvailableAppWindNum(int *iAppWindNum);
/**
*  ��ȡ��ץȡwindowsӦ����Ϣ
*
*  @param  pLSAppWindTitles: Ӧ����Ϣ
*/
EXPORTS_API void GetAvailableAppWind(ST_NLSS_INDEVICE_INF *pLSAppWindTitles);

#ifdef __cplusplus
}
#endif

#endif// _LS_MEDIACAPTURE_API_H_


