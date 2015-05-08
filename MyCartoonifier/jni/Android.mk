LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE:=STATIC
OPENCV_INSTALL_MODULES:=on

include D:/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk 



LOCAL_MODULE    := jin_part
LOCAL_SRC_FILES := jin_part.cpp
#添加cartoon。cpp目录
LOCAL_SRC_FILES += ../../MyCartoonifier_Desktop/cartoons.cpp
#LOCAL_SRC_FILES += ../../MyCartoonifier_Desktop/ImageUtils_0.7.cpp
#添加头文件搜索路径
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MyCartoonifier_Desktop


include $(BUILD_SHARED_LIBRARY)
