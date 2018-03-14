ColViz Middelware
=====================================================

## Introduction

ColViz (Collaborative Visualization) it's a Middleware oriented to the application of collaborative visualization techniques inside the HBP Project. This Middleware is mainly centered in the analysis of huge neuroscientific data sets that are growing up day by day, increasing the difficulty of knowledge extraction. Visualization techniques could help in the analysis by taking advantage of the human visual system. These advantages could be made more powerful if the visual analysis is done using collaborative visualization techniques, allowing for several groups of neuronscientist to work collaboratively. ColViz allows the Neuroscientists the aplication of collaborative visualization techniques to ease the analysis of neuroscientific data, using different Collaborative Visualization paradigms.

See [ColViz web page](http://gmrv.es/colviz/) and
[ColViz manual](http://gmrv.es/colviz/ColVizManual.pdf)
for a complete description and sample data tests.

## Dependencies

* Required dependencies for ColViz Middelware:
    * mysqlcppconn
    * ProtoBuffers
    * RapidJSON (included in this project as submodule)
    * nett (https://devhub.vr.rwth-aachen.de/VR-Group/nett)

* Required dependencies for ColViz Middelware Clients:
    * Qt >= 5.8
    * OpenMesh
    * libQGLViewer

* The following environment variables can be used to indicate the location of the libraries:
    * NETT_ROOT
    * MYSQLCONNECTORCPP_ROOT_DIR
    * OPENMESH_ROOT

## Building

ColViz Middleware and Clients has been successfully built and used on Ubuntu 16.04/17.04/17.10 using GCC 6.X.
Please ensure that you build the Release version in order to get the best performance possible.

```bash
git clone --recursive https://github.com/gmrvvis/ColViz
mkdir ColViz/build && cd ColViz/build
cmake .. -DCLONE_SUBPROJECTS=ON
make
```
