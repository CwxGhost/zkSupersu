LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := su_client
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_SRC_FILES := client.c
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_MODULE := su_server
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_SRC_FILES := server.c
include $(BUILD_EXECUTABLE)
