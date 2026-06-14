Files Format Description
========================

1. Steps File (adv-world.steps)
-------------------------------
This file records user inputs to allow replay.
- First Line: Random Seed (long integer) - ensures deterministic behavior for random events (riddles).
- Subsequent Lines: 
  game loop iteration ASCII code
  Example: "150 100" means at game loop iteration 150, the key with ASCII code 100 ('d') was pressed.

2. Result File (adv-world.result)
---------------------------------
This file records significant game events for validation (Silent Mode).
Format: "Iteration: Event Description"

Tracked Events:
- Level Changes: When a player moves to a new room.
- Health Changes: When a player loses/gains health or dies.
- Riddles: Includes the riddle index, the user's answer, and whether it was Correct/Wrong.
- Game End: Final scores for both players.

Example:
500: Player 1 moved to next level 2
750: Player 2 lost health
1200: Riddle: 1 Answer: 5 Result: Correct

3. screens files (adv-world_xxx.screen)
---------------------------------

The file starts with the map and the letter 'L', which reserves the top 5 lines for the level legend - you can change it.
The lines below represent the actual game map using specific characters (e.g., 'w' for walls).

Metadata Section ([DATA]): This section follows the map and configures dynamic game objects:
PLAYER1 / PLAYER2 [X] [Y]: Defines the starting coordinates for each player.
DOOR [ID] [Keys] [Switches] ...: Configures door mechanics, including lock requirements and connectivity.
RIDDLE [X] [Y]: Binds a riddle event to specific coordinates on the map, linking the visual symbol to the logic engine.

4. riddle files (riddles.txt)
---------------------------------

The file starts with the map in this map user can write the riddle 

Logic Section (#LOGIC_DATA): Defines the game mechanics for the specific riddle:

ANSWER: The correct numeric solution.

X / Y / ROOM: The exact coordinates and level index where the riddle is located.

INDEX: Internal identifier used by the code to manage the riddle state.

you can add as many as you want

