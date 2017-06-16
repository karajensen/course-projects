#====================================================================================
# Scripting and Tool Dev in Maya - Convex Hull - Kara Jensen
#====================================================================================

import maya.cmds as cmds
from maya.OpenMaya import MVector

# Edge between faces of a n-dimensional convexhull
class Edge(object):
    def __init__(self):
        self.indices = []
        self.indices.append(0)
        self.indices.append(0)           

# Triangle face of a n-dimensional convexhull
class Face(object):
    def __init__(self):
        self.alive = True
        self.index = -1
        self.distanceToOrigin = 0.0
        self.normal = MVector(0, 0, 0)
        self.indices = []
        self.edges = []
        for i in range(0, 4):
            self.indices.append(0)
            self.edges.append(Edge())
            
def PrintPoint(point):
    cmds.spaceLocator(a=True, p=(0.0, 0.0, 0.0))
    cmds.scale(0.1, 0.1, 0.1)
    cmds.move(point.x, point.y, point.z)
            
# Creates a diagnostic line
def PrintLine(start, end):
    cmds.curve(ws=True, p=[(start.x, start.y, start.z), (end.x, end.y, end.z)])

# Prints a Maya Vector
def PrintVector(vec):
    print("[" + str(vec.x) + ", " + str(vec.y) + ", " + str(vec.z) + "]")

# Get the position of the selected mesh vertex
def GetMeshVertex(selected, i):
    vtx = cmds.pointPosition((selected + ".vtx[" + str(i) + "]"), world=True)
    return MVector(vtx[0], vtx[1], vtx[2])
    
# Return the center point of the face
def GetFaceCenter(convexhull, face):
    p0 = convexhull[face.indices[0]]
    p1 = convexhull[face.indices[1]]
    p2 = convexhull[face.indices[2]]
    center = MVector(
        (p0.x + p1.x + p2.x) / 3.0,
        (p0.y + p1.y + p2.y) / 3.0,
        (p0.z + p1.z + p2.z) / 3.0)
    return center
    
# Create a new face between 3 points, correcting the winding order
def CreateFace(convexhull, faces, face, i0, i1, i2, u, v, hullCentroid, showDiag):
    faceToInsideHull = hullCentroid - convexhull[i0]
    if(((u ^ v).normal()) * faceToInsideHull < 0):
        CreateFaceInternal(convexhull, faces, face, i0, i1, i2, u, v, showDiag)                
    else:
        CreateFaceInternal(convexhull, faces, face, i1, i0, i2, v, u, showDiag)        
    
# Create a new face between 3 points
def CreateFaceInternal(convexhull, faces, face, i0, i1, i2, u, v, showDiag):    
    faces[face].alive = True
    faces[face].index = face
    faces[face].normal = (u ^ v).normal() 
    faces[face].indices[0] = i0
    faces[face].indices[1] = i1
    faces[face].indices[2] = i2
    faces[face].edges[0].indices[0] = i0
    faces[face].edges[0].indices[1] = i1
    faces[face].edges[1].indices[0] = i0
    faces[face].edges[1].indices[1] = i2
    faces[face].edges[2].indices[0] = i1
    faces[face].edges[2].indices[1] = i2
    if(showDiag == True):
        centerPt = GetFaceCenter(convexhull, faces[face])
        PrintPoint(centerPt)
        PrintLine(centerPt, centerPt + faces[face].normal)

# Creates the initial convex hull from 4 random points
def InitConvexHull(convexhull, faces, startingVtx, selected):
    for i in range(0, 4):
        faces.append(Face())
        convexhull.append(GetMeshVertex(selected, startingVtx[i]))

    pA = convexhull[0]
    pB = convexhull[1]
    pC = convexhull[2]
    pD = convexhull[3]
    AB = pB - pA
    AC = pC - pA
    AD = pD - pA
    BC = pC - pB
    BD = pD - pB
    
    #Find the centroid of the tetrahedron
    hullCentroid = MVector(
        (pA.x + pB.x + pC.x + pD.x) / 4.0,
        (pA.y + pB.y + pC.y + pD.y) / 4.0,
        (pA.z + pB.z + pC.z + pD.z) / 4.0)

    showFaceDiag = False
    CreateFace(convexhull, faces, 0, 0, 2, 3, AC, AD, hullCentroid, showFaceDiag)
    CreateFace(convexhull, faces, 1, 0, 3, 1, AD, AB, hullCentroid, showFaceDiag)
    CreateFace(convexhull, faces, 2, 0, 1, 2, AB, AC, hullCentroid, showFaceDiag)
    CreateFace(convexhull, faces, 3, 1, 3, 2, BD, BC, hullCentroid, showFaceDiag)
    return hullCentroid
    
# whether the two edges are equal
def AreEdgesEqual(edge1, edge2):
    if(edge1.indices[0] == edge2.indices[0] or edge1.indices[0] == edge2.indices[1]):
        if(edge1.indices[1] == edge2.indices[0] or edge1.indices[1] == edge2.indices[1]):
            return True
    return False
    
# Determines if the given edge exists among the comparison faces
def IsSharedEdge(index, edge, faces, visibleFaces):
    for faceindex in visibleFaces:
        face = faces[faceindex]
        if(face.index == index):
            continue
        if(face.alive == False):
            continue
        for connectedEdge in face.edges:
            if(AreEdgesEqual(edge, connectedEdge) == True):
                return True
    return False
    
