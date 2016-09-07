#include "idscamera.h"

/**
 * @brief IDSCamera::IDSCamera
 * Opens IDS camera, gets image format and allocates image buffer
 *
 * @param id camera id, 0 for first detected camera
 */
IDSCamera::IDSCamera(int id)
{
    m_handle = id;
    INT ret = is_InitCamera(&this->m_handle, NULL);

    if(ret == IS_SUCCESS){
        // Get number of available formats and size of list
        UINT count;
        UINT bytesNeeded = sizeof(IMAGE_FORMAT_LIST);
        ret = is_ImageFormat(m_handle, IMGFRMT_CMD_GET_NUM_ENTRIES, &count, 4);
        bytesNeeded += (count - 1) * sizeof(IMAGE_FORMAT_INFO);
        void* ptr = malloc(bytesNeeded);

        // Create and fill list
        IMAGE_FORMAT_LIST* pformatList = (IMAGE_FORMAT_LIST*) ptr;
        pformatList->nSizeOfListEntry = sizeof(IMAGE_FORMAT_INFO);
        pformatList->nNumListElements = count;
        ret = is_ImageFormat(m_handle, IMGFRMT_CMD_GET_LIST, pformatList, bytesNeeded);

        // Set each format and then capture an image
        IMAGE_FORMAT_INFO formatInfo;
        m_formatInfo = pformatList->FormatInfo[0];
        is_ImageFormat(m_handle, IMGFRMT_CMD_SET_FORMAT, &m_formatInfo.nFormatID, 4);

        // color depth in bit hard coded
        m_depth = 8;

        is_AllocImageMem(m_handle, width(), height(), depth(), &m_mem_buffer, &m_mem_id);
        is_SetImageMem (m_handle, m_mem_buffer, m_mem_id);
        is_SetColorMode(m_handle, IS_CM_MONO8);
    }
}

IDSCamera::~IDSCamera()
{
    is_ExitCamera(m_handle);
}

/**
 * @brief IDSCamera::start_capture
 * Starts live video capture
 * @return
 */
bool IDSCamera::start_capture()
{
    return is_CaptureVideo(m_handle, IS_DONT_WAIT);
}

/**
 * @brief IDSCamera::stop_capture
 * Stops live video capture
 */
void IDSCamera::stop_capture()
{
    is_StopLiveVideo(m_handle, IS_WAIT);
}

/**
 * @brief IDSCamera::next_image
 * Get the next image from video stream (start capture before calling this function)
 * @return image buffer
 */
char *IDSCamera::next_image()
{
    is_FreezeVideo(m_handle, IS_DONT_WAIT);
    return m_mem_buffer;
}

/**
 * @brief IDSCamera::set_slave
 * @param slave salve IDS camera
 * Acivates the flash IO pins of your master camera and sets the slave camera
 * to external trigger mode to synchronize both cameras.
 * You have to connect the flash pins of your master to trigger pins of you slave
 * camera. 5v (master) -> flash + (master), flash - (master) -> trigger + (slave),
 * trigger - (slave) -> ground
 * @return
 */
bool IDSCamera::set_slave(IDSCamera& slave)
{
    // trigger stuff
    is_SetExternalTrigger(m_handle, IS_SET_TRIGGER_SOFTWARE);
    is_SetExternalTrigger(slave.get_handle(), IS_SET_TRIGGER_HI_LO);

    // Set the flash to a high active pulse for each image in the trigger mode
    UINT nMode = IO_FLASH_MODE_TRIGGER_HI_ACTIVE;
    INT nRet = is_IO(m_handle, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
    if(nRet != IS_SUCCESS){
        cout << "Can't set flash/trigger mode: " << nRet << endl;
    }

    IO_FLASH_PARAMS flashParams;
    // Get the minimum values for the GPIO flash delay and flash duration
    nRet = is_IO(m_handle, IS_IO_CMD_FLASH_GET_GPIO_PARAMS_MIN, (void*)&flashParams,
                sizeof(flashParams));
    // Set the minimum values for flash delay and flash duration. Be careful: The normal flash does not work with values < 20 us
    nRet = is_IO(m_handle, IS_IO_CMD_FLASH_SET_GPIO_PARAMS, (void*)&flashParams,
                 sizeof(flashParams));

    return true;
}

/**
 * @brief IDSCamera::set_auto_param
 * Sets gain, shutter and framerate to auto mode
 */
void IDSCamera::set_auto_param()
{
    double on=1.0, off=0.0;
    is_SetAutoParameter(m_handle, IS_SET_ENABLE_AUTO_GAIN, &on, &off);
    is_SetAutoParameter(m_handle, IS_SET_ENABLE_AUTO_SHUTTER, &on, &off);
    is_SetAutoParameter(m_handle, IS_SET_ENABLE_AUTO_FRAMERATE, &on, &off);
}

/**
 * @brief IDSCamera::exposure
 * Set static exposure time
 * @param val exposure time in ms
 * @return
 */
double IDSCamera::exposure(int val)
{
    double v=(double)val;
    if( val > 0){
        is_Exposure(m_handle, IS_EXPOSURE_CMD_SET_EXPOSURE, &v, sizeof(v));
    }

    is_Exposure(m_handle, IS_EXPOSURE_CMD_GET_EXPOSURE, &v, sizeof(v));
    return v;
}

/**
 * @brief IDSCamera::width
 * @return image width
 */
int IDSCamera::width()
{
    return m_formatInfo.nWidth;
}

/**
 * @brief IDSCamera::height
 * @return image height
 */
int IDSCamera::height()
{
    return m_formatInfo.nHeight;
}

/**
 * @brief IDSCamera::depth
 * image depth in bits @todo: is set to 8 bit
 * @return
 */
int IDSCamera::depth()
{
    //is_GetColorDepth(HIDS hCam, INT* pnCol, INT* pnColMode)
    return m_depth;
}

/**
 * @brief IDSCamera::fps
 * @return current fps calculated by camera
 */
double IDSCamera::fps()
{
    double fps = 0.0;
    is_GetFramesPerSecond (m_handle, &fps);
    return fps;
}

/**
 * @brief IDSCamera::shutter_max
 * @param val maximum shutter time in ms
 * @return
 */
int IDSCamera::shutter_max(int val)
{
    double pval1, pval2;
    INT ret = is_SetAutoParameter (m_handle, IS_GET_AUTO_SHUTTER_MAX, &pval1, &pval2);

    if(val < 0){
        return (int)pval1;
    }

    pval1 = val;
    is_SetAutoParameter (m_handle, IS_SET_AUTO_SHUTTER_MAX, &pval1, &pval2);

    return pval1;
}

/**
 * @brief IDSCamera::get_handle
 * @return camera handle
 */
HIDS IDSCamera::get_handle()
{
    return m_handle;
}
