Talking Atlas / SFRoamer

This is the read me for Talking Atlas.  A basic atlas application for the visually impaired to let the user explore a map of the world.
This repository holds the source code to compile the application.  It is written in C++ using Visual Studio 2022 and is an MFC application.
To compile it you need the following.

Tolk library.  Download this from:
https://github.com/dkager/tolk
You need the C++ source files, the lib file and the dll.

sqlite3.  Web site:
https://www.sqlite.org/download.html
You need the source files, lib and dll.

Spatialite.
https://www.gaia-gis.it/fossil/libspatialite/index
using the link "download libspatialite-5.1.0.zip (zipfile, .zip)".
Spatialite may depend on GDAL.  One thing for sure is the GDAL file proj.db needs to go into the same folder as the SFRoamer executable.
You need to set the include path and library path in Visual Studio for tolk and sqlite3.

Put the dll for each in your environment path or folder containing the executable.



Database files for OSMLand from Dropbox:
https://www.dropbox.com/scl/fi/ztpw9aldrhj3r98h6udy0/OSMLand.db?rlkey=r106bbh452gj5chozc9kv9fe1&dl=0
This is just under 1Gb.

And water polygons database:
https://www.dropbox.com/scl/fi/8hmayg9vj1ulkl51btpkq/wp.db?rlkey=o11wrk28dwqbty10abdnwdm5r&dl=0
this is about 1.7Gb.

The OSMLand.db goes in the Files folder. 
Create a folder:
water-polygons-split-4326
inside Files and place the wp.db file in here.

Then try compiling and running.
