from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4 import uic
import os, sys, sip, re, cPickle

uiFile = os.path.join(os.path.dirname(__file__),"musicList.ui")
libFile = os.path.join(os.path.dirname(__file__),"music.lib")
    
windowTitle = "Music List"
windowObject = "musicListWin"

class MusicListWin(QMainWindow):
    def __init__(self, parent=None):
        """Super, loadUi, signal connections"""
        super(MusicListWin, self).__init__(parent)
        
        #Connects the ui elements to self
        uic.loadUi(uiFile, self)
        
        self.actionAdd_Folder_Site.triggered.connect(self.addFolderSiteDia)
        self.actionRemove_Folder_Site.triggered.connect(self.removeFolderSiteDia)

        self.actionRefresh_Folders.triggered.connect(self.refreshFolderSites)

        self.artist_lw.currentItemChanged.connect(self.fillAlbum)
        self.album_lw.currentItemChanged.connect(self.fillSong)
        self.song_lw.currentItemChanged.connect(self.printSongInfo)

        self.addPL_but.clicked.connect(self.addToPlaylist)
        self.removePL_but.clicked.connect(self.removeFromPlaylist)
        self.printPL_but.clicked.connect(self.printPlaylist)

        self.playlist_comB.setContextMenuPolicy(Qt.CustomContextMenu)
        self.playlist_comB.customContextMenuRequested.connect(self.playlistContext)

        self.playlist_comB.currentIndexChanged.connect(self.loadPlaylist)
        
        self.fillSites()
        self.fillPlaylist()
        self.readSettings()
        self.noDelete = True
        
    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.close()
        super(MusicListWin, self).keyPressEvent(event)  

    def closeEvent(self, event):
        if self.noDelete:
            self.writeSettings()
            self.noDelete = False
        event.accept()

    def readSettings(self):
        print "reading"
        settings = QSettings(windowObject, windowTitle)

        if settings.contains(windowObject+"/pos"):
            self.move(settings.value(windowObject+"/pos").toPoint())
        if settings.contains(windowObject+"/size"):
            self.resize(settings.value(windowObject+"/size").toSize())
        if settings.contains(windowObject+"/selected"):
            index = self.playlist_comB.findText(settings.value(windowObject+"/selected").toString())
            if index != -1:
                self.playlist_comB.setCurrentIndex(index)

    def writeSettings(self):
        print "writing"
        settings = QSettings(windowObject, windowTitle)
        settings.setValue(windowObject+"/pos", self.pos())
        settings.setValue(windowObject+"/size", self.size())
        settings.setValue(windowObject+"/selected", self.playlist_comB.currentText())

    #------------------
    # Dealing with folderSites
    # fillSites
    # addFolderSiteDia
    # removeFolderSiteDia
    # removeFolderSiteDia_remove
    # removeFolderSiteDia_ok
    # rebuildFolderSites
    # refreshFolderSites
    #------------------

    def fillSites(self):
        acts = self.menuFolder_Sites.actions()
        for act in acts:
            self.menuFolder_Sites.removeAction(act)
            
        self.sites = []
        if os.path.exists(libFile):
            fObj = open(libFile, "r")
            fRead = fObj.readlines()
            fObj.close()
            for line in sorted(fRead):
                act = QAction(line, self)
                self.menuFolder_Sites.addAction(act)
                act.folderLocation = line
                act.setCheckable(True)
                act.setChecked(True)
                act.toggled.connect(self.refreshFolderSites)
                self.sites.append(line)
                
        self.fillArtist()
    
    def addFolderSiteDia(self):
        fileDialog = QFileDialog()
        folderName = fileDialog.getExistingDirectory(None, "Select Folder")
        if folderName:
            fObj = open(libFile, "a")
            fObj.write(folderName+"\n")
            fObj.close()
            
        self.fillSites()
            
    def removeFolderSiteDia(self):
        self.removeDia = QDialog()
        self.removeDia.setWindowTitle("Remove Folders")
        self.removeDia.resize(500, 300)
        
        self.removeGrid = QGridLayout(self.removeDia)
        
        self.remove_lw = QListWidget()
        self.removeGrid.addWidget(self.remove_lw, 0,0,4,1)
        
        self.removeDiaSpacer = QSpacerItem(20, 229, QSizePolicy.Minimum, QSizePolicy.Expanding)
        self.removeGrid.addItem(self.removeDiaSpacer, 0,1,1,1)

        self.removeDiaRemoveBtn = QPushButton("Remove", self.removeDia)
        self.removeDiaRemoveBtn.clicked.connect(self.removeFolderSiteDia_remove)
        self.removeGrid.addWidget(self.removeDiaRemoveBtn, 1, 1, 1, 1)

        self.removeDiaOkBtn = QPushButton("Ok", self.removeDia)
        self.removeDiaOkBtn.clicked.connect(self.removeFolderSiteDia_ok)
        self.removeGrid.addWidget(self.removeDiaOkBtn, 2, 1, 1, 1)

        self.removeDiaCancelBtn = QPushButton("Cancel", self.removeDia)
        self.removeDiaCancelBtn.clicked.connect(self.removeDia.reject)
        self.removeGrid.addWidget(self.removeDiaCancelBtn, 3, 1, 1, 1)
        
        acts = self.menuFolder_Sites.actions()
        for act in acts:
            self.remove_lw.addItem(act.folderLocation.strip())
        
        self.removeDia.exec_()
        
    def removeFolderSiteDia_remove(self):
        self.remove_lw.takeItem(self.remove_lw.currentRow())
        
    def removeFolderSiteDia_ok(self):
        self.sites = []
        for i in range(self.remove_lw.count()):
            self.sites.append(self.remove_lw.item(i).text())
            
        self.rebuildFolderSites()
        self.removeDia.accept()
    
    def rebuildFolderSites(self):
        fObj = open(libFile, "w")
        for site in self.sites:
            fObj.write(site+"\n")
        fObj.close()
        self.fillSites()
        
    def refreshFolderSites(self):
        self.sites = []
        acts = self.menuFolder_Sites.actions()
        for act in acts:
            if act.isChecked():
                self.sites.append(act.folderLocation)
        self.fillArtist()
        
    #------------------
    # List Widget Work
    # fillArtist
    # fillAlbum
    # fillSong
    # printSongInfo
    #------------------
    
    def fillArtist(self):
        self.artistDict = {}
        self.artist_lw.clear()
        
        artistList = set([])
        for site in self.sites:
            site = site.strip()
            if os.path.exists(site):
                artists = os.listdir(site)
                for artist in artists:
                    artistList.add(artist)
                    if artist not in self.artistDict.keys():
                        self.artistDict[artist] = [site]
                    else:
                        self.artistDict[artist].append(site)
                        
        artistList = list(artistList)
        artistList.sort()
        self.artist_lw.addItems(artistList)
        self.artist_lw.setCurrentRow(0)
    
    def fillAlbum(self):
        self.albumDict = {}
        self.album_lw.clear()
        self.info_tb.setPlainText("")
        
        curArtist = self.artist_lw.currentItem()
        if curArtist:
            curArtist = str(curArtist.text())
            for sitePath in self.artistDict[curArtist]:
                artistPath = os.path.join(sitePath, curArtist)
                if os.path.exists(artistPath):
                    albums = os.listdir(artistPath)
                    for album in albums:
                        if os.path.isdir(os.path.join(artistPath, album)):
                                self.albumDict[album] = os.path.join(artistPath, album)
            self.album_lw.addItems(sorted(self.albumDict.keys()))
  
    def fillSong(self):
        self.songDict = {}
        self.song_lw.clear()
        self.info_tb.setPlainText("")
        
        curAlbum = self.album_lw.currentItem()
        if curAlbum:
            curAlbum = str(curAlbum.text())
            albumPath = self.albumDict[curAlbum]
            if os.path.exists(albumPath):
                songs = os.listdir(albumPath)
                for song in songs:
                    p = re.compile(r".*\.mp3\b", re.I)
                    if p.match(song):
                        self.song_lw.addItem(song)
    
    def printSongInfo(self):
        curAlbum = self.album_lw.currentItem()
        if curAlbum:
            curAlbum = str(curAlbum.text())
            curAlbum = self.albumDict[curAlbum]
            curSong = self.song_lw.currentItem()
            if curSong:
                curSong = str(curSong.text())
                printInfo = "Full Path: "
                printInfo += os.path.join(curAlbum, curSong)
                self.info_tb.setPlainText(printInfo)
    #------------------
    # Playlist Creation
    # newPlaylist
    # fillPlaylist
    #------------------
    
    def newPlaylist(self, name):
        fObj = open(os.path.join(os.path.dirname(__file__), str(name)+".plt"), "w")
        cPickle.dump({"songs":[], "songsInfo":{}}, fObj)
        fObj.close()
        return name
        
    def fillPlaylist(self):
        files = os.listdir(os.path.dirname(__file__))
        
        self.playlists = []
        p = re.compile(r".*\.plt\b", re.I)
        for file_ in files:
            if p.match(file_):
                self.playlists.append(file_.split(".")[0])
                
        if len(self.playlists) == 0:
            self.playlists = [self.newPlaylist("Default")]
        
        self.playlist_comB.clear()
        self.playlist_comB.addItems(self.playlists)
        
        self.loadPlaylist()

    
    #------------------
    # Working with Playlists
    # addToPlaylist
    # loadPlaylist
    # removeFromPlaylist
    # printPlaylist
    #------------------
    
    def addToPlaylist(self):
        curAlbum = self.album_lw.currentItem()
        if curAlbum:
            curAlbum = str(curAlbum.text())
        else:
            return
            
        song = self.song_lw.currentItem()
        if song:
            song = str(song.text())
        else:
            return
            
        albumPath = self.albumDict[curAlbum]
        
        if albumPath:
            curPList = self.playlist_comB.currentText()
            fObj = open(os.path.join(os.path.dirname(__file__), str(curPList)+".plt"), "r")
            pList = cPickle.load(fObj)
            fObj.close()
            
            cleanSong = os.path.splitext(song)[0]
            
            pList["songs"].append(cleanSong)
            pList["songsInfo"][cleanSong] = os.path.join(albumPath, song)
            
            fObj = open(os.path.join(os.path.dirname(__file__), str(curPList)+".plt"), "w")
            cPickle.dump(pList, fObj)
            fObj.close()
        
            self.loadPlaylist()

    def loadPlaylist(self): 
        try:
            curPList = self.playlist_comB.currentText()
            fObj = open(os.path.join(os.path.dirname(__file__), str(curPList)+".plt"), "r")
            pList = cPickle.load(fObj)
            fObj.close()
            
            self.curPList = pList
            
            self.playlist_lw.clear()
            self.playlist_lw.addItems(self.curPList["songs"])
        except IOError:
            pass
        
    def removeFromPlaylist(self):
        curPList = self.playlist_comB.currentText()
        fObj = open(os.path.join(os.path.dirname(__file__), str(curPList)+".plt"), "r")
        pList = cPickle.load(fObj)
        fObj.close()
        
        cleanSong = self.playlist_lw.currentItem().text()
        
        for i in range(pList["songs"].count(cleanSong)):
            pList["songs"].remove(cleanSong)
            
        newDict = {}
        for key, value in pList["songsInfo"].items():
            if key != cleanSong:
                newDict[key] = value
                
        pList["songsInfo"] = newDict
        
        fObj = open(os.path.join(os.path.dirname(__file__), str(curPList)+".plt"), "w")
        cPickle.dump(pList, fObj)
        fObj.close()
        
        self.loadPlaylist()
    
    def printPlaylist(self):
        print "\n"*5
        print "SONG LISTING:\n"
        for song in self.curPList["songs"]:
            print self.curPList["songsInfo"][song]
            
    #------------------
    # Playlist Context Work
    # playlistContext
    # addPlaylistDia
    # addPlaylistFromDia
    # removePlaylistDia
    # renamePlaylistDia
    # renamePlaylistFromDia
    #------------------
    
    def playlistContext(self, pos):
        menu = QMenu()
        pos = QPoint(pos.x()+20, pos.y())
        menu.addAction("Add Playlist", self.addPlaylistDia)
        menu.addSeparator()
        menu.addAction("Remove Playlist", self.removePlaylistDia)
        menu.addSeparator()        
        menu.addAction("Rename Playlist", self.renamePlaylistDia)
        menu.addSeparator()    
        menu.exec_(self.playlist_comB.mapToGlobal(pos))
        
    def removePlaylistDia(self):
        pList = self.playlist_comB.currentText()
        
        mBox = QMessageBox()
        val = mBox.question(self, "You sure?", "Are you sure you want to delete {0}?".format(pList), buttons= mBox.Ok|mBox.Cancel)
        if val == mBox.Ok:
            pListFile = os.path.join(os.path.dirname(__file__), str(pList)+".plt")
            os.remove(pListFile)
            self.fillPlaylist()
            
            self.statusBar().showMessage(pList + " has been removed.", 4000)

    def addPlaylistFromDia(self, window):
        print "addPlaylistFromDia"
        newPlaylist = window.addPL_le.text()
        if newPlaylist:
            self.newPlaylist(newPlaylist)
            self.fillPlaylist()
            index = self.playlist_comB.findText(newPlaylist)
            if index != -1:
                self.playlist_comB.setCurrentIndex(index)
                self.statusBar().showMessage(newPlaylist + " has been added.", 4000)
        window.close()
        
    def renamePlaylistFromDia(self, current, currentrow, window):
        print "renamePlaylistFromDia"
        newPlaylist = window.addPL_le.text()
        if newPlaylist:
            currentPLT = os.path.join(os.path.dirname(__file__), str(current)+".plt")
            newPLT = os.path.join(os.path.dirname(__file__), str(newPlaylist)+".plt")
            os.rename(currentPLT, newPLT)
            self.playlist_comB.setItemText(currentrow, newPlaylist)
        window.close()
        self.statusBar().showMessage(current + " has been renamed to " + newPlaylist + ".", 10000)

    def renamePlaylistDia(self):
        pList = self.playlist_comB.currentText()
        pRow = self.playlist_comB.currentIndex()
        self.renamePlayDia = DialogBox(self, "Rename Playlist", "Rename {0} to:".format(pList))
        self.renamePlayDia.okBtn.clicked.connect(lambda : self.renamePlaylistFromDia(pList, pRow, self.renamePlayDia))
        self.renamePlayDia.ShowDiag()

    def addPlaylistDia(self):
        self.addPlayDia = DialogBox(self, "Add Playlist", "Name for your new Playlist?")
        self.addPlayDia.okBtn.clicked.connect(lambda : self.addPlaylistFromDia(self.addPlayDia))
        self.addPlayDia.ShowDiag()

