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
    r=r'href\s{0,1}=\s{0,1}\"(.*.txt|.*.h|.*.C)\"' # in assignments 1&2 there is a space around =
    fileRE = re.compile(r)  
    fileList = re.findall(fileRE,html)  
    for fileURL in fileList:  
        directory = "file/assignment"+ assignment
        if not os.path.exists(directory):
            os.makedirs(directory)
        urllib.urlretrieve(url + "/" + fileURL, directory + "/" + fileURL)  
        print "assignment"+ assignment +" - "+ fileURL


for i in range(10):
# for i in [1,2]:
    url = "https://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment"+ str(i) 
    html=getHtml(url)
    #print html
    getFile(url,html,str(i))  