# Fills the given container with any edges from the face that are border edges
def FindBorderEdges(face, faces, visibleFaces, edges):
    borderCounter = 0
    for edge in face.edges:
        if(IsSharedEdge(face.index, edge, faces, visibleFaces) == False):
            borderCounter += 1
            edges.append(edge)
            # Can have a maximum of 2 border edges per face
            if(borderCounter >= 2):
                return
    
# Create a visible mesh hull from the given faces
def CreateVisibleMesh(convexhull, faces, meshName):
    convexhullFaces = []
    for f in faces:
        if(f.alive == True):
            vtxA = convexhull[f.indices[0]]
            vtxB = convexhull[f.indices[1]]
            vtxC = convexhull[f.indices[2]]               
            convexhullFaceLst = cmds.polyCreateFacet(p=[
                (vtxA.x, vtxA.y, vtxA.z), 
                (vtxB.x, vtxB.y, vtxB.z), 
                (vtxC.x, vtxC.y, vtxC.z)])
            convexhullFaces.append(convexhullFaceLst[0])
            
    # Combine the faces into a mesh
    outputLst = cmds.polyUnite(convexhullFaces, ch=False, mergeUVSets=True, name=meshName)
    cmds.polyMergeVertex(d=False, am=True, ch=False)
    return outputLst[0]
    
# Finds 4 vertices in the mesh that are not all coplanar
def GetStartingVertices(selected, vertNumber):
    p0 = GetMeshVertex(selected, 0)
    p1 = GetMeshVertex(selected, 1)
    p2 = GetMeshVertex(selected, 2)     
    u = p1 - p0
    v = p2 - p0   

    # u.(v X w) = 0 then coplanar    
    for i in range(3, vertNumber):
        w = GetMeshVertex(selected, i) - p0
        if((u * (v ^ w)) != 0.0):
            return [0, 1, 2, i]
    return []
  
# Incremental Convex hull algorithm removes all faces 
# that are facing the new point and generates new faces 
# on the border of the highlighted ones. Reference:
# http://www.eecs.tufts.edu/~mhorn01/comp163/algorithm.html
def MakeConvexHull():
    selectedLst = cmds.ls(sl=True)
    if len(selectedLst) != 1:
        cmds.warning("Can only be used on one object")
        return
    
    #Ensure no duplicated vertices in the selected mesh
    duplicatedLst = cmds.duplicate(selectedLst[0])
    selected = duplicatedLst[0]
    cmds.select(selected)
    cmds.polyMergeVertex(d=False, am=True, ch=False)
    cmds.select(selected)
    vertNumber = len(cmds.ls((selected+".vtx[*]"),fl=1))
    if(vertNumber < 4):
        cmds.warning("Must have 4 or more vertices")
        return 
        
    # Change these for diagnostics
    keepHullSteps = False
    printVisibleFaces = False
    printBorderEdges = False
        
    # Create the initial convex hull from 4 non-coplanar vertices   
    startingVtx = GetStartingVertices(selected, vertNumber)
    if(len(startingVtx) != 4):
        cms.warning("Must have 4 vertices not coplanar")
        return

    convexhull = []
    faces = []    
    hullCentroid = InitConvexHull(convexhull, faces, startingVtx, selected)
    if(keepHullSteps == True):
        CreateVisibleMesh(convexhull, faces, selected + "_init")
        PrintPoint(hullCentroid)

    # For every remaining vertex in the mesh
    for i in range(0, vertNumber):
        if(i == startingVtx[0] or i == startingVtx[1] or i == startingVtx[2] or i == startingVtx[3]):
            continue        

        point = GetMeshVertex(selected, i)  
        edges = []
        pointIndex = len(convexhull)
        convexhull.append(point)

        # Determine faces that the point is in front of
        visibleFaces = []
        for face in faces:
            if(face.alive == True):
               faceToPoint = point - convexhull[face.indices[0]]
               if((face.normal * faceToPoint) > 0.0):
                   visibleFaces.append(face.index)
        if(len(visibleFaces) == 0):
            continue
        if(printVisibleFaces == True):
            for faceindex in visibleFaces:
                PrintPoint(convexhull[faces[faceindex].indices[0]])
        
        # Find all border edges from the visible faces
        if(len(visibleFaces) == 1):
            # For a single face, all edges are on the border
            for edge in faces[visibleFaces[0]].edges:
                edges.append(edge)
        else:
            # For multiple faces, determine the border edges
            for faceindex in visibleFaces:
                FindBorderEdges(faces[faceindex], faces, visibleFaces, edges)

        if(printBorderEdges == True):
            for edge in edges:
                PrintLine(convexhull[edge.indices[0]],convexhull[edge.indices[1]])
        assert(len(edges) > 0)
        
        # Mark all visible faces as dead
        for face in visibleFaces:
            faces[face].alive = False
        
        # Connect up new faces from the edges to the point
        for edge in edges:
            faceIndex = len(faces)
            faces.append(Face())          
            u = convexhull[edge.indices[0]] - point
            v = convexhull[edge.indices[1]] - point
            i0 = pointIndex
            i1 = edge.indices[0]
            i2 = edge.indices[1]
            CreateFace(convexhull, faces, faceIndex, i0, i1, i2, u, v, hullCentroid, False)
                
        if(keepHullSteps == True):
            CreateVisibleMesh(convexhull, faces, selected + "_step")
        
    # Create the face meshes and cleanup
    cmds.delete(selected)
    CreateVisibleMesh(convexhull, faces, selected + "_hull")
    
# Testing the script
cmds.polyCube()
MakeConvexHull()
cmds.polyTorus(sx=6, sy=6)
MakeConvexHull()
cmds.polyHelix(sco=10)
MakeConvexHull()