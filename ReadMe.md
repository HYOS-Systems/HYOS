# HyEnD-OS for easy interfacing with STM32

## How To Use:
0. Setup
	1. Clone this Project into a well known folder, relative to your eclipse projects or to your eclipse workspace.
	2. Generate your eclipse-project with CubeMX and open it. Proceed in eclipse:
	
1. Initially add the Lib Folder to your Project
	1. Rightclick your Project and go to Properties
	2. Navigate to "C/C++ General - Paths and Symbols" and click the "Source Location" Tab
	3. Click "Link Folder..." and tick the "Link to folder in the file system" box
	4. Type a meaningful "Folder Name", like "HyEnD_Lib"
	5. Hit the "Variables..." button and select the "PROJECT_LOC" for the path to your project or "PARENT_LOC" for the path to your workspace, hit "OK"
	6. Now, after the "PROJECT_LOC" or "PARENT_LOC", add the relative path to the Lib folder of this project. It could look like this: PROJECT_LOC/../../Lib
	7. Hit "OK" and then "Apply and Close"; rebuild your project
	
2. Confirm if Lib Folder is added
	1. Rightclick your Project and go to Properties
	2. Navigate to "Resource - Linked Resources" and click the "Linked Resources" Tab
	3. You should find your linked Folder here. You may change it here, if needed
	
3. Add to Sources and Includes
	1. Rightclick your Project and go to Properties
	2. Navigate to "C/C++ General - Paths and Symbols"
	- Sources 
		1. Click the "Source Location" Tab
		2. If you did step 1 and did not remove your folder from here, you should find it here under /Project_Name/Your_Lib_Name
		3. If you deleted it or can not find it, click "Add Folder..." and select your Lib
	- Includes
		1. Click the "Includes" Tab
		2. Make sure that on the left under "Languages" the "GNU C" is selected
		3. Hit "Add...", then "Workspace..."
		4. Unfold your Project Folder. Select your Lib Folder on the second-highest level; hit "OK", "OK" 
	3. Hit "Apply and Close"; rebuild your project
	
4. `#include <OSUserInclude.h>` in your main and whererver else needed

## Note:
Currently support for STM32F1xx and STM32F4xx.


# Usefull stuff around Code Generation with CubeMX

- Clean Project Generation with CubeMX (!)
	- For clean code-generation from your CubeMX Project, follow this easy trick
		1. In Cube MX under "Project Manager" navigate on the left to "Code Generator"
		2. Ensure that under "Generated Files" the box "Generate peripheral initialization as a pair of ".c/.h" files per peripheral" is ticked
		This will split your Project into more files and drasticly improoves project organization

- UserCode
	- After every re-generation of code by CubeMX your UserCode can be deleted. But your Code will be kept if it is between the comments:
	```
	/* USER CODE BEGIN someSpaceNr # */
	// All Code and Comments in here will be kept.
	/* USER CODE END someSpaceNr # */
	```
	This comments are distributed unintuitively, sometimes. Keep your eyes open. Especially for User-Includes and in the main-`while(true)`-loop.
	

- NVIC Pitfalls
	- If you want to use an Interrupt cause another interrupt (e.g. BlueButton Interrupt to Send CAN Message and receive Message via CAN RX interrupt),
	make sure that the priority of the initial causing (BlueButton) interrupt is higher or same than the caused (CAN RX) interrupts.
	Otherwise the CAN RX interrupt will interrupt the sending process caused by the BlueButton interrupt (for instance).
	- I generally advice to not set any NVIC priority to the highest level (0) unless you really know what your are doing. As timers are also interrupt based your HAL Delay will stop counting, while an NVIC treatment with priotity 0 is executed. This implys that HAL Delay becomes an infinite loop whithin the NVIC treatment.
