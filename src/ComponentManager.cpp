#include "ComponentManager.h"
#include "TerrainRenderer.h"
#include "PlayerTransform.h"
#include "EulerTransform.h"
#include "MonkeyMovement.h"
#include "SkyboxRenderer.h"
#include "HeadRenderer.h"
#include "DroneRenderer.h"
#include "DroneManager.h"

constexpr float ParticleRenderer::originalPointSize;

ComponentManager::ComponentManager()
{
    //specify what you want the components to be called here.
    components["Movement"];
    components["Transform"];
    components["Collision"];
    components["Renderer"];
    components["Collect"];
    components["Particle"];
    components["DroneManager"];
}

GameObject ComponentManager::GetGameObject(string name)
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
    auto it = components.find(compName);
    if (it == components.end()) {
        std::cerr << "GetComponent with name: " << compName << " was not found in components list." << std::endl;
        throw std::runtime_error("Component not found");
    }
    return it->second[index];
}

void ComponentManager::Init(std::string resourceDirectory, AudioEngine* audioPtr)
{
    // initialize skybox
    string skyboxName = "Skybox";
    shared_ptr<SkyboxRenderer> skyboxRenderer = make_shared<SkyboxRenderer>(skyboxName, "unit_cube");
    shared_ptr<Transform> transform = make_shared<Transform>(skyboxName);
    transform->SetPos(vec3(0, 0, 0));
    transform->SetScale(vec3(100, 100, 100));
    vector<shared_ptr<Component>> skyboxComps = { skyboxRenderer, transform };
    AddGameObject(skyboxName, skyboxComps);

    // initialize the player
    shared_ptr<EulerTransform> pTransform = make_shared<EulerTransform>(player.pName);
    transform = pTransform;
    //shared_ptr<Renderer> renderer = make_shared<TextureRenderer>("LUNA/luna_body", "Luna", player.pName);
    shared_ptr<Renderer> renderer = make_shared<LunaBodyRenderer>("LUNA/new/lunaModelTextures/body", "Luna", player.pName);
    //shared_ptr<ParticleRenderer> particles = make_shared<ParticleRenderer>("SandPartTex", "Sand", player.pName, 100000, &ParticleRenderer::drawSand);
    shared_ptr<Movement> playerMovement = make_shared<PlayerMovement>(player.pName);
    std::vector<std::shared_ptr<Component>> playerComps = { transform, renderer, playerMovement};
    transform->SetPos(vec3(0, 1, 2));
    transform->SetScale(vec3(.5));
    AddGameObject(player.pName, playerComps);

    // initialize body parts as separate objects
    shared_ptr<PlayerTransform> headTrans = make_shared<PlayerTransform>(player.pHeadName, transform);
    //shared_ptr<Renderer> renderer = make_shared<TextureRenderer>("LUNA/luna_body", "Luna", player.pName);
    shared_ptr<HeadRenderer> hrenderer = make_shared<HeadRenderer>("LUNA/new/lunaModelTextures/head", "Luna", player.pHeadName);
    std::vector<std::shared_ptr<Component>> headComps = { headTrans, hrenderer };
    AddGameObject(player.pHeadName, headComps);

    shared_ptr<PlayerTransform> arm1Trans = make_shared<PlayerTransform>(player.pArm1Name, transform);
    //shared_ptr<Renderer> renderer = make_shared<TextureRenderer>("LUNA/luna_body", "Luna", player.pName);
    renderer = make_shared<LunaBodyRenderer>("LUNA/new/lunaModelTextures/right_arm", "Luna", player.pArm1Name);
    std::vector<std::shared_ptr<Component>> arm1Comps = { arm1Trans, renderer };
    AddGameObject(player.pArm1Name, arm1Comps);

    shared_ptr<PlayerTransform> arm2Trans = make_shared<PlayerTransform>(player.pArm2Name, transform);
    //shared_ptr<Renderer> renderer = make_shared<TextureRenderer>("LUNA/luna_body", "Luna", player.pName);
    renderer = make_shared<LunaBodyRenderer>("LUNA/new/lunaModelTextures/left_arm", "Luna", player.pArm2Name);
    std::vector<std::shared_ptr<Component>> arm2Comps = { arm2Trans, renderer };
    AddGameObject(player.pArm2Name, arm2Comps);

    player.Init(this, pTransform, headTrans, arm1Trans, arm2Trans, hrenderer);

    //initialize random starting attributes for particles. Generate a few batches of 20k particles.
    ParticleRenderer::gpuSetup(ShaderManager::GetInstance().GetShader("particle"), 20000);

    //initialize starting lights.
    state.InitLights();

    InitDroneManager(resourceDirectory);
    // Initialize the GLSL program, just for the ground plane.
    auto prog = make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
    prog->Init();
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addAttribute("vertPos");
    
    //these generate random things between the two values. Currently supports floats and vec3's, but is easy to add to.
    RandomGenerator randMove(-10, 10);
    RandomGenerator randTrans(-40, 40);
    RandomGenerator randScale(0.2, 2);


    string floorName = "Floor";
    renderer = make_shared<TerrainRenderer>("Cat", "Cat", floorName);
    transform = make_shared<Transform>(floorName);
    transform->SetPos(vec3(50, -4, -50));
    vector<shared_ptr<Component>> floorComps = { renderer, transform };
    AddGameObject(floorName, floorComps);

    audio = audioPtr;
}

