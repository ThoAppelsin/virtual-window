Virtual Window was a project where I prepared a 3D computer graphics application for Universal Windows Platform
using DirectX 11 and DirectXTK. Unlike a traditional 3D computer graphics application, this application allows
the viewer to move their heads to view and observe the content behind the screen, as if they were looking through a
window of their room.

## Overview

Today's computer screens are usually flat, and limited to displaying us a plane of image.
However, with just a simple 3D perspective projection of a modelled 3D world,
we can create the illusion of a 3D world behind the screen.

On the other hand, this illusion can easily be falsified through some simple testing, like:

1) closing one of our eyes and then the other, or
2) moving our heads up and down, left and right.

The image on your computer screen will likely remain indifferent to those, and you will then immediately realize
that what you see on the screen is just a projection. For a better illusion of 3D on our 2D screens,
we will need to come up with technical improvements to how we make our 3D projections, so that the consumers
will have a harder time to realize that it is just a projection, and therefore be more immersed.

Virtual Window aims towards the second one of those two tests above. The demonstrating application
presented in this repository, displays a 3D world to the user that is projected onto the screen
with regards to the viewer's position relative to the screen. The end result is closer to what would
happen in reality, if our computer screens were a window.

## Motivation
