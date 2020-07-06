#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void inotify_read(int fd){
	
	char read_buf[512] = {0};
	int size = 0;
	int pos  = 0;
	struct inotify_event *evt;
	
	int ret = read(fd,read_buf,sizeof(read_buf));
	if (ret < sizeof(*evt)){
		perror("read");
		return;
	}
	
	while (ret >= sizeof(*evt)){
		evt = (struct inotify_event*)(read_buf+pos);
		if (evt->len){
			if (evt->mask & IN_CREATE){
				printf ("file create %s\r\n",evt->name);
			}else{
				printf ("file delete% s\r\n",evt->name);
			}
		}
		size = (sizeof(*evt)+evt->len);
		ret -= size;
		pos += size;
		
	}
	
}

int main (int argc,char *argv[]){
	
	int fd = 0;
	fd = inotify_init();
	int err =  inotify_add_watch(fd,"./", IN_CREATE|IN_DELETE);
	if (err < 0){
		perror("inotify_add_watch");
		return -1;
	}
	
	while (1){
		inotify_read(fd);
		sleep(3);
	}
	
	
	
	return 0;
}
