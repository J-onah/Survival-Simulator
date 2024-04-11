#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include "animals.h"
#include "item.h"
#include "random.h"
#include "inventory.h"

#define NUM_ANIMALS_TYPE 5
#define POOL_SIZE 100

Animal initialiseAnimal(char * name, int encounter_chance, int win_chance, int damage, int rewardIdx, char * winText, char * loseText, char * rewardText)
{
    Animal animal;
    animal.name = name;
    animal.encounter_chance = encounter_chance;
    animal.win_chance = win_chance;
    animal.damage = damage;
    animal.rewardIdx = rewardIdx;
    animal.winText = winText;
    animal.loseText = loseText;
    animal.rewardText = rewardText;
    return animal;
}

Animal * setupAnimalTypes()
{
    Animal * animals = (Animal *) malloc(sizeof(Animal) * NUM_ANIMALS_TYPE);
    if (!animals)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }

    animals[0] = initialiseAnimal("Snake", 50, 70, 20, 0, 
                                  "You picked up a stick and poked it to death.\n",
                                  "You attempted to pick up a stick to poke it but the snake got you first.\n",
                                  "You obtained some Snake Meat as a result.\n");
    animals[1] = initialiseAnimal("Tiger", 15, 30, 50, 1, 
                                  "You managed to maintain eye contact with the tiger and slowly backed away, showing respect for its territory.\nThe tiger eventually loses interest and slinks away into the jungle.\nYou breathe a sigh of relief, grateful for the close encounter but also aware of the danger.\n",
                                  "The tiger pounced on you with a mighty roar!\n",
                                  "You looked down and found a rare herb known for its adrenaline, providing energy.\n");
    animals[2] = initialiseAnimal("Bear", 15, 40, 50, 2, 
                                  "You stand tall and raise your arms, making yourself appear larger than life.\nWith a firm voice, you command the bear to back off.\nSurprisingly, the bear hesitates, then turns and lumbers away into the forest, leaving you shaken but unharmed.\n",
                                  "The bear overpowered you with its sheer strength!\n",
                                  "You discover a stash of honey hidden nearby, offering nourishment and satisfaction.\n");
    animals[3] = initialiseAnimal("Wolf", 15, 55, 30, 3, 
                                  "You howled back at the wolf, asserting dominance and scaring it away.\n",
                                  "The wolf outmaneuvered you and delivered a decisive blow!\n",
                                  "You scavenge the area and find a hidden stash of edible wild berries, providing much-needed nourishment and relief.\n");
    animals[4] = initialiseAnimal("Crocodile", 5, 10, 70, 4, 
                                  "You wrestled the crocodile and emerged victorious!\n",
                                  "The crocodile grabbed hold of you with its powerful jaws!\n",
                                  "You obtained some Crocodile Meat.\n");
    return animals;
}

Item * setupAnimalRewards()
{
    Item * rewards = (Item *) malloc(sizeof(Item) * NUM_ANIMALS_TYPE);
    Item snakeMeat = initialiseConsumables("Snake Meat", 20, 20, 0, 100);
    Item rareHerb = initialiseConsumables("Rare Herb", 100, 0, 0, 30);
    Item honey = initialiseConsumables("Honey", 50, 30, 10, 50);
    Item berries = initialiseConsumables("Berries", 20, 20, 60, 80);
    Item crocMeat = initialiseConsumables("Crocodile Meat", 75, 75, 50, 100);
    rewards[0] = snakeMeat;
    rewards[1] = rareHerb;
    rewards[2] = honey;
    rewards[3] = berries;
    rewards[4] = crocMeat;

    return rewards;
}

Animal * setupAnimals()
{
    int i, j;
    int start = 0;
    int end = 0;
    Animal current;
    Animal * animals = setupAnimalTypes();
    Animal * pool = (Animal *) malloc(sizeof(Animal) * POOL_SIZE);
    if (!pool)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < NUM_ANIMALS_TYPE; i++)
    {
        current = animals[i];
        end += current.encounter_chance;
        for (j = start; j < end; j++)
        {
            pool[j] = current;
        }
        start = end;
    }
    free(animals);
    return pool;
}

void fightAnimal(Animal animal, Item rewardItem, int * outcome, int *reward)
{
    int fightRNG;
    int itemRNG;
    fightRNG = getRNGMod(100) + 1;
    if (fightRNG >= animal.win_chance)
    {
        *outcome = TRUE;
        printf("%s\n", animal.winText);
        itemRNG = getRNGMod(100) + 1;
        if (itemRNG >= rewardItem.chance)
        {
            *reward = TRUE;
        }
    }
    else
    {
        *outcome = FALSE;
        *reward = FALSE;
    }
}

void freeAnimalPool(Animal * pool, Item * rewards)
{
    free(pool);
    free(rewards);
}
