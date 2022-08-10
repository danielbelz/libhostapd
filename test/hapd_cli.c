/*
 * hapd_cli - test application for dynamic libhostapd.so
 * Copyright (c) 2022, D. Belz
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wpa_ctrl.h"
#ifdef CONFIG_CTRL_IFACE_UNIX
#include <dirent.h>
#endif /* CONFIG_CTRL_IFACE_UNIX */
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>

#define OK 0
#define ERROR -1

static char* hostapd_var_run = "/var/run/hostapd/wlan0";

static struct wpa_ctrl* ctrl_conn = NULL;
static struct wpa_ctrl *req_conn = NULL;

static int ApCheckCtrlFile()
{
	if (access(hostapd_var_run, F_OK) != 0) {
		printf("hostapd seems to not be running\n");
		// Seems to not be running
		return ERROR;
	}
    printf("APD Ctrl is available\n");
	return OK;
}

int wifi_ApCtrlRecvMessage(char *buf, size_t *sz)
{
    return(wpa_ctrl_recv(ctrl_conn, buf, sz));
}

static int ApCtrlPing()
{
	char repl_str[16] = { "\0" };
	size_t reply_len;
	int ret = 0;
	memset(repl_str, '\0', sizeof(repl_str));
	reply_len = sizeof(repl_str) - 1;
	ret = wpa_ctrl_request(req_conn, "PING", strlen("PING"), repl_str, &reply_len, NULL);
	if (ret == 0) {
		printf("APD PING responded %s\n", repl_str);
		if (strncmp(repl_str, "PONG", 4) == 0) {
			return OK;
		}
	}
	printf("APD PING didn't respond\n");
	return ERROR;
}

static int ApCtrlOpen(struct wpa_ctrl **conn)
{
    struct wpa_ctrl *lo_conn = NULL;
	if (ApCheckCtrlFile() == ERROR) {
        printf("Cannot open. APD Ctrl file not available\n");
		return ERROR;
	}

    printf("Trying to open APD Ctrl\n");
    lo_conn = wpa_ctrl_open(hostapd_var_run);
    if (!lo_conn) {
        printf("ERROR Failed to connect to hostapd\n");
        *conn = NULL;
        return ERROR;
    }

    if(wpa_ctrl_attach(lo_conn) != 0) {
        printf("ERROR Attach monitor conn failed\n");
        return ERROR;
    }

    *conn = lo_conn;
    printf("APD Ctrl ret %p:%p\n", lo_conn, *conn);
	return OK;
}

static int ApCtrlClose(struct wpa_ctrl *lo_conn)
{
	if (lo_conn) {
		wpa_ctrl_close(lo_conn); // this calls free
		lo_conn = NULL;
	}
	return OK;
}

int main(void)
{
    printf ("Starting test application. Opening the sockets to hostapd\n");
    if (ApCtrlOpen(&req_conn) == ERROR) {
        return ERROR;
    }
    if (ApCtrlOpen(&ctrl_conn) == ERROR) {
        ApCtrlClose(req_conn);
        return ERROR;
    }

    ApCtrlPing();

    ApCtrlClose(ctrl_conn);
    ApCtrlClose(req_conn);
    return 0;
}

