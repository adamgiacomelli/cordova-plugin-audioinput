LOCAL_PATH := $(call my-dir)
OPUS_DIR            := opus-1.1.3

include $(OPUS_DIR)/Android.mk

include $(CLEAR_VARS)
LOCAL_MODULE := opus-jni
LOCAL_SRC_FILES := opus-jni.c

LOCAL_CFLAGS        := -DNULL=0
LOCAL_LDLIBS        := -lm -llog
LOCAL_C_INCLUDES    := $(LOCAL_PATH)/$(OPUS_DIR)/include
LOCAL_SHARED_LIBRARIES := opus
include $(BUILD_SHARED_LIBRARY)
