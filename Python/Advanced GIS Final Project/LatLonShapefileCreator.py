## Alex Solorio

# This python file acts as a user-friendly way to create shapefiles
# from text/csv files containing latitude/longitude data for points

import arcpy
from arcpy import env
import os.path
env.workspace = "Z:\GEOG3130\FinalProject"
env.overwriteOutput = True

""" Function To Be Used For Shapefile Creation """

def addRowToShapefile(inputFilename, outputShapefileName, dataFormat, delimiter, geometryType):


    """ Create Indices of Latitude and Longitude """#

    if dataFormat == "Latitude,Longitude":
        latIndex = 0
        lonIndex = 1
    elif dataFormat == "Longitude,Latitude":
        latIndex = 1
        lonIndex = 0


    """ Read Through File And Create Array Of Points """

    infile = open(inputFilename, "r")
    lines = infile.readlines()
    points = arcpy.Array()
    for line in lines:
        data = line.split(delimiter)
        lat = float(data[latIndex])
        lon = float(data[lonIndex])
        pt = arcpy.Point(lon, lat)  # create the point
        points.append(pt)
    infile.close()


    """ Insert Data Into Shapefile Using Cursor """

    if geometryType == "POINT":
        cursor = arcpy.da.InsertCursor(outputShapefileName, ["SHAPE@"])
        for point in points:
            cursor.insertRow([point])
        del cursor
    elif geometryType == "POLYLINE":
        pline = arcpy.Polyline(points, sr)
        cursor = arcpy.da.InsertCursor(outputShapefileName, ["SHAPE@"])
        cursor.insertRow([pline])
        del cursor
    elif geometryType == "POLYGON":
        pgon = arcpy.Polygon(points, sr)
        cursor = arcpy.da.InsertCursor(outputShapefileName, ["SHAPE@"])
        cursor.insertRow([pgon])
        del cursor


""" Function Defined Above """


""" Read In Parameter Values For Computation """

inputFilename = arcpy.GetParameterAsText(0)
if len(inputFilename) == 0:     # If first argument left blank (defined a folder, not a file)
    inputFilename = arcpy.GetParameterAsText(1)
outputShapefileName = arcpy.GetParameterAsText(2)
geometryType = arcpy.GetParameterAsText(3)
dataFormat = arcpy.GetParameterAsText(4)
delimiter = arcpy.GetParameterAsText(5)
sr = arcpy.GetParameterAsText(6)
outpath = env.workspace


""" Determine If Handling Multiple Files Or Not """
multipleFiles = False
if os.path.isdir(inputFilename):
    multipleFiles = True


""" Create The Empty Shapefile """

arcpy.CreateFeatureclass_management(outpath, outputShapefileName, geometryType, "", "", "", sr)
print arcpy.GetMessages()


""" Process The Files """

if multipleFiles == False:      # Just one file to process
    addRowToShapefile(inputFilename, outputShapefileName, dataFormat, delimiter, geometryType)

else:                            # Process all files within the folder
    for file in os.listdir(inputFilename):
        file = inputFilename + "\\" + file
        addRowToShapefile(file, outputShapefileName, dataFormat, delimiter, geometryType)
