LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := jin_part
LOCAL_SRC_FILES := jin_part.cpp
#����cartoon��cppĿ¼
LOCAL_SRC_FILES += ../../Cartoonifier_Desktop/cartoon.cpp
LOCAL_SRC_FILES += ../../Cartoonifier_Desktop/ImageUtils_0.7.cpp
#����ͷ�ļ�����·��
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Cartoonifier_Desktop

include $(BUILD_SHARED_LIBRARY)