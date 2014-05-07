#include "corruptedsphere.h"

static int PI = 3.141592654;

namespace Degenerator {

CorruptedSphere::CorruptedSphere()
{
}

AStruct CorruptedSphere::generateMesh(std::vector<std::string> params)
{
    float r,h,k,z;
    r = atoi(params.at(0).c_str());//around 25
    h = atoi(params.at(1).c_str());//around 0
    k = atoi(params.at(2).c_str());//around 0
    z = atoi(params.at(3).c_str());//around 0
    int sphereSpace = atoi(params.at(4).c_str());//around 20
    int aMax = atoi(params.at(5).c_str());//around 180
    int bMax = atoi(params.at(6).c_str());//around 180

    double coruption = ((float)atoi(params.at(7).c_str()))/1000.0;//around 1000
    double coruption2 = ((float)atoi(params.at(8).c_str()))/1000.0;//around 1000
    double coruption3 = ((float)atoi(params.at(9).c_str()));//around 1000
    double coruption4 = ((float)atoi(params.at(10).c_str()));//around 1000

    int mode = atoi(params.at(11).c_str());//around 180
    AStructPrimitive modePrimitive;

    if(mode == 0) modePrimitive = TriangleList;
    else modePrimitive = TriangleStrip;

    int n;
    double a;
    double b;
    n = 0;

    float size = 1.0;
//    std::cout << osc2.oscillate() << std::endl;
    std::vector<Ogre::Vector3> positions;
    std::vector<Ogre::Vector3> normals;
    std::vector<Ogre::Vector2> texCoords;
    std::vector<int> indices;
    int indexCount = 0;
    //Put oscillators on every 180 thing.
    for( b = 0; b <=aMax; b+=sphereSpace){
        for( a = 0; a <= bMax; a+=sphereSpace){
            Ogre::Vector3 pos = Ogre::Vector3(0,0,0);
            Ogre::Vector3 normal = Ogre::Vector3(0,0,0);
            Ogre::Vector2 texCoord = Ogre::Vector2(0,0);

            pos.x = r/coruption * sin((a+coruption3) / (180) * PI) * sin((b) /180 * PI) - h;
            pos.y = r * cos((a) / 180 *  PI)/coruption2 * sin((b+coruption4) /180 * PI) + k;
            pos.z = r * cos((b+coruption4) / 180 * PI) - z;
            normal.x = pos.x - h;
            normal.y = pos.y - k;
            normal.z = pos.z - z;
            positions.push_back(pos);
            normal.normalise();
            normals.push_back(normal);
            texCoord.x = (sin((a+coruption3) / (180) * PI) * sin((b) / 180 * PI))*size;
            texCoord.y = (cos((a) /180 * PI) * sin((b+coruption4) /180 * PI))*size;
            texCoords.push_back(texCoord);
            indices.push_back(indexCount);
            indexCount++;

            pos.x = r * sin((a) / 180 * PI) * sin((b + sphereSpace+coruption4) /  180 * PI) - h;
            pos.y = r/coruption * cos((a+coruption3) / (180) * PI) * sin((b + sphereSpace) /  180 * PI) + k;
            pos.z = r * cos((b + sphereSpace+coruption3) / 180 * PI)/coruption2 - z ;
            normal.x = pos.x - h;
            normal.y = pos.y - k;
            normal.z = pos.z - z;
            positions.push_back(pos);
            normal.normalise();
            normals.push_back(normal);
            texCoord.x = (sin((a) / 180 * PI) * sin((b + sphereSpace+coruption4) / 180 * PI))*size;
            texCoord.y = (cos((a+coruption3) / (180) * PI) * sin((b + sphereSpace) / 180 * PI))*size;
            texCoords.push_back(texCoord);
            indices.push_back(indexCount);
            indexCount++;

            pos.x = r * sin((a + sphereSpace) / 180 * PI)/coruption2 * sin((b+coruption3) / (180) * PI) - h;
            pos.y = r * cos((a + sphereSpace+coruption4) / 180 * PI) * sin((b) / 180 * PI) + k;
            pos.z = r/coruption * cos((b+coruption4) / 180 * PI) + z;
            normal.x = pos.x - h;
            normal.y = pos.y - k;
            normal.z = pos.z + z;
            positions.push_back(pos);
            normal.normalise();
            normals.push_back(normal);
            texCoord.x = (sin((a + sphereSpace) / 180 * PI) * sin((b+coruption3) / (180) * PI))*size;
            texCoord.y = (cos((a + sphereSpace+coruption4) / 180 * PI)  * sin((b) / 180 * PI))*size;
            texCoords.push_back(texCoord);
            indices.push_back(indexCount);
            indexCount++;

            pos.x = r * sin((a + sphereSpace+coruption4) / 180 * PI) * sin((b + sphereSpace) / 180 * PI) - h;
            pos.y = r * cos((a + sphereSpace) / 180 * PI)/coruption2 * sin((b + sphereSpace+coruption3) / (180) * PI) + k;
            pos.z = r * cos((b + sphereSpace+coruption3) / 180 * PI) + z;
            normal.x = pos.x - h;
            normal.y = pos.y - k;
            normal.z = pos.z - z;
            positions.push_back(pos);
            normal.normalise();
            normals.push_back(normal);
            texCoord.x = (sin((a + sphereSpace+coruption4) / 180 * PI) * sin((b + sphereSpace) / 180 * PI))*size;
            texCoord.y = (cos((a + sphereSpace) / 180 * PI)  * sin((b + sphereSpace+coruption3) / (180) * PI))*size;
            texCoords.push_back(texCoord);
            indices.push_back(indexCount);
            indexCount++;
        }
    }

    indices.clear();
    AStruct aStruct = {positions,indices,texCoords,normals,modePrimitive};

    return aStruct;
}

}
