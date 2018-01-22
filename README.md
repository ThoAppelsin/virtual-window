Virtual Window project is about making the 3D computer graphics applications more realisticly three dimensional.
It demonstrates the idea of projecting the 3D world behind onto screen with respect to the viewer's position,
on a Universal Windows Platform application written in C++ using DirectX 11 and DirectXTK.

Before I had started to this project, my aim was to turn my computer screen into a window to the virtual
world behind, hence I chose the name *Virtual Window* for the project.

## Overview

Today's computer screens are usually flat, and limited to displaying us a plane of image.
However, with just a simple 3D perspective projection of a modelled 3D world,
we can create the illusion of a 3D world behind the screen.

On the other hand, we can easily see through this illusion, and find out that what we see is just a projection.
How? Try:

1) closing one of our eyes and then the other, or
2) moving our heads up and down, left and right.

What you see on your computer screen will likely not change when you do those.
There are some technologies that allows you to see different images with your both eyes,
some of them even without special glasses. With or without glasses, they always require a special screen,
but then make it possible for an application to pass the first test. This project is not about the first test.

Virtual Window aims to let the 3D applications pass the second test, with the only special equipment being
a front-facing camera attached to the screen.


indifferent to those, and you will then immediately realize
that what you see on the screen is just a projection. For a better illusion of 3D on our 2D screens,
we will need to come up with technical improvements to how we make our 3D projections, so that the consumers
will have a harder time to realize that it is just a projection, and therefore be more immersed.

Virtual Window aims towards the second one of those two tests above. The demonstrating application
presented in this repository, displays a 3D world to the user that is projected onto the screen
with regards to the viewer's position relative to the screen. The end result is closer to what would
happen in reality, if our computer screens were a window.

## Motivation
