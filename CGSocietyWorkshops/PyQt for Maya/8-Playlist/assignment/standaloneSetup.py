import os, sys

os.environ["MAYA_LOCATION"] = "C:\Program Files\Autodesk\Maya2012"
os.environ["PYTHONHOME"]    = "C:\Program Files\Autodesk\Maya2012\Python"
os.environ["PATH"] = "C:\\Program Files\\Autodesk\\Maya2012\\bin;" + os.environ["PATH"]

sys.path.append("C:\Program Files\Autodesk\Maya2012\bin\python26.zip")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python\DLLs")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python\lib")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python\lib\plat-win")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python\lib\lib-tk")
sys.path.append("C:\Program Files\Autodesk\Maya2012\bin")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python")
sys.path.append("C:\Program Files\Autodesk\Maya2012\Python\lib\site-packages")
