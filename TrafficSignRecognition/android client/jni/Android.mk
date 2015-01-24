LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include C:/OpenCV-2.4.9-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := sign_detector
LOCAL_SRC_FILES := sign_detector.cpp
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)