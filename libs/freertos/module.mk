ifeq ($(USE_FREERTOS),y)

    FREERTOS_BASE=libs/freertos
    FREERTOS_HEAP_TYPE ?= 4
    
    DEFINES+=USE_FREERTOS
    INCLUDES += -I$(FREERTOS_BASE)/include

    SRC+=$(wildcard $(FREERTOS_BASE)/source/*.c)
    SRC+=$(FREERTOS_BASE)/portable/MemMang/heap_$(FREERTOS_HEAP_TYPE).c

    ifeq ($(USE_FPU),y)
        INCLUDES += -I$(FREERTOS_BASE)/portable/GCC/ARM_CM4F
        SRC+=$(FREERTOS_BASE)/portable/GCC/ARM_CM4F/port.c
    else
        INCLUDES += -I$(FREERTOS_BASE)/portable/GCC/ARM_CM3
        SRC+=$(FREERTOS_BASE)/portable/GCC/ARM_CM3/port.c
    endif
endif
