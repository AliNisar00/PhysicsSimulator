# Physics Simulator
This is our HU Spring'23 OOP & DM course's project. The back-end utilizes Box2D and the front-end is handled by SDL2, both based on C++ implementation.

To build this project on your own machine using Visual Studio IDE:
1. Download Visual Studio from https://visualstudio.microsoft.com/ and select the "Desktop Development with C++" workload from the installer.
2. Clone the repository, either from Visual Studio clone window or from GitHub directly. Remember for Visual Studio, our solution name and project name are "testBox2DSDL", reminiscent of the first testing (we will try to change that if time permits us).
3. Extract the Box2D and SDL2 libraries and place the extracted folders in the root of C:\, i.e. C:\Box2D and C:\SDL2. The include files for these libraries are here: C:\Box2D\include\box2d and C:\SDL2\include. And the library (.lib) files are here: C:\Box2D\build\bin\Debug and C:\SDL2\lib\x64.
4. Now, you can run the project from Visual Studio and VS will build the project for you and run it.

If there are build errors regarding missing include or library files, you would need to set up your VS (don't worry, you only need to do this once):
1. Open project properties, either from the menu bar as project->properties or from the Solution Explorer (the file directory system of VS) by right clicking the project (not the solution) and clicking properties.
2. To add the include files, go to the C/C++ properties from the Configuration Properties panel on the left in the properties window and click it, then in the right panel click on the Additional Include Directories line, and then click the arrow on the right and then <Edit...>. The relevant window pops up, here add these: C:\Box2D\include\box2d and C:\SDL2\include. Then click Ok to close the window and Apply and Ok on the main window.
3. To add the library files, go to the same Project Properies window as for the include files, but this time go the Linker properties from the Configuration Properties panel, there select the Additional Library Directories, click the arrow and then <Edit...> to open the relavent window. Here add: C:\Box2D\build\bin\Debug and C:\SDL2\lib\x64. Click Ok and then Apply, but don't close the main window. We need to tell VS the specific .lib files to use. To do that, go to the Input option under the Linker drop-down in the Configuration Properties panel of the main window. Here, go to Additional Dependencies and add the following, each on a separate line without any commas or other delimiter:

box2d.lib
SDL2.lib
SDL2main.lib
ShLwApi.Lib
shell32.lib
SDL2_ttf.lib
SDL2_mixer.lib
SDL2_image.lib

Then click Ok in the popup, and then Apply and Ok in the main window.

4. Now, you can run the project from Visual Studio and VS will build the project for you and run it.


As always, if you encounter any problems, you can contact me. For this project specifically, those who are from my university, can contact me via email or MS Teams.
