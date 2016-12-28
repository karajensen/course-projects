from PyQt4.QtGui import QComboBox

newItem = QComboBox()
newItem.addItem("item name", 1)
newItem.addItem("item name 2", "some name")

#With API 1
print newItem.itemData(0).toString()

#With API 2
print newItem.itemData(0)

