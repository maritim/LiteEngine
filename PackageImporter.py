import os
with open('PackageImporterPath.txt') as f:
    package = f.readline()
os.system("cd assets")# Go to assets folder
os.system("git clone --recursive " + package) # Clone the package in the assets folder