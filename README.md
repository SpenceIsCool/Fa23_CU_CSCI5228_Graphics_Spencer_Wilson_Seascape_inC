# Final Project: Seascape
Spencer David Wilson
CSCI 5229 Fall 2023


## BUILD and RUN
try
~~~
./runMe.sh
~~~

if that fails:
- build: `make`
- run: `./project`


## RECOMMENDED PATH
- click the button: observe the thing, maybe swim around a bit (arrows and asdw)
    - take note of all the transparent objects in the scene
    - this is handled by the `struct Alpha` in SDW.h
    - and the code in `alpha.c`
    - the code is basically written as a thunk/promise that we execute later
    - we execute this only after the non-alpha objects are all drawn in the scene
- h: see the help menu, when in doubt, press h
- h: close the menu to move forward
- <nothing>: note the red sea fan is transparent (on your right)
- 2: see the butterfly fish perfectly kiss the top of the staghorn coral
- 3: see the jellyfish (fully transparent objects)
- 4: see the grouper and the shrimp coming in its mouth and out of its gills (note that the eyes have a transparent film on them)
- 5: see the squid as they change colors about half of the time. They eyes have a transparent film and the side fins are transparent.
- 6: observe the sharks swimming in harmony with reasonable angles. observe the tranparent film on the eyes. Observe the effect of fog in the scene (as the shark far away is difficult to make out).
- 7: see the eel, it's hiding in a nook (we'll see the lighting soon enough)
- 8: see the turtles doing some synchronized swimming. press and hold "w" to swim down the turtles center. Note the tranparent eye film again
- 9: see the bait ball, it's wild. recommend that you swim into it and watch a bit. You might notice the eel and grouper here.
    - p: pause the animation to really check out the bait ball, note that each fish is a texture on a low vertex sphere
    - p: resume play mode
- m: see the orthoganal view of the scene
- m: see the perspective view of the scene
- NOW: we can look at the objects of the scene and their lighting in isolation
- o: see the backdrop of the scene without all the critters (can check normals here)
    - press 8
    - as the light comes around note that the upper sand bank is more red
    - the custom shader is removing red at lower depths (recall, can you p to pause/play the light)
- o: see the blocking reef structure, note this includes transparent red sea fans
- o: see the grouper alone, likely easier to see the animation now
- o: see the butterfly fish kissing its two reef sections
- o: see the jellyfish, check out its shape and colors, I think this is particularly fun to look at
- o: see the squid, check out the transparency of its fins
- o: check out the bait ball, from certain angles this shows interesting shapes of the random number generator (press 8)
- o: see the eel
    - HERE: be sure to check out the eel lighting and shape
    - key tasking was to make this tubing and ribbon structure
- o: see the turtles spinning around the y axis


## Default mode
- play mode with all objects
- first person perspective view
- centered near the butterfly fish as a starting point for the dive
- in day mode
- with flashlight on
- axis disabled


## Key bindings:
- h: to enter and exit help mode
- 0: return to original setup
- m: change view mode [orthogonal, perpective, first person]
- o/O: change the object of interest (RECOMMEND: change view mode 'm')
- 1 - 9: go to set points in the scene
    - 1: explorer starting point
    - 2: butterfly fish (note the arch pattern)
    - 3: jellyfish (note the tranparency)
    - 4: grouper (note the cleaner shrimp)
    - 5: squid (changes color and has transparent fins)
    - 6: sharks (swim in correct angles)
    - 7: eel (it's higing on the cliff)
    - 8: turtle (went a bit overboard in the swimming shapes)
    - 9: bait ball (about 1000 fish and doens't slow down the viz much)
- n: toggle Night and Day mode
- f: toggle flashlight
- l: toggle coordinate lines
- p: pause/play auto changes
- <asdw>: change player/diver position
- <arrows>: move the angles
- ESC: end the program


## Self Reflection
What a fun assignment to work on. Here is what I've done (in bullet form for easy grading):
- I'm most proud of this (even though its a bit hacky)
    - a method to store alpha blended methods in an array with the relevant spec so that you can draw the opque objects and store all the instructions to draw transparent objects at a later time. It will re-apply the definition time projection and model-view matrix then apply the spec provided before executing the method to draw the transparent object. This is used for the jellyfish, squid and red seafan, as well as for a transparent film applied to most eyeballs in the scene.
    - in project.c: `_ALPHAS` is a list of length 1000
    - throughout the code execution, spec dependent drawing instructions can be added to `_ALPHAS` (using project.c `updateAlphas`)
    - after all the opaque objects are drawn, then project.c `handleAllAlphas` will actually perform the drawings depent on code in `alpha.c` which re-apply the drawing definition time modelview and projection matrix before executing the drawing instructions with the related spec.
    - limitation: currently the list lenght is static, so it could mess up on more complex scenes. It's also a bit space intensive to store a length 1000 pointer to structs with function pointers and 16 point matrixes and so on.
    - future work: move to an object oriented language for better control of challenges like these
    - presentation: I plan to focus on this for my presentation on Thursday
- by midterm: a method in baseShapes.c 'drawTexturedTube' that will draw a tube around a defined series of centers like we see in the eel, I think it's normals are correct
- by midtern: a method in baseShapes.c 'drawTexturedRibbon' that will draw a flat shape based on the points provided, the normals are a bit off, but it looks cool on the eel so I'm fine with that
- by midtern: a custom shader that allows for per pixel lighting on the backdrop, but it doesn't support the spot light
- by midterm: grouper feeding station, butterfly fish actions, sharks that turn realisitically, eels, and a baitball
- additional features: larger reef systems, sea fans (alpha), squid (alpha), turtle, jellyfish(alpha)
- significantly more involved scene with some measure of suspense and aw built into the users experience.
- added blue fog to the scene


## Goals completed since the midterm:
- implement some/all of the following in some technical way (not just a texture on a low vertex ball): lion fish, jelly fish (alphas), squid (alphas), turtle, seafans, stingray
- really fill in the space with interesting things for the diver to see: I am very unsure about where this should be in my priorities vs spending more time on advanced graphics things that would be hard to notice without reading the README, but capture the spirit of graduate work in computer graphics
- add fog


## Goals left unfinished from the midterm:
- fix the shader to work with the spotlight (spent hours on this, can't figure it out)
- Stretch Goals:
    - clean the code and address the TODOS and add good enough comments that I could look at this in 3 years and have a sense of what is going on (mininmum version of this is a given)
    - fix the shader to make a more realistic red removal. In experiments so far, since the system is RGB, just removing read makes things in the deep look cyan, but in reality they should be more brown.
    - a school of fish that swims in a realistic way: this is really HARD between the randomness of setup and the method of turning the fish
    - make the baitball reactive to the diver, if the diver is in the ball, then the fish will move away from the diver (given that this is basically the airflow project you showed, I'm guessing this is really HARD)


# TIMING
48 hours total
32 hours so far as best as I can determine from my calendar (through 11/29)
16 hours after the project review


# RESOURCES
- great textures: https://withpoly.com/browse/textures
- significant code samples come from the work of Willem A. (Vlakkies) Schreüder and his teem for CSCI 4228/5228 Computer Graphics course at the university of color boulder
- Addition materials found on stackoverflow, LearnOpenGL, and Khronos (among others cited within the code base)


