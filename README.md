# Survival Simulator
Topic/Problem: Survival Simulator Text Game
<br>Expected Input: Keyboard input to terminal for action choice.
<br>Expected Output: Standard output to console showing description of events, as well as attribute values, action options, and game menu.

### Features
- 4 Attributes → Energy, Hunger, Thirst, Health
- Resources → Different types of food and water
- Inventory → For Food and Water

### Action Details
→ Scavenging and sleeping takes a certain number of time (random within a range, using stdlib.h and time.h for Random Number Generator)

→ Actions have a consequence (Energy, Hunger, Thirst, Health):
- Scavenge (Action) → Consequence: A chance to get resources (food and water) but also a chance to encounter dangerous animal
- Sleep (Action) → Consequence: Gain Energy
- Eat (Action) → Consequence: Decrease Hunger, Gain Little Energy
- Drink (Action) → Consequence: Decrease Thirst

→ Actions when Encountering Dangerous Animal:
- Fight → Chance to decrease health
- Run → Decrease energy

### Rules
- If you run out of energy, disallow scavenging. As the number of days increases, higher chances of encountering dangerous animals.
- Hunger/Thirst is maxed, health decreases every action (fixed decrease value).
- Lose condition: Health goes to 0.
- Goal condition: Survive as long as possible (Score Recorded: Number of days)

### File Reading and Writing
File Read:
- Implementation of an operation similar to the SQL operation of SELECT to retrieve data.

File Write:
- Implementation of operations similar to the SQL operations of UPDATE, INSERT and DELETE to modify the data.

### Parsers
For the gameplay, custom parsers were made to validate input for various cases. It checks if the user input is within the range of hardcoded values for each individual parser. The following is the list of parsers used during the gameplay:
- AnimalEncounterParser:
   - When the player encounters an animal and has to choose between fighting and running.  
- InputParser:
   - Most common input, deciding the next action for that specific day
- InventoryParser:
   - When player decides to use something from the inventory
- ItemConfirmationParser:
   - A confirmation for which item to use from the inventory

For the creation of accounts, the parsers used are as the following:
- The username parser, defined by the function checkSignUpUsername(), is used to validate and ensure that the username meets the required constraints before being allowed to be used as the username of the new account.
- The password parser, defined by the function checkSignUpPassword(), is used to validate and ensure that the password meets the required constraints before being allowed to be used for the new account. 

For the reading and writing of files, a parser is also used to validate the SQL statements that were given to read or write to files.


### SETUP INSTRUCTIONS
1. Clone the project
2. Run "make main" on command line
3. Run  
   Windows: SurvivalSim.exe  
   POSIX: SurvivalSim.o  
Note: If you encounter any problems, make sure to run "make clean" before "main main" again
