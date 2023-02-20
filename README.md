# Assignment

Documentation of implemented features.

Health and stamina of player can be modified through BP_ThirdPersonCharacter.

Triggerbox is BP that can be placed in the level. The amound of damage dealt,
as well as its size can be modified in BP_CampFire. (If there was requirement
for various types of Trigger Boxes to deal damage - I'd create abstract class and derive CampFire from it) 

Pause Menu:
Pressing "Esc" button pauses the game and allows the player to change the window mode setting. 

Inventory:
Currently there is an instance of only one item - Key.
To add more items user should derive from abstract class  UItem and assign properties either in C++ or BP to set up mesh, texture, name ect.
To be able to place pickable items in the world, the user should derive from "Pickup" class and assign desired item to "Item Template"
in the defaults in editor.

I think that for actual game it would be necessary to implement more universal interaction system to interact with objects in the world
and display widgets and indicators that would help the player to understand the logic and what he could do with various items.

Note. C++ classes should have been organised from the beggining such as in Blueprints folder but
editor stopped me from creating folders and reorganising classes due to references. 
