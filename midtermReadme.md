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
- h: see the help menu, when in doubt, press h
- h: close the menu to move forward
- (1): see the grouper and the shrimp coming in its mouth and out of its gills
- 2: see the butterfly fish perfectly kiss the top of the staghorn coral
- 3: see the bait ball, it's wild. recommend that you swim into it and watch a bit
    - p: pause the animation to really check out the bait ball, note that each fish is a texture on a low vertex sphere
    - p: resume play mode
- 4: see the eel, it's hiding in a nook (we'll see the lighting soon enough)
- 5: see the sharks in their peacful circle
- m: see the orhtoganl view of the scene
- m: see the perspective view of the scene
- o: see the backdrop of the scene without all the critters (can check normals here)
- o: see the grouper alone, likely easier to see the animation now
- o: see the butterfly fish alone, ssdd
- o: see the eel
    - HERE: be sure to check out the eel lighting and shape
    - key tasking was to make this tubing and ribbon structure


## Default mode
- play mode with all objects
- first person perspective view
- centered on grouper
- in day mode
- with flashlight on
- axis enabled


## Key bindings:
- h: to enter and exit help mode
- 0: return to original setup
- m: change view mode [orthogonal, perpective, first person]
- o/O: change the object of interest (RECOMMEND: change view mode 'm')
- 1 - 5: go to set points in the scene
    - 1: grouper (note the cleaner shrimp)
    - 2: butterfly fish (note the arch pattern)
    - 3: bait ball (about 1000 fish and doesn't slow the tool down too much)
    - 4: eel (note how it's hiding in the cliff)
    - 5: sharks (note different sizes and swim pattern with correct angles)
- n: toggle Night and Day mode
- f: toggle flashlight
- l: toggle coordinate lines
- p: pause/play auto changes
- <asdw>: change player/diver position
- <arrows>: move the angles
- ESC: end the program


## Self Reflection
I may have misunderstood something early in the semester. I was under the impression that if we spent excessive time on the homeworks early in the term, then that can count toward our project hours. Here is what I've done (in bullet form for easy grading)
- a method in baseShapes.c 'drawTexturedTube' that will draw a tube around a defined series of centers like we see in the eel, I think it's normals are correct
- a method in baseShapes.c 'drawTexturedRibbon' that will draw a flat shape based on the points provided, the normals are a bit off, but it looks cool on the eel so I'm fine with that
- a custom shader that allows for per pixel lighting on the backdrop, but it doesn't support the spot light yet, so it's kinda lame for now
- I learned that I can't use alpha to easily remove make black in an object transparent. Based on lectures, I think this requires a custom shader


## What's Next?
- I look forward to your advise on how to prioritize these
- Goals:
    - implement some/all of the following in some technical way (not just a texture on a low vertex ball): lion fish, jelly fish (alphas), squid (alphas), turtle, seafans, stingray
    - really fill in the space with interesting things for the diver to see: I am very unsure about where this should be in my priorities vs spending more time on advanced graphics things that would be hard to notice without reading the README, but capture the spirit of graduate work in computer graphics
    - fix the shader to work with the spotlight
    - add fog
- Stretch Goals:
    - clean the code and address the TODOS and add good enough comments that I could look at this in 3 years and have a sense of what is going on (mininmum version of this is a given)
    - fix the shader to make a more realistic red removal. In experiments so far, since the system is RGB, just removing read makes things in the deep look cyan, but in reality they should be more brown.
    - a school of fish that swims in a realistic way: this is really HARD between the randomness of setup and the method of turning the fish
    - make the baitball reactive to the diver, if the diver is in the ball, then the fish will move away from the diver (given that this is basically the airflow project you showed, I'm guessing this is really HARD)


# TIMING
32 hours so far as best as I can determine from my calendar (through 11/29)


# RESOURCES
- great textures: https://withpoly.com/browse/textures


