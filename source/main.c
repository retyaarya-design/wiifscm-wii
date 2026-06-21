#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <network.h>
#include <lwip/sockets.h>

int main(void) {
    VIDEO_Init();
    WPAD_Init();
    void *fb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(&TVNtsc480IntDf));
    VIDEO_Configure(&TVNtsc480IntDf);
    VIDEO_SetNextFramebuffer(fb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
    console_init(fb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    fatInitDefault();
    net_init();
    struct in_addr addr;
    addr.s_addr = net_gethostip();
    char ip[16];
    strncpy(ip, inet_ntoa(addr), 15);
    printf("\033[2J");
    printf("================================\n");
    printf("  Wii FSCM Server\n");
    printf("  IP: %s\n", ip);
    printf("  FTP: Port 21\n");
    printf("  HTTP: Port 8080\n");
    printf("================================\n");
    printf("\nPress HOME to exit\n");
    int ftp_srv = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in fa;
    memset(&fa, 0, sizeof(fa));
    fa.sin_family = AF_INET;
    fa.sin_port = htons(21);
    fa.sin_addr.s_addr = INADDR_ANY;
    bind(ftp_srv, (struct sockaddr*)&fa, sizeof(fa));
    listen(ftp_srv, 3);
    int http_srv = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ha;
    memset(&ha, 0, sizeof(ha));
    ha.sin_family = AF_INET;
    ha.sin_port = htons(8080);
    ha.sin_addr.s_addr = INADDR_ANY;
    bind(http_srv, (struct sockaddr*)&ha, sizeof(ha));
    listen(http_srv, 3);
    int running = 1;
    while (running) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) running = 0;
        struct sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int fc = accept(ftp_srv, (struct sockaddr*)&cli, &len);
        if (fc >= 0) {
            send(fc, "220 Wii FSCM FTP\r\n", 18, 0);
            char buf[4096];
            while (recv(fc, buf, 4095, 0) > 0) {
                if (strncmp(buf, "QUIT", 4) == 0) break;
                send(fc, "200 OK\r\n", 8, 0);
            }
            close(fc);
        }
        int hc = accept(http_srv, (struct sockaddr*)&cli, &len);
        if (hc >= 0) {
            char buf[4096] = {0};
            recv(hc, buf, 4095, 0);
            char *r = "HTTP/1.1 200 OK\r\n\r\n<html><body style='bg:#000;color:#fff'><h1>Wii FSCM</h1></body></html>";
            send(hc, r, strlen(r), 0);
            close(hc);
        }
        VIDEO_WaitVSync();
    }
    close(ftp_srv);
    close(http_srv);
    return 0;
}
