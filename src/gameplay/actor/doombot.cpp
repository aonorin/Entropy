#include "gameplay/actor/doombot.h"
#include "math.h"
#include "OGRE/OgreManualObject.h"
#include "Core/collisioncamera.h"
#include "btBulletCollisionCommon.h"

using namespace hypnos;

void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16)
{
    Ogre::ManualObject * manual = Entropy::getSingletonPtr()->mSceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    float fDeltaRingAngle = (Ogre::Math::PI / nRings);
    float fDeltaSegAngle = (2 * Ogre::Math::PI / nSegments);
    unsigned short wVerticeIndex = 0 ;

    // Generate the group of rings for the sphere
    for( int ring = 0; ring <= nRings; ring++ ) {
        float r0 = r * sinf (ring * fDeltaRingAngle);
        float y0 = r * cosf (ring * fDeltaRingAngle);

        // Generate the group of segments for the current ring
        for(int seg = 0; seg <= nSegments; seg++) {
            float x0 = r0 * sinf(seg * fDeltaSegAngle);
            float z0 = r0 * cosf(seg * fDeltaSegAngle);

            // Add one vertex to the strip which makes up the sphere
            manual->position( x0, y0, z0);
            manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
            manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);

            if (ring != nRings) {
                // each vertex (except the last) has six indicies pointing to it
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex);
                manual->index(wVerticeIndex + nSegments);
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex + 1);
                manual->index(wVerticeIndex);
                wVerticeIndex ++;
            }
        }; // end for seg
    } // end for ring
    manual->end();
    Ogre::MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

    mesh->_setBoundingSphereRadius(r);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
    }
}

DoomBot::DoomBot(Ogre::Vector3 pos) :
    EnemyActor(
        pos,
        physics::createCollisionSphere(pos,1,1),
        "DoomBot",
        entropy::Flier
        )
{
    std::cout << "DOOMBOT CREATED!" << std::endl;
    createSphere("DoomBotSphere",1);
    Ogre::Entity* ent = Entropy::getSingletonPtr()->mSceneMgr->createEntity(name.append("Entity"),"DoomBotSphere");
    setEntity(ent);
    setTarget(Entropy::getSingletonPtr()->mCameraMan->getPathID());
    //    btRigidBody::upcast(object)->setAngularFactor(0.0);
}

void DoomBot::move(Ogre::Vector3 nextPosition)
{
    //    std::cout << "moving to: " << nextPosition << std::endl;
    Ogre::Vector3 vector =  nextPosition - ogreNode->getPosition();
    vector.normalise();
    vector = vector*6;

    btRigidBody::upcast(object)->activate(true);
    btVector3 currentVelocity = btRigidBody::upcast(object)->getLinearVelocity();
    btRigidBody::upcast(object)->setDamping(0.975,1);
    float maxSpeed = 6;
    float minSpeed = -6;
    btRigidBody::upcast(object)->setGravity(btVector3(0,0,0));
    btVector3 velocity(0,0,0);

//    if(currentVelocity.x()+vector.x<=maxSpeed && currentVelocity.x()+vector.x>=minSpeed)
//    {
            velocity.setX(std::min(std::max(currentVelocity.x()+vector.x,minSpeed),maxSpeed));
//    }
//    if(currentVelocity.y()+vector.y<=maxSpeed && currentVelocity.y()+vector.y>=minSpeed)
//    {
            velocity.setY(std::min(std::max(currentVelocity.y()+vector.y,minSpeed),maxSpeed));
//    }
//    if(currentVelocity.z()+vector.z<=maxSpeed && currentVelocity.z()+vector.z>=minSpeed)
//    {
            velocity.setZ(std::min(std::max(currentVelocity.z()+vector.z,minSpeed),maxSpeed));
//    }

    btRigidBody::upcast(object)->applyCentralForce(velocity);
}

void DoomBot::constructDoomBot()
{
    std::cout << "Constructing Doom bot" << std::endl;
    Ogre::Vector3 pos(
                Entropy::getSingletonPtr()->mCameraMan->getPosition().x()+10,
                Entropy::getSingletonPtr()->mCameraMan->getPosition().y(),
                Entropy::getSingletonPtr()->mCameraMan->getPosition().z()
                );
    gameplay::actors::addActor(new DoomBot(pos));
}
