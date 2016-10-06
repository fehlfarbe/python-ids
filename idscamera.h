#ifndef IDSCAMERA_H
#define IDSCAMERA_H

#include "ueye.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class IDSCamera
{
public:
    IDSCamera(int id=0);
    ~IDSCamera();

    bool start_capture();
    void stop_capture();

    void *next_image();

    bool set_slave(IDSCamera &slave);
    void set_auto_param();

    double exposure(int val=-1);

    int width();
    int height();
    int depth();
    double fps();
    int shutter_max(int val=-1);

    HIDS get_handle();

private:
    HIDS m_handle;

    // image size
    int m_depth;
    IMAGE_FORMAT_INFO m_formatInfo;

    // camera memory
    char *m_mem_buffer;
    int m_mem_id;
};

#endif // IDSCAMERA_H
