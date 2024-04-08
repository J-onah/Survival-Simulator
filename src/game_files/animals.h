#ifndef ANIMALS_H
#define ANIMALS_H

struct animal
{
    char * name;
    int encounter_chance;
    int win_chance;
    int damage;
    int rewardIdx;
    char * winText;
    char * loseText;
    char * rewardText;
};

Animal InitialiseAnimal(char * name, int encounter_chance, int win_chance, int damage, int rewardIdx, char * winText, char * loseText, char * rewardText);
Animal * SetupAnimalTypes();
Item * SetupAnimalRewards();
Animal * SetupAnimals();
void FightAnimal(Animal animal, Item rewardItem, int * outcome, int *reward);
void FreeAnimalPool(Animal * pool, Item * rewards);

#endif