void ComponentManager::InitDroneManager(std::string resourceDirectory)
{
    string managerName = "Drone Manager";
    shared_ptr<Renderer> renderer = make_shared<DroneRenderer>("drone_rockie/Robot_Rockie", 
        "droneBase", "droneEmiss", "droneNormal", managerName);
    shared_ptr<Transform> transform = make_shared<Transform>(managerName);
    shared_ptr<DroneManager> manager = make_shared<DroneManager>(managerName);
    //where all the variables at the top come in.
    vec3 pos = vec3(0.0f);

    //finally, calculate each spawned fragment's height at this position.
    transform->ApplyTranslation(vec3(0.0f, HeightCalc::heightCalc(0.0f, 0.0f), 0.0f));
    vector<shared_ptr<Component>> managerComps = { renderer, transform, manager };
    AddGameObject(managerName, managerComps);
}

void ComponentManager::AddLineOfStars()
{
    int numStarsToSpawn = (rand() % 12) + 1; //1-12 stars spawning
    RandomGenerator randTrans(-10, 10); //generate a position offset from the player's right-vector
    
    float offsetRight = randTrans.GetFloat();    //get some number between -4 and 4
    float distFactor = 150.0f; //how far away from the player, in world space units, do we start spawning star fragments
    float spacing = 4.0f; //the distance between each star fragment, if multiple are spawned.
    vec3 playerGaze = normalize(player.GetForward()); //double check that this is normalized.
    vec3 playerRight = cross(playerGaze, vec3(0, 1, 0));

    //start initializing stars.
    for (int i = 0; i < numStarsToSpawn; ++i){
        string sphereName = "Star Bit" + to_string(state.TotalObjectsEverMade);
        string sphereShapeFileName = "Star Bit";
        shared_ptr<Renderer> renderer = make_shared<StarRenderer>(sphereShapeFileName, "Rainbow", "Star", sphereName);
        shared_ptr<Renderer> particles = make_shared<ParticleRenderer>("Alpha", "particle", sphereName, 20000, ParticleRenderer::originalPointSize, 5, &ParticleRenderer::drawSplash);
        shared_ptr<Transform> transform = make_shared<Transform>(sphereName);
        shared_ptr<Collision> collision = make_shared<Collision>(sphereName, sphereShapeFileName);
        shared_ptr<Collect> collect = make_shared<Collect>(sphereName);

        //where all the variables at the top come in.
        vec3 pos = player.GetPosition() + //the player's position
            playerGaze * (distFactor + (spacing * i)) +  //plus distFactor units in front of the player, plus an additional spacing for each one spawned.
            playerRight * offsetRight; //plus shifted to the /left or right a random number, which is the same for all star fragments in this particular line.
            
        //finally, calculate each spawned fragment's height at this position.
        transform->ApplyTranslation(vec3(pos.x, HeightCalc::heightCalc(pos.x, pos.z), pos.z));
        transform->ApplyScale(vec3(0.02f));
        vector<shared_ptr<Component>> sphereComps = { renderer, particles, transform, collision, collect };
        AddGameObject(sphereName, sphereComps);
        state.TotalObjectsEverMade++;
        //cout << "\nAdded one more star!\n";
    }
}

