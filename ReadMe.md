# HyEnD-OS for easy interfacing with STM32

## How To Use:
0. Setup
	1. Clone this Project into a well known folder (Like HyEnD_OS), relative to your eclipse workspace.
	2. Generate your eclipse-project with CubeMX with Clean Project Generation, as described below. Open it. Proceed in eclipse:
	
1. Add the HyEnD_OS Lib Folder to your Project
	1. Rightclick your Project and go to Properties
		- Sources 
			1. Navigate to "C/C++ General - Paths and Symbols" and click the "Source Location" Tab
			2. Click "Link Folder..." and tick the "Link to folder in the file system" box
			3. Hit the "Variables..." button and select the "WORKSPACE_LOC" for your Workspace Path, hit "OK"
			4. Now, after the "WORKSPACE_LOC", add the relative path to the Lib folder of this project. It could look like this: WORKSPACE_LOC/HyEnD_OS
			5. Hit "OK"
		- Includes
			1. Click the "Includes" Tab
			2. Make sure that on the left under "Languages" the "GNU C" is selected
			3. Hit "Add...", then "Workspace..."
			4. Unfold your Project Folder. Select your HyEnD_OS LibFolder on the second-highest level; hit "OK", "OK" 
	2. Hit "Apply and Close"; rebuild your project (If you get an error inside /connectivity/sd/ you might need to select Middleware -> FATFS -> SD CARD in 	your .ioc file first)
	
2. Confirm if HyEnD_OS Folder is added
	1. Navigate to "Resource - Linked Resources" and click the "Linked Resources" Tab
	2. You should find your linked Folder here. You may change it here, if needed
	
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
