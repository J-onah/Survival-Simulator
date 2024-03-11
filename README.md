# Survival_Simulator
Topic/Problem: Survival Simulator Text Game

Expected Input: Keyboard input to terminal for action choice.

Expected Output: Standard output to console showing description of events, as well as attribute values, action options, and game menu.

### Features

4 Attributes → Energy, Hunger, Thirst, Health

Resources → Different types of food and water

Inventory → For Food and Water

Action Details:

→ Scavenging and sleeping takes a certain number of time (random within a range, using stdlib.h and time.h for Random Number Generator)

→ Actions have a consequence (Energy, Hunger, Thirst, Health):

· Scavenge (Action) → Consequence: A chance to get resources (food and water) but also a chance to encounter dangerous animal

· Sleep (Action) → Consequence: Gain Energy

· Eat (Action) → Consequence: Decrease Hunger, Gain Little Energy

· Drink (Action) → Consequence: Decrease Thirst

Actions when Encountering Dangerous Animal:

· Fight → Chance to decrease health

· Run → Decrease energy

Rules:

If you run out of energy, disallow scavenging. As the number of days increases, higher chances of encountering dangerous animals.

Hunger/Thirst is maxed, health decreases every action (fixed decrease value).

Lose condition: Health goes to 0.

Goal condition: Survive as long as possible (Score Recorded: Number of days)

File Reading and Writing:

File Read:

- Read CSV file to retrieve highest score with associated name.

- Read file that contains specific user progress if it exists, and load the values when player continues the game after exiting and returning to the program again.

File Write:

- If player score goes above highest score, get their names and write to CSV file their names with the respective highest score.

- For saving progress, generate new file for specific player name if it does not yet exist, and write out current resources and health values to file to save it.

Parser:

- Ensure only available keyboard inputs (according to choices shown) are given as program inputs.

- File writing parser:

o Ensure values under certain sections are in correct format

§ E.g. Inventory could have text, but health and energy values should only have numbers.

- File reading parser:

o Ensure file / file data section corresponds to player name as selected by current player

## INSTRUCTIONS
1. Clone the project
2. Run "make main" on command line
3. Run  
   Windows: SurvivalSim.exe  
   POSIX: SurvivalSim.o  
Note: If you encounter any problems, make sure to run "make clean" before "main main" again
