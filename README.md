# 2-D Sandbox Game
#### This is a sandbox 2-D game inspired by Terraria made almost from scratch. It is written in C++ utilizing Raylib 4.0.

# About
## World Generation
#### The terrain generation is done by generating psuedo random values at each block position and then processing the values to decide whether or not to add blocks/walls. Above a certain height 1-D perlin noise is used to determine the height of blocks at each column and thus create surface hills. Below a certain height we switch to using 2-D perlin noise, the value returned is compared to a threshold if its above that threshold a block is placed otherwise it is left empty creating underground caves.

### Surface
![Surface](https://user-images.githubusercontent.com/96942793/185235032-0e3fac5a-4d5e-45ec-80fd-7a69baa15714.png)


### Cave
![Cave](https://user-images.githubusercontent.com/96942793/185235008-51c5a391-4d3a-4d05-9460-558dccd8dc54.png)



## Colision Resolution
#### A basic grid system and AABB collision detection method are used to resolve any collisions. 
![Collisions GIF](https://github.com/AbdulrahmanEl-Bedewy/Terraria_RipOff/blob/b534d088d7aa32587ee02e0116a0b92d0e9ce9a4/ScreenShots/Collision.gif)


# Gameplay
#### A showcase of gameplay.
https://user-images.githubusercontent.com/96942793/185233233-cfa0baa0-df30-45d3-a60e-521f899cff21.mp4



Day/Night cycle
![Day_Night](https://user-images.githubusercontent.com/96942793/185234237-a6e7502b-03fb-45cb-b775-4adae2ecff3c.gif)


## Weapons
### Bow

![Bow](https://user-images.githubusercontent.com/96942793/185234177-809938be-19ed-4d5b-928b-8b4fbc0cfb0f.gif)


### Sword

![Sword](https://user-images.githubusercontent.com/96942793/185234218-08d04ff6-ab68-4b56-ab7c-76a827284a17.gif)

### Pickaxe
![Pickaxe](https://github.com/AbdulrahmanEl-Bedewy/Terraria_RipOff/blob/3c3b46948753e26970af324116e79a6dacb231c1/ScreenShots/Pickaxe.gif)

## Controls
- WASD for movement
- Space to jump
- Enter to expand storage
- Mouse left click to use selected item
- Scroll wheel to change selected item in hotbar
- R to reset player position to spawn point
- P to pause

## Task List
- [ ] Improve collisions 
- [x] Add world save/load functionality
- [ ] Improve memory usage by loading chunks of the world
- [ ] Add moving enemies
- [ ] Add ores
- [ ] Add trees
- [ ] Add health bar
- [ ] Add clouds
 ## Credits
 - [Day backgrounds](https://szadiart.itch.io/bakcground-hill)
 - [Sun](https://www.freeiconspng.com/img/48190)
 - [Night backgrounds](https://nyknck.itch.io/background-set-pixel-assets)
 - [Player sprite](https://opengameart.org/content/fantasy-character-npc-sprites)
 - [Weapons](https://opengameart.org/content/lpc-medieval-fantasy-character-sprites)
 - [Pickaxe](https://opengameart.org/content/tool-icons)
 - [Dirt blocks](https://mamanezakon.itch.io/forest-tileset)
 - [Dirt wall](https://opengameart.org/content/lpc-dirt-wall-sidescroller)
 - [Background music](https://opengameart.org/content/outdoor-environment-music)
 - [Perlin Noise Algorithim](https://github.com/SRombauts/SimplexNoise)
