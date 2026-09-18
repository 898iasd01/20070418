#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>  
#include <sys/types.h>  
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <regex.h>
  int main(void)
  {
      char *regstr = "<a href=\"([^\"]+)\">([^<]+)</a>";
  
      regex_t reg;
      regcomp(&reg,regstr,REG_EXTENDED);
  
      int fd;
      fd = open("Utl.txt",O_RDWR);
      int size;
      size = lseek(fd,0,SEEK_END);
  
      char *mmap_data = NULL;
    mmap_data = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
     close(fd);
 
      printf("%s\n",mmap_data);
 
      int regnum = 3;
      regmatch_t match[regnum];
      char link[1024];
      char title[1024];
 
      int offset = 0;
      while(regexec(&reg, mmap_data + offset, regnum, match,0)==0)
      {
            bzero(link,sizeof(link));
            bzero(title,sizeof(title));
            regoff_t url_start = offset + match[1].rm_so;
            regoff_t url_len = match[1].rm_eo - match[1].rm_so;
            regoff_t title_start = offset + match[2].rm_so;
            regoff_t title_len = match[2].rm_eo - match[2].rm_so;

	     snprintf(link, sizeof(link), "%.*s", url_len, mmap_data + url_sta    rt);
            snprintf(title, sizeof(title), "%.*s", title_len, mmap_data + tit    le_start);
            printf("匹配结果, title = %s    link = %s\n", title, link);
            offset += match[0].rm_eo;
       }
      regfree(&reg);
      return 0;
  }

