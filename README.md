## ******************* Escape The Maze Game *******************

I chose to use "C" programming skills and Raylib libraries to integrate visuals on MacOS to create a "Maze Escape Game" in the terminal for my Praxis Programming Game Project.

### ******************* Instructions Before Play *******************

The Raylib graphics library was used to create this game with the CLion IDE for MacOS. 
The game must be run using a configured external tool within CLion because Raylib can cause integration problems when used directly in the terminal.

Because the code uses Raylib to render images, it cannot be run with a simple command-line compiler that does not support graphics. 
Therefore, with the right build settings, CLion is the optimal environment to run this project.

### ******************* HOW TO SET UP *******************

1. Install Raylib via Homebrew 
   Open terminal and run:  
   "bash"

   brew install raylib
   

2. Open the game project in CLion

3. Configure CLion to run the game using an External Tool 
   To run the game successfully:

   Go to `Tools -> External Tools -> Edit External Tools

    Create a new entry:
      Name: Build and Run Raylib
      Program: `/opt/homebrew/bin/make` (This is just an example)
      Arguments: No Arguments (Leave Empty)
      Working Directory: `$ProjectFileDir$`

4. To play the game 
   Use the CLion menu and Navigate to: 
      Tools -> External Tools -> Build and Run Raylib

If you try to run the game from the standard terminal in CLion without this setup, 
you may get Raylib-related errors such as missing libraries or linking issues.

### ******************* HOW TO PLAY *******************

1. After launching, you’ll be presented with a difficulty menu:
   Press `1` For Easy
   Press `2` For Medium
   Press `3` For Hard
   Press `Q` to Quit

2. Use the arrow keys to move the player:
   Press the Right Arrow Key to move Right  
   Press the Left Arrow Key to Move Left  
   Press the Up Arrow Key to Move Up  
   Press the Down Arrow Key to Move Down

3. Your goal is to navigate the maze and reach the green coloured exit block located in the bottom-right area.

4. Once you escape:
   The message “You Escaped!” is shown.
   Press the Enter key to return to the main menu.

5. The maze is generated randomly each time with increasing difficulty:
   Easy: Small grid with a simple path
   Medium: Slightly larger grid with a few decoys and longer paths
   Hard: Largest grid with high complexity and more false paths

