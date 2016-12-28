import os

try:
    uiFile = os.path.join(os.path.dirname(__file__),"fileReader_1.ui")
except NameError:
    uiFile = os.path.join(os.getcwd(),"fileReader_1.ui")

print uiFile
