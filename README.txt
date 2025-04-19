
// distr linux mint 21 or ubuntu

//step dependencies
sudo apt install -y libopencv-dev libglew-dev libglfw3-dev libfreeimage-dev libfreetype-dev libglm-dev libsoil-dev

//step build all

cmake .
make 


//step clean
make clean-all


// execute
./pano file.avi

key_W to rotate camera left around root scene
key_E to rotate camera right too

key_UP/key_DOWN = change distance of point view

mouse move - to rotate node with pano
