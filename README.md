# Enhanced Gameplay Ability System Plugin

## Plug-in details

This is a plugin that will have extended features of the GAS framework in Unreal Engine 5

**The current version of Unreal Engine:** 5.7.4

> [!NOTE] 
> ~~ Currently looking at porting it over to 5.8.2, but this will take time to make sure nothing major has changed between versions: 10th September, 2026 ~~
> Most recent branch is currently on 5.8.2 and nothing noticeable has broken
> 

## What this plugin aims to achieve

The GAS framework is a bare bones template for beginning to setup different gameplay mechanics. The idea behind this framework is to expand on that drastically so it can be used as a building block for making traditional RPG elements that we would normally find in them. This is getting documented on live confluence that will go into more detail about how to setup specific classes and how some of the classes have been written. 
>[!important]
>The writing of the pages is taking time, so the most important part for me right now is making sure what I put is correct and isn't blatantly wrong such as how the inputs work.


some of the features implemented are: 
* Custom attribute sets for both Combat and Vitals that only communicate through calculations
* Reworked the Input system to work with tags and native inputs
* Enemy AI that has GAS implemented along with Behaviour Tree Nodes to activate abilities based on tags
* UI that can listen to any attribute defined in the editor for scalable UI elements
* Extended the base abilities to include levelling the abilities and handle events


