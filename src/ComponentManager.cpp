#include "ComponentManager.h"

GameObject ComponentManager::GetObject(string name)
{
    //TODO this should return a reference/copy (discuss) to a game object
    //based on the key in a map.
    auto it = objects.find(name);
    if (it == objects.end()) {
        std::cerr << "GetObject with name: " << name << " was not found in objects list." << std::endl;
        throw std::runtime_error("object not found");
    }
    return it->second;
}

shared_ptr<Component> ComponentManager::GetComponent(string compName, int index)
{
    if      (compName == componentVectorNames[0]) { //movement
        return movements[index];
    }
    else if (compName == componentVectorNames[1]) { //transform
        return transforms[index];;
    }
    else if (compName == componentVectorNames[2]) { //collision
        return collisions[index];;
    }
    else if (compName == componentVectorNames[3]) { //render
        return renderers[index];
    }
    else if (compName == componentVectorNames[4]) { //collect
        return collects[index];
    }
    else {
        throw "unexpected component name error";
    }
}

void ComponentManager::Init(std::string resourceDirectory)
{
    camera = Camera::GetInstance(vec3(0,1,0));

    // Initialize the GLSL program, just for the ground plane.
    auto prog = make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
    prog->Init();
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addAttribute("vertPos");
    
    //these generates random things between the two values. Currently supports floats and vec3's, but is easy to add to.
    RandomGenerator randMove(-10, 10);
    RandomGenerator randTrans(-40, 40);
    RandomGenerator randScale(0.2, 2);

    //make some starting objects, with the same assets but different starting positions and velocities.
    for (int i = 0; i < state.GetInitialCount(); i++) {
        string sphereName = "suzanne" + to_string(state.GetCount());
        string sphereShapeFileName = "suzanne";
        shared_ptr<Renderer> renderer = make_shared<TextureRenderer>(sphereShapeFileName, "Cat", sphereName);
        vec3 startingVelocity = vec3(randMove.GetFloat(), 0, randMove.GetFloat());
        shared_ptr<Movement> movement = make_shared<Movement>(sphereName, startingVelocity);
        shared_ptr<Transform> transform = make_shared<Transform>(sphereName);
        shared_ptr<Collision> collision = make_shared<Collision>(sphereName, sphereShapeFileName);
        shared_ptr<Collect> collect = make_shared<Collect>(sphereName);
        transform->ApplyTranslation(vec3(randTrans.GetFloat(), 1, randTrans.GetFloat()));
        
        float scale = randScale.GetFloat();
        transform->ApplyScale(vec3(scale, 1, scale));

        vector<shared_ptr<Component>> sphereComps = { renderer, movement, transform, collision, collect };
        AddGameObject(sphereName, sphereComps);
    }
}

void ComponentManager::UpdateComponents(float frameTime, int width, int height)
{
    // update movements
    for (auto& move : movements)
    {
        if (!move->IsActive) continue;
        move->Update(frameTime, this);
    }
    //resolve collisions.
    for (auto& giver : collisions) {
        //TODO do collisions with the player, and then the ground border here.
        if (!giver->IsActive) continue;  //don't collide with destroyed objects.
        giver->Update(frameTime, this);
        for (auto& receiver : collisions) { //the naive n^2 approach.
            if (!receiver->IsActive) continue; //don't collide with destroyed objects.
            if (giver.get() == receiver.get()) continue; //don't collide with yourself, that's just ridiculous.
            //now we know that both objects are active, and could potentially collide.
            //Resolve updates gameObject information for both giver and receiver if a collision occurred.
            giver->Resolve(receiver, frameTime);
        }
        
    }

    // update transforms based on movements.
    for (auto& trans : transforms)
    {
        if (!trans->IsActive) continue;
        trans->Update(frameTime, this);
    }

    // update flashing animation
    for (auto& collect : collects)
    {
        if (!collect->IsActive) continue;
        collect->Update(frameTime, this);
    }

    //update camera position.
    camera.CalcPerspective(width, height);
    camera.Update(frameTime, this);
    //finally update renderers/draw.

    for (auto& rend : renderers)
    {
        if (!rend->IsActive) continue;
        rend->Update(frameTime, this);
    }
}

