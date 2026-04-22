# Android makefile for display kernel modules

AYN_DLKM_ENABLE := true
ifeq ($(TARGET_KERNEL_DLKM_DISABLE), true)
       ifeq ($(TARGET_KERNEL_DLKM_AYN_OVERRIDE), false)
               AYN_DLKM_ENABLE := false
       endif
endif

ifeq ($(AYN_DLKM_ENABLE),  true)
       AYN_SELECT := CONFIG_MSM_AYN=m
       BOARD_OPENSOURCE_DIR ?= vendor/qcom/opensource
       BOARD_COMMON_DIR ?= device/qcom/common

       LOCAL_PATH := $(call my-dir)

       include $(CLEAR_VARS)

       # This makefile is only for DLKM
       ifneq ($(findstring vendor,$(LOCAL_PATH)),)

       ifneq ($(findstring opensource,$(LOCAL_PATH)),)
               AYN_BLD_DIR := $(shell pwd)/$(BOARD_OPENSOURCE_DIR)/ayn-drivers
       endif # opensource

       DLKM_DIR := $(TOP)/$(BOARD_COMMON_DIR)/dlkm

       LOCAL_ADDITIONAL_DEPENDENCIES := $(wildcard $(LOCAL_PATH)/**/*) $(wildcard $(LOCAL_PATH)/*)

       # Build
       ###########################################################
       # This is set once per LOCAL_PATH, not per (kernel) module
       KBUILD_OPTIONS := AYN_ROOT=$(AYN_BLD_DIR)

       KBUILD_OPTIONS += MODNAME=ayn_dlkm
       KBUILD_OPTIONS += BOARD_PLATFORM=$(TARGET_BOARD_PLATFORM)
       KBUILD_OPTIONS += $(AYN_SELECT)

       ###########################################################

ifeq ($(TARGET_BOARD_PLATFORM), kalama)

       ###########################################################
        include $(CLEAR_VARS)
        LOCAL_SRC_FILES   := $(wildcard $(LOCAL_PATH)/**/*) $(wildcard $(LOCAL_PATH)/*)
        LOCAL_MODULE              := ayn-miniled.ko
        LOCAL_MODULE_KBUILD_NAME  := ayn-miniled.ko
        LOCAL_MODULE_TAGS         := optional
        #LOCAL_MODULE_DEBUG_ENABLE := true
        LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
        include $(DLKM_DIR)/Build_external_kernelmodule.mk
        ###########################################################

endif
       endif # DLKM check
endif
