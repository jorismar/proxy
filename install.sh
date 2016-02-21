#!/bin/bash

#------------------------------------------ LINKS ------------------------------------------#

git_ffmpeg="git://source.ffmpeg.org/ffmpeg.git"
git_gpac="https://github.com/gpac/gpac.git"

#---------------------------------- GETTING DEPENDENCIES -----------------------------------#

sudo apt-get install git libx264-dev yasm libbz2-dev make pkg-config g++ zlib1g-dev firefox-dev libfreetype6-dev libjpeg62-dev libpng12-dev libopenjpeg-dev libmad0-dev libfaad-dev libogg-dev libvorbis-dev libtheora-dev liba52-0.7.4-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavresample-dev libxv-dev x11proto-video-dev libgl1-mesa-dev x11proto-gl-dev linux-sound-base libxvidcore-dev libssl-dev libjack-dev libasound2-dev libpulse-dev libsdl1.2-dev dvb-apps libav-tools libavdevice-dev libmozjs185-dev nodejs  doxygen &&\

git clone $git_ffmpeg   dependencies/ffmpeg  &&\
git clone $git_gpac     dependencies/gpac    &&\

#-------------------------------- INSTALLING DEPENDENCIES ----------------------------------#

#FFmpeg

cd dependencies/ffmpeg  &&\
./configure --enable-shared --disable-debug --enable-libx264 --enable-avresample --enable-gpl   &&\
make -j &&\
sudo make install &&\

#GPAC Lib and Applications

cd ../gpac &&\
./configure &&\
make &&\
sudo make install &&\
sudo make install-lib &&\

#-------------------------------- INSTALLING PROXY SERVER ----------------------------------#

cd ../../modules &&\
sudo g++ ./src/*.cpp -I ./include/ -std=c++11 -pthread -o /usr/local/bin/DashServer &&\
sudo cp -avr engine/ /usr/local/etc/engine &&\
cd .. &&\
doxygen doxygen &&\

echo "\033[1mDone. \033[0;0;0m" ||\

echo "\033[1mERRO! \033[0;0;0m"




