import sys, tempfile
tempDir = tempfile.gettempdir()
    
if tempDir not in sys.path:
    sys.path.append(tempDir)