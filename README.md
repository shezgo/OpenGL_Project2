\# OpenGL\_Project2



This repository contains the CSC630 Project 2 codebase, including C++ source files, graphics assets (`.obj`, `.mtl`, `.png`, `.jpg`), and essential `.lib` files. Git LFS is used to store large files efficiently.



---



\## \*\*Prerequisites\*\*



\- \*\*Git\*\*: Make sure Git is installed on your system.

\- \*\*Git LFS\*\*: Required to download large files tracked by LFS.

\- \*\*C++ Compiler / Visual Studio\*\*: To build the project.

\- \*\*Additional dependencies\*\* listed in document below.



---



1\. Install Git LFS (if not already installed) and clone repo:



&nbsp;  bash commands:



&nbsp;  git lfs install

&nbsp;  git clone https://github.com/shezgo/OpenGL\_Project2.git



&nbsp;  - cd to OpenGL\_Project2 folder.



2\. Open in Visual Studio 2022



3\. Set up dependencies/libraries:

* Select the project in VS solution explorer tab on the right
* select project drop down at top of the window
* select properties
* select VC++ Directories in the left window, edit include directories to include the Libraries/include folder and the headers folder
* within VC++ Directories, next edit Library directories to include Libraries/lib folder.
* Select Linker in the left window, then select Input from the resulting dropdown options. Edit Additional Dependencies to include:, on separate lines:

glfw3.lib

opengl32.lib

* Select OK, then select apply.



4\. Build and run project - a window with the graphics should pop up.



