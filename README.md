LiteEngine
==========

Lite 3D Game Engine

Project Health
=================

| Service | System | Compiler | Status |
| ------- | ------ | -------- | ------ |
| [Travis CI](https://travis-ci.org/maritim/LiteEngine)| Linux 64 bits  | GCC 4.8.4 | [![Travis CI](https://travis-ci.org/maritim/LiteEngine.svg?branch=master)](https://travis-ci.org/maritim/LiteEngine)
| [AppVeyor](https://ci.appveyor.com/project/maritim/liteengine)| Windows 64 | Visual Studio 2017 | [![AppVeyor](https://ci.appveyor.com/api/projects/status/s0fqli66756555gt/branch/master?svg=true)](https://ci.appveyor.com/project/maritim/liteengine/branch/master)

Gallery
=================

Global Illumination
![Voxel Cone Tracing](https://github.com/maritim/LiteEngine/blob/gh-pages/LiteEngine%20Screenshot%209.png)
Animation System
![Animation System](https://github.com/maritim/LiteEngine/blob/gh-pages/LiteEngine%20Screenshot%2010.png)

Debian Install Guide
=================

Get the project from GitHub
--------------------

* Install git

        sudo apt-get install -y git

* Create a local directory for the project

        mkdir LiteEngine && cd LiteEngine
    
* Clone the project

        git clone https://github.com/maritim/LiteEngine

Install and configure dependecies
--------------------

* Install dependecies

        sudo chmod +x setup.sh
        sudo ./setup.sh
    
Build
-----

* Compile the project

        mkdir Build && cd Build
        cmake -DCMAKE_BUILD_TYPE=Release .. && make && cd ..
        
* Run the application using a prototype scene

        ./Build/LiteEngine --startscene Assets/Scenes/Prototype.scene 
