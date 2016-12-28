import sip
sip.setapi('QString', 2)
sip.setapi('QVariant', 2)

import sys
tempDir = r"C:\temp"
    
if tempDir not in sys.path:
    sys.path.append(tempDir)