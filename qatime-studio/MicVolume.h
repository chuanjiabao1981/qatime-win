#ifndef _CMicVolume_H_
#define _CMicVolume_H_

#include <afxcoll.h>

class CMicVolume
{
public: 
    CMicVolume();
    ~CMicVolume();

    int iCurIndex;
    CStringArray strarr;
    CDWordArray intarr;
    bool GetVolume();
    bool SetVolume(int iVol);
    bool GetDeviceList();
    int iMicVol;
    int iMixMaxVol;
    int iMicOldVol;
};
#endif  // _CMicVolume_H_