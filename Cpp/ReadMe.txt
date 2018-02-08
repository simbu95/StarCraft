
This is my current C++ code for this project. It has a couple library dependancies, which will be up to the user to resolve to compile. I am using the sc2api (available from here https://github.com/Blizzard/s2client-api), and the boost library, specifically asio (available from here http://www.boost.org/users/history/version_1_66_0.html)

Not much to say about the code, other then it calls the sc2 api to create a game, and uses boost to host a server for the java GUI to communicate to. Then, as the game is playing, it communicates information that can be collected from the game to the GUI, which also recieving commands to perform actions.

I personally had a lot of trouble getting the C++ project working, so below is a small guide on the things I had the most trouble with. This is all assuming Windows 10, Visual Studio 2017, using a new project, and by no means is an extensive guide.

First, you need the StarCraft libraries and the Boost Libraries. I downloaded the precompiled StarCraft libraries (can be found in the readme of the sc2api github), and I downloaded Boost. I then had to compile the boost libraries, by running the bootstrap.bat file, followed by running the generated b2.exe. On a small note, the default settings of b2 did not generate the specific .lib I needed, and so I had to run it from command prompt using b2 build-type=static.

After getting all the required files, I started a new project, and had to change the project configuration. This was located at the top of the project properties page, using a button titled configuration manager. I had to change the configuration to release, and the platform to x64. This may be different if you don't use the precompield libs.

Next I had to change the runtime libraries, which can be found in the project properties under C/C++ ; Code Generation ; Runtime Library. I had to set this to /MT. If your .lib files aren't being loaded, this probably has something to do with it (you recieve an error about either functions not being found, or if you force the inclusion of the libs (Linker; Input; Additional Dependancies), you might recieve an error about incorrect Runtime)

Finally, make sure your project has access to the files downloaded, from the project properties go to C/C++ ; General; Include Additional Directories (this is the root where your header files are searched from), and to Linker; General; Additional Library Directories (This should be the folder where your .lib are stored)

After all that, most if not all compile errors should be resolved. That being said, the api is still being updated, so some of this could change. 

Other then that, currently I am using a map that may not be easy to find, so I have included it. You can change the map to a different one, or add the map in your Starcraft files (from the installation go to StarCraft II/Maps/Ladder and add the map there.)
