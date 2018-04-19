#file-name: download.py
#@author: Birdy 2018/4/19

import os
import re   
import urllib  
def getHtml(url):  
    page=urllib.urlopen(url)  
    html=page.read()  
    return html  

def getFile(uri, html, assignment):  
    r=r'href=\"(.*.txt|.*.h|.*.C)\" tppabs='
    fileRE = re.compile(r)  
    fileList = re.findall(fileRE,html)  
    # filename=1  
    for fileURL in fileList:  
        #print url[:-9] + fileURL
        directory = "file/assignment"+ assignment
        if not os.path.exists(directory):
            os.makedirs(directory)
        urllib.urlretrieve(url[:-9] + fileURL, directory + "/" + fileURL)  

        #print 'file "%s" done' %fileURL 
        print "assignment"+ assignment +" - "+ fileURL
        #filename+=1  


# url=raw_input("please input the source url:")  
for i in range(10):
    url = "http://10.76.1.181/courses/training/mitF04/assignments/assignment"+ str(i) +"/index.htm"
    html=getHtml(url)
    getFile(url,html,str(i))  
