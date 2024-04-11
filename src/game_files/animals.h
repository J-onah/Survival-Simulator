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

Animal initialiseAnimal(char * name, int encounter_chance, int win_chance, int damage, int rewardIdx, char * winText, char * loseText, char * rewardText);
Animal * setupAnimalTypes();
Item * setupAnimalRewards();
Animal * setupAnimals();
void fightAnimal(Animal animal, Item rewardItem, int * outcome, int *reward);
void freeAnimalPool(Animal * pool, Item * rewards);

#endif