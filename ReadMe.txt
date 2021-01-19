// ----------------------------------------------------------------------------------------------------------------------------------------------------
HyEnD-OS for easy interfacing with STM32.

How To Use:
0. Generated your eclipse-project with CubeMX and open it. Proceed in eclipse:
1. Rightclick your Project and go to Properties
2. Navigate to "C/C++ General - Paths and Symbols"
3. Add Library:
	- Includes
		1. Navigate to "Includes - GNU C"
		2. Click "Add..." and add this Lib-Folder
	- Sources
		1. Navigate to "Source Location"
		2. Click "Link Filder..." and add this Lib-Folder
4. Rebuild your Project to refresh Includes and Sources
5. #include <OSUserInclude.h> in your main and whererver else needed

Note:
Currently support for STM32F1xx and STM32F4xx.


// ----------------------------------------------------------------------------------------------------------------------------
Usefull stuff around Code Generation with CubeMX

- Clean Project Generation with CubeMX (!)
	- Before generating every code you should (for clean code-generation) follow this easy trick
		1. In Cube MX under "Project Manager" navigate on the left to "Code Generator"
		2. Ensure that under "Generated Files" the box "Generate peripheral initialization as a pair of ".c/.h" files per peripheral" is ticked
		This will split your Project into more files and drasticly improoves project organization

- UserCode
	- After every re-generation of code by CubeMX your UserCode can be deleted. But your Code will be kept if it is between the comments:
	/* USER CODE BEGIN someSpaceNr # */
	// All Code and Comments in here will be kept.
	/* USER CODE END someSpaceNr # */
	This comments are distributed unintuitively, sometimes. Keep your eyes open. Especially for User-Includes and in the main-while(true)-loop.
	

- NVIC Pitfalls
	- If you want to use an Interrupt cause another interrupt (e.g. BlueButton Interrupt to Send CAN Message and receive Message via CAN RX interrupt),
	make sure that the priority of the initial causing (BlueButton) interrupt is higher or same than the caused (CAN RX) interrupts.
	Otherwise the CAN RX interrupt will interrupt the sending process caused by the BlueButton interrupt (for instance).
