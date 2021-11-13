#!/bin/bash

# Installing Dependencies Function

function InstallDependenciesPacman
{
	echo "Attempting to install dependencies using pacman"

	dep="gcc g++ make cmake sdl2 sdl2_image sdl_sound glew openal assimp bullet"

	for pkg in $dep; do
		if dpkg -l "$pkg" &> /dev/null; then
			echo "Package '$pkg' already installed. Do nothing."
		else
			echo "Package '$pkg' is not installed. Installing..."
			sudo pacman -Syy $pkg
		fi
	done

	echo "Dependencies installed"
}

function InstallDependenciesAptGet
{
	echo "Attempting to install dependencies using apt-get"

	dep="build-essential cmake libsdl2-image-dev libsdl-sound1.2-dev libsdl2-dev libglew-dev libopenal-dev libassimp-dev libbullet-dev"

	for pkg in $dep; do
		if dpkg -l "$pkg" &> /dev/null; then
			echo "Package '$pkg' already installed. Do nothing."
		else
			echo "Package '$pkg' is not installed. Installing..."
			sudo apt-get update
			sudo apt-get install -y $pkg
		fi
	done	

	echo "Dependencies installed"
}

function InstallDependenciesDnf
{
	echo "Attempting to install dependencies using dnf"

	dep="gcc gcc-c++ make cmake SDL2_image-devel SDL2-devel SDL_sound-devel glew-devel openal-devel assimp-devel bullet-devel"

	for pkg in $dep; do
		if dpkg -l "$pkg" &> /dev/null; then
			echo "Package '$pkg' already installed. Do nothing."
		else
			echo "Package '$pkg' is not installed. Installing..."
			dnf install -y $pkg
		fi
	done	

	echo "Dependencies installed"
}

function InstallDependenciesDarwin
{
	if hash brew 2>/dev/null; then
		echo "Package brew is not installed. Installing..."
		/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
	fi

	echo "Package brew is updating..."
	brew update

	dep_apple="cmake sdl2 sdl2_image sdl_sound glew openal assimp bullet"

	for pkg in $dep_apple; do
	    if brew list -1 | grep -q "^${pkg}\$"; then
			echo "Package '$pkg' already installed. Do nothing."
		else
			echo "Package '$pkg' is not installed. Installing..."
			brew install --yes ${pkg}
		fi
	done
}

function InstallDependenciesFreeBSD
{
	pkg install sdl2 sdl2_image sdl_sound openal-soft bullet glew assimp
}

# Main

if [[ `uname` == "Linux" ]]; then

	#search for package manager
	if type pacman 2> /dev/null; then	
		InstallDependenciesPacman
	elif type apt-get 2> /dev/null; then
		InstallDependenciesAptGet
	elif type dnf 2> /dev/null; then
		InstallDependenciesYum
	else
		echo "ERROR: Unknown distribution. You will have to install the dependencies manually."
		exit;
	fi

elif [[ `uname` == "Darwin"* ]]; then
	
	InstallDependenciesDarwin

elif [[ `uname` == "FreeBSD" ]]; then

	InstallDependenciesFreeBSD

fi
