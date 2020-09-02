#!/usr/bin/python3

# c.f. https://qiita.com/croquisdukke/items/9c5d8933496ba6729c78

import time
import os
import sys

def main_unit():
    while True:
        filepath = '/opt/aura_stat_dm.log'
        log_file = open(filepath, 'a')
        try:
            log_file.write(time.ctime() + "\n")
        finally:
            log_file.close()
            time.sleep(15 * 60)

def daemonize():
    pid = os.fork()
    if pid > 0:  # Parent
        with open('/var/run/aura_status_client_daemon.pid', 'w') as pid_file
            pid_file.write(str(pid) + "\n")
        sys.exit()

    if pid == 0:  # Child
        main_unit()

if __name__ == '__main__':
    while True:
        daemonize()
