#include "MicVolume.h"
#include <mmsystem.h>
//#include <mmeapi.h>
CMicVolume::CMicVolume()
{
    iCurIndex = 1;
    iMicVol = 0;
    iMixMaxVol = 65535;
}

CMicVolume::~CMicVolume()
{

}

bool CMicVolume::GetDeviceList()
{
    int iMixerCount = 0;
    MIXERCAPS mcDAT;
    HMIXER hmDAT;
        MIXERLINE mlDAT;
    iMixerCount = ::mixerGetNumDevs();
    if (iMixerCount > 0)   
    {
        for (int i = 0; i < iMixerCount; i ++)
        {
            memset(&mcDAT, 0x00, sizeof(MIXERCAPS));
            if (::mixerGetDevCaps(i, &mcDAT, sizeof(MIXERCAPS)) == MMSYSERR_NOERROR)  
            {
                if (mcDAT.cDestinations != 0)
                {
                    if (::mixerOpen(&hmDAT, i, 0, 0, MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)   
                    {
                        memset(&mlDAT, 0x00, sizeof(MIXERLINE));
                        mlDAT.cbStruct = sizeof(MIXERLINE);
                        mlDAT.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

                        if (::mixerGetLineInfo((HMIXEROBJ)hmDAT, 
                                                &mlDAT,
                                                MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_HMIXER ) == MMSYSERR_NOERROR )
                        {
                            strarr.Add(mcDAT.szPname);
                            intarr.Add(i);
                        }
                        mixerClose(hmDAT);
                    }
                }
            }
        }
    }
    return true;
}


bool CMicVolume::GetVolume()
{
    MIXERCAPS mcDAT;
    HMIXER hmDAT;
    MIXERLINE mlDAT;
    MIXERLINECONTROLS mlCTRL;
    MIXERCONTROL mCTRL;
    MIXERCONTROLDETAILS mcdDAT;
    MIXERCONTROLDETAILS_UNSIGNED mcduDAT;
    bool bRet = false;
        
    iMicVol = 0;
    iMixMaxVol = 65535;

    memset(&mcDAT, 0x00, sizeof(MIXERCAPS));
    if (::mixerGetDevCaps(iCurIndex, &mcDAT, sizeof(MIXERCAPS)) == MMSYSERR_NOERROR)  
    {
       if (::mixerOpen(&hmDAT, iCurIndex, 0, 0, MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)    
       {
            memset(&mlDAT, 0x00, sizeof(MIXERLINE));
            mlDAT.cbStruct = sizeof(MIXERLINE);
            mlDAT.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

            if (::mixerGetLineInfo((HMIXEROBJ)hmDAT, &mlDAT,
               MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_HMIXER ) == MMSYSERR_NOERROR ) 
            {
                memset(&mlCTRL, 0x00, sizeof(MIXERLINECONTROLS));
                memset(&mCTRL, 0x00, sizeof(MIXERCONTROL));
                mlCTRL.cbStruct = sizeof(MIXERLINECONTROLS);
                mlCTRL.dwLineID = mlDAT.dwLineID;
                mlCTRL.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
                mlCTRL.cControls = 1 ;
                mlCTRL.pamxctrl = &mCTRL;
                mlCTRL.cbmxctrl = sizeof(MIXERCONTROL);
                if (::mixerGetLineControls((HMIXEROBJ)hmDAT, &mlCTRL,
                   MIXER_GETLINECONTROLSF_ONEBYTYPE | MIXER_OBJECTF_HMIXER) == MMSYSERR_NOERROR)   
                {
                    memset(&mcdDAT, 0x00, sizeof(MIXERCONTROLDETAILS));
                    memset(&mcduDAT, 0x00, sizeof(MIXERCONTROLDETAILS_UNSIGNED));
                    mcdDAT.cbStruct = sizeof(MIXERCONTROLDETAILS);
                    mcdDAT.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
                    mcdDAT.paDetails = &mcduDAT;
                    mcdDAT.dwControlID = mCTRL.dwControlID;
                    mcdDAT.cChannels = (mCTRL.fdwControl | MIXERCONTROL_CONTROLF_UNIFORM ? 1 : mlDAT.cChannels);
                    if ((mCTRL.fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE))   
                    {
                        mcdDAT.cMultipleItems = mCTRL.cMultipleItems;
                    }
                    if (mixerGetControlDetails((HMIXEROBJ)hmDAT, &mcdDAT, MIXER_SETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)    
                    {

                        iMicVol = mcduDAT.dwValue;
                        iMixMaxVol = mCTRL.Bounds.dwMaximum;

                    }
                }
            }
            ::mixerClose(hmDAT);
            bRet = true;
        }
    }
    return bRet;
}

bool CMicVolume::SetVolume(int iVol)
{
   MIXERCAPS mcDAT;
    HMIXER hmDAT;
    MIXERLINE mlDAT;
    MIXERLINECONTROLS mlCTRL;
    MIXERCONTROL mCTRL;
    MIXERCONTROLDETAILS mcdDAT;
    MIXERCONTROLDETAILS_UNSIGNED mcduDAT;
    bool bRet = false;
        
    iMicVol = 0;
    iMixMaxVol = 65535;

    memset(&mcDAT, 0x00, sizeof(MIXERCAPS));
    if (::mixerGetDevCaps(iCurIndex, &mcDAT, sizeof(MIXERCAPS)) == MMSYSERR_NOERROR)  
    {
       if (::mixerOpen(&hmDAT, iCurIndex, 0, 0, MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR)    
       {
            memset(&mlDAT, 0x00, sizeof(MIXERLINE));
            mlDAT.cbStruct = sizeof(MIXERLINE);
            mlDAT.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

            if (::mixerGetLineInfo((HMIXEROBJ)hmDAT, &mlDAT,
               MIXER_GETLINEINFOF_COMPONENTTYPE | MIXER_OBJECTF_HMIXER ) == MMSYSERR_NOERROR ) 
            {
                memset(&mlCTRL, 0x00, sizeof(MIXERLINECONTROLS));
                memset(&mCTRL, 0x00, sizeof(MIXERCONTROL));
                mlCTRL.cbStruct = sizeof(MIXERLINECONTROLS);
                mlCTRL.dwLineID = mlDAT.dwLineID;
                mlCTRL.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
                mlCTRL.cControls = 1 ;
                mlCTRL.pamxctrl = &mCTRL;
                mlCTRL.cbmxctrl = sizeof(MIXERCONTROL);
                if (::mixerGetLineControls((HMIXEROBJ)hmDAT, &mlCTRL,
                   MIXER_GETLINECONTROLSF_ONEBYTYPE | MIXER_OBJECTF_HMIXER) == MMSYSERR_NOERROR)   
                {
                    memset(&mcdDAT, 0x00, sizeof(MIXERCONTROLDETAILS));
                    memset(&mcduDAT, 0x00, sizeof(MIXERCONTROLDETAILS_UNSIGNED));
                    mcdDAT.cbStruct = sizeof(MIXERCONTROLDETAILS);
                    mcdDAT.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
                    mcduDAT.dwValue = iVol;
                    mcdDAT.paDetails = &mcduDAT;
                    mcdDAT.dwControlID = mCTRL.dwControlID;
                    mcdDAT.cChannels = (mCTRL.fdwControl | MIXERCONTROL_CONTROLF_UNIFORM ? 1 : mlDAT.cChannels);
                    if ((mCTRL.fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE))    
                    {
                        mcdDAT.cMultipleItems = mCTRL.cMultipleItems;
                    }
                    if (mixerSetControlDetails((HMIXEROBJ)hmDAT, &mcdDAT, MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER ) != MMSYSERR_NOERROR)
                    {
                        bRet = false;
                    }
                }
            }
            ::mixerClose(hmDAT);
            bRet = true;
        }
    }
    return bRet;
}
