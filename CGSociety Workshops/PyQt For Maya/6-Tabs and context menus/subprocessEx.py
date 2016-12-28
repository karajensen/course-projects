import subprocess

#Opens jedit
subprocess.Popen(["jedit"])

#Opens a file into jedit
subprocess.Popen(["jedit", r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour6\code\finalFileReader_1.pyw"])

#Launches the same file with python
subprocess.Popen(["python", r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour6\code\finalFileReader_1.pyw"])

#Creates an instance of the Popen class and the stdout and stderr are going into subprocess
proc = subprocess.Popen(["python", r"C:\Users\Malcolm\Dropbox\cgWorkshopsUpload\hour6\code\finalFileReader_1.pyw"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

#If you run the rest of the code after you've run the proc above
#It will print nothing 1000 times, then wait for the subprocess to finish
#The it will push the out and err from the subprocess for you to use
#Followed by another print
print "nothing\n"*1000
proc.wait()
out, err = proc.communicate()
print "something awesome"



