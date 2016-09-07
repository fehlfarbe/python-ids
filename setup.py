#!/usr/bin/env python
# -*- coding: utf-8 -*-
#from distutils.core import Extension
from setuptools import setup, find_packages, Extension


sourcefiles = ["idscamera_wrap.cxx", "idscamera.cpp"]

ids_module = Extension('_idscamera',
                       sources=sourcefiles,
                       language="c++",
                       include_dirs=["/usr/include/"],
                       libraries=["ueye_api"],
                       library_dirs=[])

setup(name='idscamera',
      version='0.1',
      packages=find_packages(),
      include_package_data=True,
      install_requires=[
          'numpy',
      ],
      author="""Python wrappers: Marcus Degenkolbe""",
      description="""IDS camera Python wrappers""",
      ext_modules=[ids_module],
      py_modules=["idscamera"],
      )
