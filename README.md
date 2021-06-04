# 3D Motion Capture Using Raspberry Pi


![system](https://user-images.githubusercontent.com/38900338/104141795-daf02500-53fb-11eb-849d-937f17aed7b5.JPG)
* openCV, openGL powered by Visual Studio on Windows.
* Same network workstation between PC and Raspberry Pis using portable home router.

<br>

## Result
* Visualize the movement of a person's upper body in 3D characters
![result](https://user-images.githubusercontent.com/38900338/104141833-06730f80-53fc-11eb-9776-280511755f78.JPG)

<br>

## Installation
1. Clone this repository RPIs and PC (need at least two Raspberry Pi Board)
```
git clone https://github.com/HYEEWON/3D_Motion_Capture_2018.git
```

2. Raspberry Pi setting


2-1. Camera calibration: Need a 9x5 grid like a chessboard.

2-2. Execute PiMain() 

```
cd ./3D_Motion_Capture_2018/RaspberryPiCalibration/
make // do camera calibration first.

cd ..
cd ./RaspberryPiMain/

make // execute main both of RPIs
```

3. PC setting

Use Visual Studio 2017 (or later is also fine.) on Windows

```
// opencv version
4.x.x > opencv version => 3.x.x

/* opengl version doesn't matter
* But, let us know if there is a problem.
*/
```

