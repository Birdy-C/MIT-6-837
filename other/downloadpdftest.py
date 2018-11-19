#file-name: download.py
#@author: Birdy 2018/4/19

import os
import re   
import urllib
import requests
def getHtml(url):  
    page=urllib.urlopen(url)  
    html=page.read()  
    return html  

def getPDF(url,file_name):
    print url
    print file_name
    u = urllib.urlopen(url)
    f = open(file_name, 'wb')
    block_sz = 8192
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break
        f.write(buffer)
    f.close()
    print ("Sucessful to download" + " " + file_name)



getPDF("http://10.76.1.181/courses/training/mitF04/calelectures/00_Intro.pdf","lectures/00_Intro.pdf")