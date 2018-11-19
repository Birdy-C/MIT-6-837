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
    try:
		u = urllib.urlopen(url)
		print "Successful"
    except:
		print "Unsuccessful"
    f = open(file_name, 'wb')

    block_sz = 8192
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break
        f.write(buffer)
    f.close()
    print ("Sucessful to download" + " " + file_name)


def getFile(url, html):  
    r=r'a href=\"(lectures/.*.pdf)\"'
    fileRE = re.compile(r)  
    fileList = re.findall(fileRE,html)  
    print fileList
    for fileURL in fileList:    
        # I don't know how to write regular expression there
        if(fileURL.find('ppt')==-1):
           getPDF("https://groups.csail.mit.edu/graphics/classes/6.837/F04/" + fileURL,fileURL)



url = "https://groups.csail.mit.edu/graphics/classes/6.837/F04/calendar.html"
html=getHtml(url)
#print html
getFile(url,html)  