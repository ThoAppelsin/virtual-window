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

Virtual Window aims to let the 3D applications pass the second test.
The world behind is projected with respect to the viewer's position, so it becomes harder
for the viewer to realize that it is just a projection, and the experience is more realistic and immersive.
Finally, we can and we do this only by using a single front-facing camera.

## Motivation

This idea to make the 3D projection responsive to my movements against the screen as a viewer
came to me when I was taking the introductory course to computer graphics. I grew more passionate about the idea
as I remembered how I was trying to see down below by moving my head up close to the screen when I was
playing an old 3D game as a child.

I was wondering how come nobody had ever thought of this before, but also was sure that nobody hasn't,
since otherwise there would definitely be at least some games or applications that I would have heard of using
this technique to provide more immersive experience to their users. Thinking that I was onto something
novel, I had worked on this project fully motivated for weeks.

I am glad that it was after I have finished preparing my demonstrating application,
when I found out that other people had already done this before. It was still disappointing,
but would also be demotivating if I were to discover this when my project was half-done.




