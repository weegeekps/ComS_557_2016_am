# gl-breakout
OpenGL Breakout Game written for ISU ComS 557
Author: Adam Morris

## Features

* Bump mapping on the walls and paddles.
* Environment mapping on the ball.
* Ball animation based on it's interaction with it's environment.
* Triggered lighting on the blocks and ball that changes when the ball impacts the walls.

## Controls

* Left Arrow moves the Paddle left.
* Right Arrow moves the Paddle right.
* Space starts the Ball moving.

## Notes on Mechanics

While the main purpose of this project was to show off what I had learned in class, I also strived to make this an actual game. I feel that goal has been achieved. These notes should help you understand the mechanics of the game I created.

* The ball speeds up along the z-axis (vertical from the players position) when the side walls are hit.
* The ball slows down along the z-axis when the back wall is hit.
* Where the ball impacts the paddle changes the x-axis (horizontal) velocity. Edges speed it up, but the center slows it down.
* If the ball goes out of bounds, the ball and paddle reset to their starting positions, and you can continue playing.
* Before you launch the ball, you can set the launch direction by moving the paddle. It always launches in the last direction of the paddle.
* When you win, everything stops and you can't reset. Just have to close the program. Good job!

## Known bugs

There are one or two bugs that I didn't have time to finish before turning in.

* If the ball gets stuck behind a block between the blocks and the back wall the collision detection can go a bit wonky.
* I have no idea if the CMake file still works. YMMV with that. I recommend just opening the included VS project file and fixing the broken references.
