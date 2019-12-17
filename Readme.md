# Converting and Calibrating Distributed-Temperature-Sensing XML Data
## General Info
I wrote this program for my bachelor thesis. It reads in distributed-temperature-sensing data from XML files, optionally calibrates and then writes them to a NetCDF file. User input is read from a config file with a self-written parser (At the time I was not as aware of the possibilities to include functionality from other people's libraries :)).

This software was created to achieve the following goals:
+ Consolidate the (usually) big amount of XML files produced by one experiment into one NetCDF file from which the data can be read faster.
+ Do some calibration on the fly.
+ Use C++ as the programming language in order to create a program that executes quickly.

## For Developers
There is a [class diagram](class_diagram.png). Also, I used the codeblocks IDE (version 16.01) so there are some IDE files, but the paths to the used libraries aren't correct anymore. Also, the used libraries are:
+ boost 1.66 (mainly for file system usage)
+ rapidxml (for fast XML parsing)
+ NetCDF++ (for writing the NetCDF files)
+ lapack (for matrix transformations)
+ UnitTest++ (for the unit tests) (The testing framework resides [here](/TestProjects/unittest-cpp-master)

Boost is not included in the /lib folder because the whole thing takes up several gigabytes. 

The unit tests in the [test projects folder](/TestProjects) contain wrong file paths to the code that is to be tested.
