RiskGame

How to set up the project for own use.

Add a directory "dependencies" in the same depth as the src directory.
In dependencies, add the directories "include" and "lib".
Add sfml files to include and lib, make sure that they are the 32 bit version!
(include files need to be organised in "SFML/*")

After this is done, use premake5 to build the project in visualstudio.