void ComponentManager::AddBunchOfSandParticles() {
    int numSandToSpawn = 1;
    RandomGenerator randTrans(-20, 20); //generate a position offset from the player's right-vector
    float offsetRight = randTrans.GetFloat(); //get some number
    vec3 playerGaze = normalize(player.GetForward()); //double check that this is normalized.
    vec3 playerRight = cross(playerGaze, vec3(0, 1, 0));
    float distFactor = 20;
    
    //start initializing stars.
    for (int i = 0; i < numSandToSpawn; ++i) {
        string SandName = "Sand" + to_string(state.TotalObjectsEverMade);
        
        shared_ptr<ParticleRenderer> particles = make_shared<ParticleRenderer>("SandPartTex", "Sand", SandName, 1, 80, FLT_MAX, &ParticleRenderer::drawSand);
        shared_ptr<Transform> transform = make_shared<Transform>(SandName);

        //where all the variables at the top come in.
        vec3 pos = player.GetPosition(); //the player's position
        particles->initialPlayerSpeed = player.GetCurrentSpeed();
        particles->initialPlayerDirection = player.GetVelocity();
        //finally, calculate each spawned fragment's height at this position.
        transform->ApplyTranslation(vec3(pos.x, HeightCalc::heightCalc(pos.x, pos.z), pos.z));
        transform->ApplyScale(vec3(0.02f));
        vector<shared_ptr<Component>> Comps = { particles, transform };
        AddGameObject(SandName, Comps);
        state.TotalObjectsEverMade++;
        //cout << "\nAdded one more star!\n";
    }
}

// Iterate through all of the component vectors. Usually call Update on all of them, although
// there are some differences.
//
//What this does, is separate the game behavior into stages. You can be sure that if a component 
//needs to access data from a different component type, all of those different component types are
//synchronized to the same frame. 
void ComponentManager::UpdateComponents(float frameTime, int width, int height)
{
    //Timer timer;
    //the first thing that should happen in every frame. Stores total time.
    state.IncTotalFrameTime(frameTime);
    if (state.ShouldSpawnStar()) {
        AddLineOfStars();
    }
    
    if (state.ShouldSpawnSand()) {
        AddBunchOfSandParticles();
    }

    // update movements
    for (auto& move : components["Movement"])
    {
        if (!move->IsActive) continue;
        move->Update(frameTime, this);
    }
    //int mvtime = timer.stop();
    //resolve collisions.
    //timer.start();
    for (auto& giver : components["Collision"])
    {
        if (!giver->IsActive) continue;  //don't collide with destroyed objects.
        giver->Update(frameTime, this);
    }
    //int colitime = timer.stop();
    //timer.start();
    // update transforms based on movements.
    for (auto& trans : components["Transform"])
    {
        if (!trans->IsActive) continue;
        trans->Update(frameTime, this);
    }
    //int transtime = timer.stop();
    //timer.start();
    // update flashing animation
    for (auto& collect : components["Collect"])
    {
        if (!collect->IsActive) continue;
        collect->Update(frameTime, this);
    }

    for (auto& droneManager : components["DroneManager"])
    {
        if (!droneManager->IsActive) continue;
        droneManager->Update(frameTime, this);
    }

    //int coletime = timer.stop();
    //timer.start();
    // update the player
    player.Update(frameTime, this);
    
    // update the camera
    camera.Update(frameTime, width, height, this);

    // update lights
    lightComponent.Update(frameTime, this);
    //int pcamlight = timer.stop();
    //finally update renderers/draw.

    //timer.start();
    for (auto& rend : components["Renderer"])
    {
        if (!rend->IsActive) continue; //if the component is active (isn't awaiting replacement in the component vector structure)
        if (!static_pointer_cast<Renderer>(rend)->IsInViewFrustum(state, this, camera)){ //if the renderer component has culling enabled and is actually outside the view frustum.
            continue;
        }
        //else
        rend->Update(frameTime, this);
    }
    //int rendtime = timer.stop();
    //draw particles last because they are transparent.
    //timer.start();
    for (auto& part : components["Particle"])
    {
        if (!part->IsActive) continue;
        part->Update(frameTime, this);
        
    }
    //sort("Particle");
    for (auto& part : components["Particle"])
    {
        const auto& rend = static_pointer_cast<ParticleRenderer>(part);
        if (rend->IsActive && rend->IsInViewFrustum(state, this, camera)) {
            rend->Draw(frameTime);
        }
    }
}

void ComponentManager::sort(std::string compName)
{
    vec3 s, t, sk;
    vec4 p;
    quat r;
    glm::decompose(camera.GetView(), s, r, t, sk, p);
    partComponentSorter.C = glm::toMat4(r);
    //sort transparent objects, according to their world position and the camera's position. This requires an update to a GameObject's Particle index!
    
    std::sort(components[compName].begin(), components[compName].end(), [this](const shared_ptr<Component>& c1, const shared_ptr<Component>& c2) 
        {return partComponentSorter(*static_pointer_cast<ParticleRenderer>(c1).get(), *static_pointer_cast<ParticleRenderer>(c2).get()); }); //needs a comparator-like object for each component. Parameterize this if needed.
    //there are now potentially corrupt GameObject indices to particle components. Use the unique name to resolve them.
    for (int i = 0; i < components[compName].size(); i++) {
        if (!components[compName][i]->IsKilled) {
            objects[components[compName][i]->Name].SetComponentLocation(compName, i);
        }
    }
    //now we need to resolve the fact that the open slots may have changed as well.
    recalculateOpenSlots(compName);
    //Then finally draw them, in order.
}

