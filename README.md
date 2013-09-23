Byteland - Solving a graph theory problem using C
========

Problem
--------

Byteland consists of N cities numbered 1..N.
There are M roads connecting some pairs of cities.
There are two army divisions, A and B, which protect the kingdom.
Each city is either protected by army division A or by army division B.  

You are the ruler of an enemy kingdom and have devised a plan to destroy Byteland.
Your plan is to destroy all the roads in Byteland disrupting all communication. 
If you attack any road, the armies from both the cities that the road connects comes for its defense.
You realize that your attack will fail if there are soldiers from both armies A and B defending any road.  

So you decide that before carrying out this plan, you will attack some of the cities and defeat the army located in the city to make your plan possible.
However, this is considerably more difficult. 
You have estimated that defeating the army located in city i will take up ci amount of resources.
Your aim now is to decide which cities to attack so that your cost is minimum and no road should be protected from both armies A and B. 


Solution
--------

This program solves the problem by, first, spotting the links between cities that are protected by two different armies.
For each city, the program looks at the ratio (cost of destroying a city)/(number of linked cities that are protected by another army).
It also calculates the cost of destroying every one of its linked cities protected by another army.

Then, with all that information stored in arrays, the program determines which city is to be deleted.
We first delete the cities that have the most linked cities protected by another army unless destroying that city is more expensive than destroying every one of these linked cities.
If two cities have the same number of linked cities, we look at their ratio (cost of destroying a city)/(number of linked cities that are protected by another army) and we delete the city with the smallest ratio.


How to build
-------------

1. Debug (verbose) mode :
gcc -o byteland byteland.c -DDEBUG -Wall

2. Release mode :
gcc -o byteland byteland.c -Wall

How to use
------------

Run the command :
./byteland < input00x.txt
