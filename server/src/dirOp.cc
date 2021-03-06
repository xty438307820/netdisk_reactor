#include <string>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//filemode: d -> 目录, - -> 文件, 内部使用
char __getMode(mode_t mode){
    int high4 = mode >> 12;
    if(high4 == 8) return '-';
    else if(high4 == 4) return 'd';
}

//内部使用
std::string __getTime(char* tim){
    int i;
    int flag = 0;
    int len = strlen(tim);
    std::string tim_str;
    for(i = 0;i < len;i++){
        if(tim[i] == ' '){
            i++;
            break;
        }
    }
    while(1){
        if(tim[i] == ':'){
            flag++;
            if(flag == 2) break;
        }
        tim_str += tim[i++];
    }
    return tim_str;
}

//传入参数为ls路径
//example: myls("./"), 注意参数最后要有'/'
std::string myls(const char* path){
    struct dirent *p;
    DIR *pdir = opendir(path);
    struct stat sta;
    std::string lsInfo;
    char buf[288] = {0};
    while((p=readdir(pdir))!=NULL){
        if(!strcmp(".",p->d_name) || !strcmp("..",p->d_name)) continue;
        sprintf(buf,"%s%s",path,p->d_name);
        stat(buf,&sta);

        char mode = __getMode(sta.st_mode);
        long size = sta.st_size;
        std::string tim( __getTime(ctime(&sta.st_mtime)) );

        sprintf(buf,"%c %12ld %s %s\n",mode,size,tim.c_str(),p->d_name);
        lsInfo += buf;
    }
    closedir(pdir);
    if(lsInfo.size() > 0) lsInfo.pop_back();
    return lsInfo;
}

//return -1代表文件夹创建失败, 同名文件或文件夹存在
int myMkdir(const char* path){
    return mkdir(path,0755);
}

int myRemove(const char* path){
    return remove(path);
}

int testOpenDir(const char* path){
    DIR* pdir = opendir(path);
    if(pdir == NULL) return -1;
    closedir(pdir);
    return 0;
}

//return 0表示文件存在
int fileExist(const char* path){
    return access(path,F_OK);
}