void ComponentManager::AddGameObject(string name, vector<shared_ptr<Component>> comps)
{
    //unordered_map<type_info*, size_t> componentList;
    //don't care what container is used to pass in components,
    //Pad unused components with null.
    unordered_map<string, size_t> objComps;
    for (const auto& comp : comps) {
        if (!comp) continue; //null check
        //if not null
        auto p = addToComponentList(comp);
        objComps[p.first] = p.second;
        objComps.insert(p);
    }
    objects[name] = GameObject(name, objComps);
    //call Init on all of a GameObject's components, now that they are initialized.
    for (auto& comp : objects[name].GetComponentLocations()) {
        string name = comp.first;
        size_t index = comp.second;
        GetComponent(name, index)->Init(this);
    }//end processing component vector freeing
}

//add the component to the first-available position of the corresponding vector of components.
pair<string, size_t> ComponentManager::addToComponentList(const shared_ptr<Component>& comp) {
    //these series of checks check whether an attempted cast returns a nullptr, indicating that
    //a cast could/couldn't take place, effectively type-checking the object.
    int index = -1;
    string compType = "undefinedComponentType"; //make it really obvious if not detected.
    shared_ptr<Component> ptr;
    if (nullptr != (ptr = dynamic_pointer_cast<Movement>(comp))) {
        //type name here, same as in constructor.
        compType = "Movement";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<Transform>(comp))) {
        compType = "Transform";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<Collision>(comp))) {
        compType = "Collision";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<ParticleRenderer>(comp))) {
        compType = "Particle";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<Renderer>(comp))) {
        compType = "Renderer";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<Collect>(comp))) {
        compType = "Collect";
    }
    else if (nullptr != (ptr = dynamic_pointer_cast<DroneManager>(comp))) {
        compType = "DroneManager";
    }
    
    //TODO the other concrete types. Format should be pretty much identical.
    
    if (compType == "undefinedComponentType" || components.find(compType) == components.end()) {
        cerr << "new components should have a similar else-if block, that casts to the "
             << "concrete type before insertion into the component vector." << endl;
        throw std::runtime_error("index returned: " + to_string(index) + ", compType returned: " + compType);
    }

    index = getNextOpenSlot(componentOpenSlots[compType]);
    addHelper(ptr, components[compType], index);

    // finally(this should happen at the end, unconditionally, for all component types :
    //return the index where the component resides now.
    return make_pair(compType, index); //insert this into the GameObject.
}

//This modifies compList by inserting comp.
template<typename CONCRETE, typename ABSTRACT>
void ComponentManager::addHelper(shared_ptr<CONCRETE> comp, vector<shared_ptr<ABSTRACT>>& compList, int& index) {
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

//this accounts for the possibility that a killed particle system component could be swapped with an active particle system component during sorting.
//Recalculates the open slots available for a given component vector.
void ComponentManager::recalculateOpenSlots(const std::string& componentVectorName)
{
    OpenSlots& slots = componentOpenSlots[componentVectorName]; //we will be adjusting this
    const vector<std::shared_ptr<Component>>& componentVector = components[componentVectorName]; //based on iterating through this
    //clear the memory.
    slots.clear();
    //add all inactive slots back to the queue
    for (size_t i = 0; i < componentVector.size(); i++) {
        if (componentVector[i]->IsKilled) { 
            slots.push(i); //isKilled is more restrictive then isActive. Only push if you are sure it's not being used.
        }
    }
}

void ComponentManager::RemoveGameObject(string name)
{
    //make a copy, don't actually remove from map until its components are all gone
    GameObject obj = GetGameObject(name);
    assert(name == obj.Name); //DEBUG quick sanity check
    unordered_map<string, size_t> comps = obj.GetComponentLocations();
    for (auto& comp : comps) {
        string name = comp.first;
        size_t index = comp.second;

        //free up for insertion. Do this by supplying the component's
        //indices for use by a new component, and marking the component as not in use.
        GetComponent(name, index)->IsActive = false;
        GetComponent(name, index)->IsKilled = true;
        componentOpenSlots[name].push(index);
        
    }//end processing component vector freeing

    //no longer referenced anywhere, delete from map.
    objects.erase(name); 
}

void ComponentManager::Cleanup()
{
}
