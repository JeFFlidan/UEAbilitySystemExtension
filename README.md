# MvAbilitySystem
Extension for UE Gameplay Ability System.

## Table of contents
- [1. Overview](#overview)
    - [1.1 Mv Ability System Component](#mv-ability-system-component)
    - [1.2 Mv Gameplay Ability](#mv-gameplay-ability)
    - [1.3 Mv Gameplay Ability Active](#mv-gameplay-ability-active)
    - [1.4 Mv Gameplay Ability Active Combat](#mv-gameplay-ability-active-combat)
    - [1.5 Mv Gameplay Ability Active Movement](#mv-gameplay-ability-active-movement)
    - [1.6 Mv Gameplay Ability Passive](#mv-gameplay-ability-passive)
    - [1.7 Mv Health Component](#mv-health-component)
    - [1.8 Mv Mana Component](#mv-mana-component)
    - [1.9 Mv Character](#mv-character)
- [2. Blueprint API](#blueprint-api)
    - [2.1 Getters](#getters)
        - [Find Mv Ability System Component](#find-mv-ability-system-component)
        - [Find Player Mv Ability System Component](#find-player-mv-ability-system-component)
        - [Find Health Component](#find-health-component)
        - [Find Mana Component](#find-mana-component)
    - [2.2 Classes](#classes)
        - [2.2.1 Mv Ability System Component](#mv-ability-system-component-api)
            - [Add Input Tag To Ability](#add-input-tag-to-ability)
            - [Remove Input Tag from Ability](#remove-input-tag-from-ability)
            - [Deactivate Passive Ability by Class](#deactivate-passive-ability-by-class)

<a name="overview"></a>
## 1. Overview
This chapter contains general information about all the classes that can be inherited by blueprint or can be used as a type in blueprint.

<a name="mv-ability-system-component"></a>
### 1.1 Mv Ability System Component
`Mv Ability System Component` must be used by all actors in the game if those actors are to use any abilities. For player character this component is stored in the `Player State` object. For any NPC this component must be strored in the actor itself.

<a name="mv-gameplay-ability"></a>
### 1.2 Mv Gameplay Ability
Base class of `Mv Gameplay Ability Active` and `Mv Gameplay Ability Passive`. Contains all common methods for both types of abilities. MUST NOT BE USED AS PARENT CLASS FOR BLUEPRINTS, ONLY C++!!!

<a name="mv-gameplay-ability-active"></a>
### 1.3 Mv Gameplay Ability Active
Contains general methods and events for active abilities. Must be used as parent class for abilities that are neither `Combat` nor `Movement`. An example of usage can be found in `Project Devourer` at `/All/Game/Devourer/Abilities/Basic/Interact`.

<a name="mv-gameplay-ability-active-combat"></a>
### 1.4 Mv Gameplay Ability Active Combat
Must be used as the parent class for combat abilities. You can use one or more animation montages. An example of usage can be found in `Project Devourer` at `/All/Game/Devourer/Abilities/Active/SwordAttack`.

<a name="mv-gameplay-ability-active-movement"></a>
### 1.5 Mv Gameplay Ability Active Movement
Must be used as the parent class for movement abilities. An example of usage can be found in `Project Devourer` at `/All/Game/Devourer/Abilities/Active/Slide`.

<a name="mv-gameplay-ability-passive"></a>
### 1.6 Mv Gameplay Ability Passive
Must be used as the parent class for passive abilities. Passive ability has `Main Gameplay Effect`, that is applied immediately after granting an ability to the actor, and `Additional Gameplay Effects`, that can be applied with a certain chance when the ability is subsequently assigned to the actor. Each additional gameplay effect is granted only once. In almost all cases main gameplay effect must declare some difficult behavior, such as increasing damage after reaching a certain health level, while additional gameplay effects should describe simple logic, such as adding 4% to the player's damage.

<p align="center">
    <img src="/Images/PassiveAbility.png"/>
</p>

<a name="mv-health-component"></a>
### 1.7 Mv Health Component
Contains information about the actor's health.

<a name="mv-mana-component"></a>
### 1.8 Mv Mana Component
Contains information about the actor's mana.

<a name="mv-character"></a>
### 1.9 Mv Character
Must be a base class for each character class that have to use the ability system.

<a name="blueprint-api"></a>
## 2. Blueprint API
All blueprint functions are stored under MVAS category.

<a name="getters"></a>
### 2.1 Getters
All methods for getting components. All getters are pure and can be used without target. If requested component is invalid or does not exist, nullptr will be returned. However, validation methods are not recommended to use for getters. All problems with invalid return values must be resolved without adding additional if branches with validations. 

<a name="find-mv-ability-system-component"></a>
`Find Mv Ability System Component` returns the actor's `Mv Ability System Component`. Must not be used with player character, only NPC. 

<a name="find-player-mv-ability-system-component"></a>
`Find Player Mv Ability System Component` returns the actor's `Mv Ability System Component`. Always returns the player's ability component from active `Player State`. 

<a name="find-health-component"></a>
`Find Health Component` returns the actor's `Mv Health Component`. Can be used in player and NPC blueprints.

<a name="find-mana-component"></a>
`Find Mana Component` returns the actors's `Mv Mana Component`. Can be used in player and NPC blueprints.

<a name="classes"></a>
### 2.2 Classes
All necessary methods inheritted by blueprints (also some enums and events). For some classes there can be not only methods implemented in this plugin, but functions from the base classes of the engine. This is done for convenience and to reduce time spent searching for the necessary information.

<a name="mv-ability-system-component-api"></a>
#### 2.2.1 Mv Ability System Component

<a name="add-input-tag-to-ability"></a>
`Add Input Tag To Ability` adds an input tag to the chosen ability until the input tag is removed. An input tag is associated with one `Input Action`, so the ability with the input tag will be activated when the `Input Action` is triggered. Usage example is below.

<p align="center">
    <img src="/Images/UsageExample_AddInputTagToAbility.png"/>
</p>

<a name="remove-input-tag-from-ability"></a>
`Remove Input Tag from Ability` removes passed input tag from the chosen ability. If everything is successful, the ability will not be activated by the `Input Action` associated with the removed input tag. Usage example is below.

<p align="center">
    <img src="/Images/UsageExample_RemoveInputTagFromAbility.png"/>
</p>

<a name="deactivate-passive-ability-by-class"></a>
`Deactivate Passive Ability by Class` removes chosen passive ability from active list. Once  deactivating, all gameplay effects granted by this passive ability will be removed. Passed `Ability Class` value MUST BE INHERITED FROM `Mv Gameplay Ability Passive` OR ITS CHILD.