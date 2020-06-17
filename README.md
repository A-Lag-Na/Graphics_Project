Camera Controls:
WASD for horizontal, forwards/backwards movement.
QE for vertical movement.
Press R/4 to toggle directional light off/on.
T to move directional light towards +X.
Arrow Keys and Right Control, Right Shift to move Point Light (Represented by cube).
IJKL for spotlight X and Z axis postitional movement. UO for Y axis movement.
8456 for spotlight X and Z axis directional movement. 79 for Y axis movement.
MN to turn on/off reflective sphere's wave vertex shader.
FG to disable/renable point light.

Left shift to enable split-screen. Left Control to disable.

Grid is hard-coded. Corvette is rendered from file using .obj loading.


# GTemplates

Convenient template projects for developers using Gateware.

To compile a template on your platform:

	download CMake: https://cmake.org/download/

	Open the CMake GUI (or use cmd line if preferred)

	Point it at the specific template's folder and
	output to a build folder of your choice (ex: ./build)

	Click "Configure" and select your preferred environment then
	hit "Generate" and check your output folder for your solution

The IDEs we typically test with:
	Win32: Visual Studio
	MacOS: XCode
	Linux: CodeLite

Certain templates require additional dependencies, so be sure to check
the included README.md in each specific template to make sure you have them.
A good rule of thumb is to just read the "Gateware/README.md" to get setup.
