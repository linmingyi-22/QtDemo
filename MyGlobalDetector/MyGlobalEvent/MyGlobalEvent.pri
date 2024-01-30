HEADERS += \
    $$PWD/MyGlobalKeyEvent.h \           # 全局键盘事件监听器类头文件
    $$PWD/MyGlobalMouseEvent.h           # 全局鼠标事件监听类头文件

SOURCES += \
    $$PWD/MyGlobalKeyEvent.cpp \     # Windows全局键盘事件监听器
    $$PWD/MyGlobalMouseEvent.cpp \   # windows全局鼠标事件监听器

win32 {
LIBS+= -luser32           # 使用WindowsAPI需要链接库
}
