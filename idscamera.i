/************************************

Swig Module for IDS camera Python wrapper

************************************/

// module name
%module(directors="1") idscamera

%include "cdata.i"

%{
	#define SWIG_FILE_WITH_INIT
    #include "idscamera.h"
%}

%inline %{
    #define __LINUX__
%}

%include "idscamera.h"

%extend IDSCamera {
%pythoncode {
    def next(self):
        import numpy as np
        import idscamera
        img = idscamera.cdata(self.next_image(), self.height() * self.width())
        img = np.frombuffer(img, dtype=np.uint8, count=self.height() * self.width())
        return img.reshape((self.height(), self.width()))

   }
}
