BASEDIR	:= $(dir $(firstword $(MAKEFILE_LIST)))
VPATH	:= $(BASEDIR)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
# ROMFS is a directory that will be available as romfs:/
#---------------------------------------------------------------------------------
TARGET		:=	ufile
BUILD		:=	build
SOURCES		:=	source source/Overlay source/GUI source/Entry source/Device source/DiscInterface source/Device/PhysicalDevice
INCLUDES	:=	
ROMFS		:=	romfs
include $(WUT_ROOT)/share/romfs-wiiu.mk

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		+=	-O2 -DUSE_FILE32API -DNOCRYPT -DINPUT_JOYSTICK -DMUSIC -D_XOPEN_SOURCE $(ROMFS_CFLAGS) $(INCLUDES)
CXXFLAGS	+=	$(CFLAGS)
LDFLAGS		+=	$(WUT_NEWLIB_LDFLAGS) $(WUT_STDCPP_LDFLAGS) $(WUT_DEVOPTAB_LDFLAGS) $(ROMFS_LDFLAGS) \
				-lcoreinit -lvpad -lsndcore2 -lsysapp -lnsysnet -lnlibcurl -lproc_ui -lgx2 -lgfd -lwhb \
				-lfreetype -lpng -lmpg123 -lvorbisidec -ljpeg -lz -lbz2 \
				-lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_ttf -liosuhax -lfat -lntfs -ltween

#---------------------------------------------------------------------------------
# get a list of objects
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
SFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.s))
OBJECTS		:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o) $(ROMFS_TARGET)

#---------------------------------------------------------------------------------
# objectives
#---------------------------------------------------------------------------------
$(TARGET).rpx: $(OBJECTS)

clean:
	rm -rf $(TARGET).rpx $(TARGET).rpx.elf $(OBJECTS) $(OBJECTS:.o=.d)

#---------------------------------------------------------------------------------
# wut
#---------------------------------------------------------------------------------
WUT_KEEP_RPLELF	:=	1
include $(WUT_ROOT)/share/wut.mk

#---------------------------------------------------------------------------------
# portlibs
#---------------------------------------------------------------------------------
PORTLIBS	:=	$(DEVKITPRO)/portlibs/ppc
PORTLIBS2	:=	$(DEVKITPRO)/portlibs/wiiu
LDFLAGS		+=	-L$(PORTLIBS)/lib -L$(PORTLIBS2)/lib
CFLAGS		+=	-I$(PORTLIBS)/include -I$(PORTLIBS2)/include
CXXFLAGS += -I$(PORTLIBS)/include -I$(PORTLIBS2)/include