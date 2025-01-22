# P1 Execution
## How to Compile:    
    Execute ./Server to run server first.
    Note: If you want to recompile the server, first run the following commands:
        rm -f Server
        g++ -o Server Server.cpp -lzmq

    Navigate to ./MainClient/build and run ./game.
    Note: If you want to recompile the engine, first run the following commands:
        make clean
        make game

## How to Play: 
    1. WASD to navigate. (W=Jump)
    2. Pressing R while game is running will start replay recording.
    3. Pressing P after pressing R will end recording and play the replay.
    4. Pressing the BACKSPACE key repeatedly enables us to toggle between three modes for time - normal (tic size = 50ms), fast (tic size = 10ms) and slow (tic size = 100ms)
    5. Pressing the LSHIFT+A or LSHIFT+D chords enable dashes in either direction. The Resize event is not raised.
    6. Pressing the SPACEBAR allows us to toggle pause.
    7. Pressing the LSHIFT key enables fixed size mode. If the resize button is pressed without pressing LSHIFT first, the screen is resized in the default mode, with objects being stretched to fit the screen. If LSHIFT is pressed, whenever the screen is resized, objects retain their pixel size and location.
    8. Pressing ESC exits the game as though the user would have pressed the cross icon. This was implemented as a convenience for testing.

## Notes:
    1. Colliding with a death zone will initiate the respawn event triggered by the script.

# P2 Execution 
## How to Compile:    
    Execute ./Server to run server first.
    Note: If you want to recompile the server, first run the following commands:
        rm -f Server
        g++ -o Server Server.cpp -lzmq

    Navigate to ./MainClient/build and run ./game.
    Note: If you want to recompile the engine, first run the following commands:
        make clean
        make game

## How to Play: 
    1. W to jump and A/D to move Left/Right (Classic Flappy Bird Controls)
    2. Pressing R while game is running will start replay recording.
    3. Pressing P after pressing R will end recording and play the replay.
    4. Pressing the BACKSPACE key repeatedly enables us to toggle between three modes for time - normal (tic size = 50ms), fast (tic size = 10ms) and slow (tic size = 100ms)
    5. Pressing the SPACEBAR allows us to toggle pause.
    6. Pressing the LSHIFT key enables fixed size mode. If the resize button is pressed without pressing LSHIFT first, the screen is resized in the default mode, with objects being stretched to fit the screen. If LSHIFT is pressed, whenever the screen is resized, objects retain their pixel size and location.
    7. Pressing ESC exits the game as though the user would have pressed the cross icon. This was implemented as a convenience for testing.

## Note:
    1. Colliding with the bottom of the screen will reset the bird's position and flash "FAILURE" on the screen.

# P3 Execution
## How to Compile:    
    Execute ./Server to run server first.
    Note: If you want to recompile the server, first run the following commands:
        rm -f Server
        g++ -o Server Server.cpp -lzmq

    Navigate to ./MainClient/build and run ./game.
    Note: If you want to recompile the engine, first run the following commands:
        make clean
        make game

## How to Play: 
    1. A/D to move gun left/right. Left Mouse Button to shoot.
    2. Pressing R while game is running will start replay recording.
    3. Pressing P after pressing R will end recording and play the replay.
    4. Pressing the BACKSPACE key repeatedly enables us to toggle between three modes for time - normal (tic size = 50ms), fast (tic size = 10ms) and slow (tic size = 100ms)
    5. Pressing the SPACEBAR allows us to toggle pause.
    6. Pressing the LSHIFT key enables fixed size mode. If the resize button is pressed without pressing LSHIFT first, the screen is resized in the default mode, with objects being stretched to fit the screen. If LSHIFT is pressed, whenever the screen is resized, objects retain their pixel size and location.
    7. Pressing ESC exits the game as though the user would have pressed the cross icon. This was implemented as a convenience for testing.

## Notes:
    1. Shooting all the aliens will result in victory. This may take some time to trigger even though all aliens are dead. Just keep firing! This is because I haven't quite tuned the fire rate yet and this results in multiple deaths per update cycle which takes a while to process. The victory screen does show up eventually.
    2. If any alien touches the bottom of the screen, "FAILURE" will pop up and the blocks will reset.
