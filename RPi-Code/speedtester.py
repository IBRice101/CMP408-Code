#!/usr/bin/env python3

"""
Name: speedtest.py
Desc: a program to test the speed of a computer network
Auth: Isaac Basque-Rice
Date: 24/12/2022
"""

import time
import speedtest

def test():
    """ Performs the speedtest and prints download, upload speeds and ping """
    st = speedtest.Speedtest()

    servernames = []
    st.get_servers(servernames)

    down_speed = st.download()
    up_speed = st.upload()
    ping = st.results.ping

    # TODO: send to JSON object
    print("Download speed: " + str(int(down_speed)) + " b/s")
    print("Upload speed: " + str(int(up_speed)) + " b/s")
    print("Ping: " + str(int(ping)) + " ms")

def main():
    """ The main function """
    start = time.time()
    now = time.ctime(start)

    print("Beginning speedtest: " + str(now))

    test()

    end = time.time()

    print("Speedtest completed in " + str(int(end-start)) + " seconds")

if __name__ == "__main__":
    main()