class DialogBox(QDialog):
    def __init__(self, parent, windowTitle, lblDesc):
        super(DialogBox, self).__init__(parent)
        self.setWindowTitle(windowTitle)
        self.resize(500, 281)

        self.mainBox = QVBoxLayout(self)
        self.spacer1 = QSpacerItem(20, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
        self.label = QLabel(lblDesc)
        self.spacer2 = QSpacerItem(20, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.labelLayout = QHBoxLayout()
        self.labelLayout.addItem(self.spacer1)
        self.labelLayout.addWidget(self.label)
        self.labelLayout.addItem(self.spacer2)
        self.mainBox.addLayout(self.labelLayout)
        
        self.addPL_le = QLineEdit()
        self.mainBox.addWidget(self.addPL_le)
        self.buttonBox = QHBoxLayout()
        
        self.okBtn = QPushButton("Ok", self)
        self.buttonBox.addWidget(self.okBtn)

        self.cancelBtn = QPushButton("Cancel", self)
        self.cancelBtn.clicked.connect(self.reject)
        self.buttonBox.addWidget(self.cancelBtn)
        self.mainBox.addLayout(self.buttonBox)

    def ShowDiag(self):
        self.resize(300,20)
        self.exec_()
 
def run():
    app = QApplication(sys.argv)
    gui = MusicListWin()
    gui.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    run()
