# SpookyProject
Small project I've been working on. While it is planned to be a complete game in the near future, its primary use is to let me experiment freely with new systems
on an already established codebase.

## Current focuses
- Composition over inheritance
- Modular code, dividing game into separable parts
- Physics-based puzzles
- Quest system using [SUQS](https://github.com/sinbad/SUQS)
- Save system using [SPUD](https://github.com/sinbad/SPUD)

Current UE version: 5.4.4

# Notes
## Physics Simulation
- Creating small physics objects can be glitchy when the player steps over it or run into it.
This was resolved by letting the player capsule overlap the smaller objects.
Then, a small sphere capsule is placed at the base of the player so that the physics objects move
to the side when the player walks through them. Creating a separate object channel helps separate
'background' physics objects from interactable ones.~~~~
- Another issue is when small physics objects are put between a 'rock and a hard place'. They will
stutter and flail around until it finds a way out. Adjusting the Max Depenetration Velocity helps resolve this.
But, this means that the physics object has a chance to phase through objects.
- For interacable physics objects (ones where the player can step up on and move around),
setting the correct mass is extremely important to prevent sudden movements in the object.
- In addition, 'falling off' a physics object can sometimes have the object (or player!) launched in
the air. This can be mitigated by disabling CanCharacterStepUpOn and adjusting 'Perch' and 'Ledge' settings in the Character Movement Component.
- Disabling pawn collision while the player is grabbing an object is the only solution I found to prevent
flying while grabbing a physics object under you. A better solution may be possible.

## External Assets Currently Used
- https://elbolilloduro.itch.io/paquete-de-modelos-low-poly-estilo-psx-2
- https://chilly-durango.itch.io/medieval-building-parts