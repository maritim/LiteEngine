LiteEngine
==========

Lite 3D Game Engine

Unix Install Guide
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

        make
        
* Run the application using a prototype scene

        ./GameEngine.out Assets/Scenes/Prototype.scene 
