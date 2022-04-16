# API Guide
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
If a Component of a GameObject needs data from other Components of the same GameObject, Init() will acquire shared_ptr's to the relevant classes. This will allow read-writes between components. Init() is called after AddGameObject (in ComponentManager) is called for the component, so all the components should be in a valid state.

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
