#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>

#define FB_PATH "/dev/fb0"
#define WIDTH 600
#define HIGHT 600

unsigned int *fb_addr = NULL;

void draw_back( unsigned int width, unsigned int hight);

int main(int argc, char **argv)
{
    int fb = -1, ret = -1;
    struct fb_fix_screeninfo fb_finfo;
    struct fb_var_screeninfo fb_vinfo;

    // 第一步：打开设备
    fb = open(FB_PATH, O_RDWR);
    if(fb < 0)
    {
        perror("open failed");
        return -1;
    }

    printf("open %s success, fb = %d\n", FB_PATH, fb);
    memset(&fb_finfo, 0x00, sizeof(fb_finfo));
    memset(&fb_vinfo, 0x00, sizeof(fb_vinfo));

    // 第2步：获取fb设备信息
    ret = ioctl(fb, FBIOGET_FSCREENINFO, &fb_finfo);
    if(ret < 0) {
        perror("ioctl failed");
        close(fb);
    }

    printf("id:%s\n", fb_finfo.id);
    printf("smem_start:%x,\n", fb_finfo.smem_start);
    printf("smem_len:%u\n", fb_finfo.smem_len);
    printf("mmio_start:%x\n", fb_finfo.mmio_start);

    ret = ioctl(fb, FBIOGET_VSCREENINFO, &fb_vinfo);
    if(ret < 0) {
        perror("ioctl failed");
        close(fb);
    }

    printf("xres:%u\n", fb_vinfo.xres);
    printf("yres:%u\n", fb_vinfo.yres);
    printf("xres_virtual:%u\n", fb_vinfo.xres_virtual);
    printf("yres_virtual:%u\n", fb_vinfo.yres_virtual);
    printf("xoffset:%u\n", fb_vinfo.xoffset);
    printf("yoffset:%u\n", fb_vinfo.yoffset);
    printf("bits_per_pixel:%u\n", fb_vinfo.bits_per_pixel);

    // 修改屏幕的分辨率
    fb_vinfo.xres = 600;
    fb_vinfo.yres = 480;
    fb_vinfo.xres_virtual = 600;
    fb_vinfo.yres_virtual = 960;
    unsigned int size = fb_vinfo.xres_virtual * fb_vinfo.yres_virtual * fb_vinfo.bits_per_pixel / 8;
    ret = ioctl(fb, FBIOPUT_VSCREENINFO, &fb_vinfo);
    if(ret < 0) {
        perror("ioctl failed");
        close(fb);
    } 

    printf("****************************************\n");

    ret = ioctl(fb, FBIOGET_VSCREENINFO, &fb_vinfo);
    if(ret < 0) {
        perror("ioctl failed");
        close(fb);
    }

    printf("xres:%u\n", fb_vinfo.xres);
    printf("yres:%u\n", fb_vinfo.yres);
    printf("xres_virtual:%u\n", fb_vinfo.xres_virtual);
    printf("yres_virtual:%u\n", fb_vinfo.yres_virtual);
    printf("xoffset:%u\n", fb_vinfo.xoffset);
    printf("yoffset:%u\n", fb_vinfo.yoffset);
    printf("bits_per_pixel:%u\n", fb_vinfo.bits_per_pixel);

printf("****************************************\n");

    ret = ioctl(fb, FBIOGET_FSCREENINFO, &fb_finfo);
    if(ret < 0) {
        perror("ioctl failed");
        close(fb);
    }

    printf("id:%s\n", fb_finfo.id);
    printf("smem_start:%x,\n", fb_finfo.smem_start);
    printf("smem_len:%u\n", fb_finfo.smem_len);
    printf("mmio_start:%x\n", fb_finfo.mmio_start);
printf("****************************************\n");


    // 第三步：mmap
    fb_addr = mmap(NULL,  size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fb, 0);
    if(fb_addr == NULL) {
        perror("mmap error");
        close(fb);
        return -1;
    }

    printf("fb_addr:%p\n", fb_addr);

    // 第四步：刷背景
    draw_back( WIDTH, HIGHT);

    close(fb);

    return 0;
}

void draw_back( unsigned int width, unsigned int hight)
{
    unsigned int x, y;

    for (y = 0; y < width; y++) {
        for(x = 0; x < hight; x++) {
            *(fb_addr + y*WIDTH  + x) = 0xffffffff;
        }
    }
}