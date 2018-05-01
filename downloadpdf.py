#file-name: download.py
#@author: Birdy 2018/4/19

import os
import re   
import urllib  
def getHtml(url):  
    page=urllib.urlopen(url)  
    html=page.read()  
    return html  

def getFile(uri, html):  
    r=r'a href=\"(.*.pdf)\" tppabs='
    fileRE = re.compile(r)  
    fileList = re.findall(fileRE,html)  
    # filename=1  
    print fileList
    for fileURL in fileList:  
        #print url[:-9] + fileURL
        #if not os.path.exists(directory):
        #    os.makedirs(directory)
        urllib.urlretrieve(url[:-9] + fileURL, fileURL)  

        #print 'file "%s" done' %fileURL 
        print fileURL
        #filename+=1  


url = "http://10.76.1.181/courses/training/mitF04/calendar.html"
html=getHtml(url)
#print html
getFile(url,html)  