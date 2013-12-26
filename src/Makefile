VERSION		=	0.1

FILE		=	main

TARGET		=	$(FILE).mot

CFLAGS		=	-O0 -mh -DH8_3052

TOOL_PREFIX	=	h8300-elf-

CC			=	$(TOOL_PREFIX)gcc
AS			=	$(TOOL_PREFIX)as
LD			=	$(TOOL_PREFIX)ld

LDSCRIPT	=	../include/h8rom.x
CRT0		=	../include/h8crt0.s	
ASRCS		=	
SRCS		=	$(FILE).c

FW			=   h8write
CPU			=	3052

INC_PATH 	= /home/tomoya/development/h8_etc/3052/include
LIB_PATH	= /home/tomoya/development/h8_etc/3052/include

#	make all source trees
all : $(TARGET)



$(FILE).mot: $(FILE).exe
	$(TOOL_PREFIX)objcopy -O srec $(FILE).exe $@


$(FILE).exe:  Makefile $(LDSCRIPT) $(SRCS:.c=.o) $(CRT0) $(ASRCS) $(LIBS)
	$(TOOL_PREFIX)gcc -L$(LIB_PATH) -I$(INC_PATH) $(CFLAGS) -T $(LDSCRIPT) -nostdlib $(CRT0) $(ASRCS) $(SRCS:.c=.o) -o $@ $(LIBS) -lc -lgcc

tar:
	cd .. && tar jcvf $(FILE)-$(VERSION)-`date '+%Y%m%d'`.tar.bz2 $(FILE)

flash:
	$(FW) -$(CPU) $(TARGET)

clean :
	rm -f $(TARGET)
	rm -f $(FILE).exe
	rm -f $(SRCS:.c=.o)