void ComponentManager::AddGameObject(string name, vector<shared_ptr<Component>> comps)
{
    map<type_info*, size_t> componentList;
    //don't care what container is used to pass in components,
    //Pad unused components with null.
    map<string, size_t> objComps;
    for (const auto& comp : comps) {
        if (!comp) continue; //null check
        //if not null
        auto p = addToComponentList(comp);
        objComps.insert(p);
    }
    objects[name] = GameObject(name, objComps);
    
    for (auto& comp : objects[name].GetComponentLocations()) {
        string name = comp.first;
        size_t index = comp.second;

        if (name == componentVectorNames[0]) { //movement
            movements[index]->Init(this);
        }
        else if (name == componentVectorNames[1]) { //transform
            transforms[index]->Init(this);
        } 
        else if (name == componentVectorNames[2]) { //collision
            collisions[index]->Init(this);
        }
        else if (name == componentVectorNames[3]) { //render
            renderers[index]->Init(this);
        }
        else if (name == componentVectorNames[4]) { //collect
            collects[index]->Init(this);
        }
        //TODO add additional potential components.
    }//end processing component vector freeing
}

//add the component to the first-available position of the corresponding vector of components.
pair<string, size_t> ComponentManager::addToComponentList(const shared_ptr<Component>& comp) {
    //these series of checks check whether an attempted cast returns a nullptr, indicating that
    //a cast could/couldn't take place, effectively type-checking the object.
    int index = -1; //if this remains at -1 there wasn't a successful cast.
    string compType = "undefinedComponentType"; //make it really obvious if not detected.
    if (auto ptr = dynamic_pointer_cast<Movement>(comp)) {
        index = getNextOpenSlot(movementSlots);
        compType = componentVectorNames[0];
        addHelper(ptr, movements, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Transform>(comp)) {
        index = getNextOpenSlot(transformSlots);
        compType = componentVectorNames[1];
        addHelper(ptr, transforms, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Collision>(comp)) {
        index = getNextOpenSlot(collisionSlots);
        compType = componentVectorNames[2];
        addHelper(ptr, collisions, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Renderer>(comp)) {
        index = getNextOpenSlot(rendererSlots);
        compType = componentVectorNames[3];
        addHelper(ptr, renderers, index);
    }
    else if (auto ptr = dynamic_pointer_cast<Collect>(comp)) {
        index = getNextOpenSlot(rendererSlots);
        compType = componentVectorNames[4];
        addHelper(ptr, collects, index);
    }
    //TODO the other concrete types. Format should be pretty much identical.
    if (index == -1 || compType == "undefinedComponentType") {
        cerr << "new components should have a similar else-if block, that casts to the "
             << "concrete type before insertion into the component vector." << endl;
    }
    // finally(this should happen at the end, unconditionally, for all component types :
    //return the index where the component resides now.
    return make_pair(compType, index); //insert this into the GameObject.
}

//This modifies compList by inserting comp.
template<typename T>
void ComponentManager::addHelper(shared_ptr<T> comp, vector<shared_ptr<T>>& compList, int& index) {
    if (index == -1) {
        index = static_cast<int>(compList.size());
        compList.push_back(comp);
    }
    else {
        compList[index] = comp;
    }
}

//returns the index of the next open slot, or -1 if there are no open slots.
int ComponentManager::getNextOpenSlot(OpenSlots& slots) {
    //this indicates that there are either no available slots in an empty vector,
    //or there are no empty slots in a vector with things in it.
    //Either way the correct response is push_back.
    if (slots.empty()) return -1;
    else {
        size_t val = slots.top();
        slots.pop(); //remove the element, as the corresponding position is going to be immediately filled.
        return val;
    }
}

void ComponentManager::RemoveGameObject(string name)
{
    //make a copy, don't actually remove from map until its components are all gone
    GameObject obj = GetObject(name);
    assert(name == obj.Name); //DEBUG quick sanity check
    map<string, size_t> comps = obj.GetComponentLocations();
    for (auto& comp : comps) {
        string name = comp.first;
        size_t index = comp.second;

        //free up for insertion. Do this by supplying the component's
        //indices for use by a new component, and marking the component as not in use.
        if (name == componentVectorNames[0]) { //movement
            movements[index]->IsActive = false;
            movementSlots.push(index); 
        }
        else if (name == componentVectorNames[1]) { //transform
            transforms[index]->IsActive = false;
            transformSlots.push(index);
        } 
        else if (name == componentVectorNames[2]) { //collision
            movements[index]->IsActive = false;
            collisionSlots.push(index);
        }
        else if (name == componentVectorNames[3]) { //render
            renderers[index]->IsActive = false;
            rendererSlots.push(index);
        }
        else if (name == componentVectorNames[4]) { //collect
            collects[index]->IsActive = false;
            collectSlots.push(index);
        }
        //TODO add additional potential components.
    }//end processing component vector freeing

    //no longer referenced anywhere, delete from map.
    objects.erase(name); 
}

void ComponentManager::Cleanup()
{
}
