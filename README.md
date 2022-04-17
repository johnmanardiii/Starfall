# API Guide
1. [GameObject](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#gameobject)
2. [Component](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#component)
3. [ComponentManager](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#componentmanager)
4. [Camera](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#camera)
5. [GameState](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#game-state)
6. [RandomGenerator](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#randomgenerator)
7. [Ground](https://github.com/Bob-Loth/Lab2-EC-Model/edit/master/README.md#ground-drawground-initground)


# Component Data Structure
## GameObject
All a GameObject is, is a name, and a map between any components it might have, and the index of that component in ComponentManager's component vectors.
GameObject has methods GetComponentLocation and GetComponentLocations in order to get these indices. This is important for linking components, described later.

### Adding a GameObject
In ComponentManager::Init(), if at program start, or in ComponentManager::UpdateComponents, if during program:
- Give the GameObject a unique name, one that hasn't been used before.
- If it's renderable (has a Renderer component) specify the name of the .obj file (without the extension) as well.
- Create shared_ptr's (of the final, derived type) to any components you want the object to have.
- Initialize the component's starting values.
- Make a vector<shared_ptr<Component>> of those components, and pass that and the unique name to AddGameObject.



## Component
A component is a piece of a GameObject, that has a part of its data and behavior. It's a virtual class, with derived classes requiring Update and Init, described below. Each component has a Name, and an IsActive determining if its Update() will be called in the render loop. The rest is up to the derived class.
### Update
Each Update will be called every frame, for all active components. 

### Init
If a Component of a GameObject needs data from other Components of the same GameObject, Init() will acquire shared_ptr's to the relevant classes. This will allow read-writes between components. Init() is called after AddGameObject (in ComponentManager) is called for the component, so all the components should be in a valid state. the Collect Component is heavily linked, as it receives and updates information across a huge amount of its total components, and it serves as a good example of how to do this.

### Adding a new Component:
- Make a new derived class that inherits Component, or a derived class of Component (TextureRenderer does this already, for an example).
- Implement an Init: (this function can do nothing if it doesn't depend on other components.)
	- call componentManager->GetGameObject(Name) to get the indices of the other components you want to link.
	- call componentManager->GetComponent("ComponentName", index) to get a shared_ptr<Component> of the specified component.
	- static_pointer_cast the shared pointer to the concrete type.
- Implement an Update (this function can do nothing)
	- Update should be called per-frame, but implementation details are up to the component.
	- It's probably a good idea to make a new function for anything that doesn't always happen per-frame.
- Add the data structure to ComponentManager.
	- In ComponentManager's constructor, invoke the default constructor and give the underlying components a name.
	- Add a condition to ComponentManager::addToComponentList, in the same format as the others. Use the same name as in the constructor.
	- Add in a loop in ComponentManager::UpdateComponents that calls Update on all active components in the vector, or whatever other behavior is desired for those components.
## ComponentManager
ComponentManager is the heart of the program. It provides abstractions for the setup, updating, and teardown of the various Components that make up GameObjects.

### Data Layout
Each derived-class Component has a corresponding vector of shared pointers, and a priority queue filled with the indices of components whose GameObject has been removed, and are ready to be overwritten.

AddGameObject and RemoveGameObject should be used to deal with this data structure. There is also a vector of the "official" component names (componentVectorNames) that can be used to properly link components.

### Init
Where GameObjects get initially added, before the first render pass.
Details about adding GameObjects are in "Adding a Game Object".

### UpdateComponents
Overall, this loop:
- Does things like add GameObjects at the beginning.
- For every component type except Renderer,
	- checks whether the component is still active
	- calls Update on the component.
- Updates the camera data.
- For Renderer, specifically after updating camera,
	- checks whether the Renderer is active
	- calls Update on the Renderer, which draws itself.

This is different for collision, because Collision components interact with other Collision components, as well as the edges of the ground and the camera.

## Camera
Singleton class, that currently performs common first-person camera transforms. Receives data from glfw callbacks and updates every frame. Currently implements its own collision detection for the purposes of Lab, but that is easily stripped out.

## Game State
This will change drastically, as it now just holds global state data and compute for the lab2 game. Things that might still be useful are:

- cumulativeFrameTime and IncTotalFrameTime()

## RandomGenerator
provides an easy way to get a lot of uniformly-distributed random floats and vec3's. Should be fairly straightforward to add more data types.
```
RandomGenerator rg(-10, 10);
float f = rg.GetFloat() //gets a random float in the range [-10,10] inclusive.
```
## Ground (drawGround, initGround)
the CPU-defined data used for the ground plane. We can either expand on this or load a flat mesh from an obj file.
