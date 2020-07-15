# Boids

This is an implementation of the famous "boids" algorithm, developed by Craig Reynolds. It creates many boids, each of which follows simple rules to create the behavior of flocking, such as with birds or fish. The code is written in C++ and uses SFML for rendering the boids and taking in user input.

The weighting of the rules used to steer the boids can be modified as the program runs, which can help one better understand how each parameter affects steering. The main parameters are cohesion, alignment, and separation. You can learn more about these from here: https://en.wikipedia.org/wiki/Boids

Color fear is the repulsion from differently colored boids. Mouse fear is the repulsion from the mouse. Max neighbors can be used to decrease the number of neighboring boids being looked at, which can yield significant performance increases without obvious changes in boid behavior.

The number of boids can be changed with the up and down arrows. The trail length left by the boids can be changed with the left and right arrows. The number of differently colored boids can be changed with the + and - keys. The opacity of the boid trails can be changed with the 'o' key.

To run the program from an executable, have the files from Boids -> Release in the same location and run Boids.exe.
