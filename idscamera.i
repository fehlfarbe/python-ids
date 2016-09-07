/************************************

Swig Module for IDS camera Python wrapper

************************************/

// module name
%module(directors="1") idscamera

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
        img = self.next_image()
        if len(img) > 0:
            img = np.frombuffer(img, dtype=np.uint8, count=len(img))
            return img.reshape((self.height(), self.width()))
        else:
            return np.zeros((self.height(), self.width()), dtype=np.uint8)
   }
}
