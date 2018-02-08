
A compiled Jar for the current version of the project, along with the source files. Currently it only generates a GUI, and either displays recieved information, or when an action occurs (such as clicking a button, using a check box, ect) it sends a message via socket communication to the C++ portion of the program. 

You need to have the C++ could already running (as this immediately tries to connect to the socket, which is currently set to 4998)

To compile a new version for yourself, the easiest way using command line would be (assuming windows)

1. Ensure you have the java SDK and make sure it is in your system evironments (easily found online)
2. Navigate to the directory that contains the folder stargui
3. In command prompt in this directory, use the command javac .\stargui\*    (this compiles all java files)

To run the compiled code simply use the command java stargui.StarGUI

A note on my design, current the main function just creates a thread that runs the GUI, which is not really needed (verus just creating an object). However this allows for additional code to added with minimal effort, as the GUI is entirely self-contained.
