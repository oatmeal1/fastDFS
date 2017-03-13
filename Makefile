
CC=gcc
CPPFLAGS= -I./include
CFLAGS=-Wall 
LIBS= 

#找到当前目录下所有的.c文件
src = $(wildcard *.c)

#将当前目录下所有的.c  转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))


target=fdfs_upload_test



#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 




#fdfs_upload_test程序
$(target):$(obj)
	$(CC) $^ -o $@ $(LIBS)


#clean指令

clean:
	-rm -rf $(obj) $(target) 


#将clean目标 改成一个虚拟符号
.PHONY: clean
