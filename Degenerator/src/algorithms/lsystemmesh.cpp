#include "lsystemmesh.h"
#include "Entropy/src/Utility/Math.h"
#include "Entropy/src/Fractal/LSystem.h"
#include "Entropy/src/Utility/geometry.h"
#include "Entropy/src/Utility/Vector.h"
#include "Entropy/src/Utility/Array.h"
#include "meshfunctions.h"

using namespace fractal;
using namespace utility::math;

namespace Degenerator {

LSystemMesh::LSystemMesh()
{

}

AStruct LSystemMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    // F = Forward
    // + = right turn
    // - = left turn
    // ^ = turn up
    // v = turn down
    // [ = push matrix
    // ] = pop matrix

    rule_set ruleSet;
    // old rule: "F+[+F-F-F]-[-F+F+F]"
    std::string currentRule("F+[+F-F^F]-[vF+F+F]");
    int seed = 0;

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case LRule:
            currentRule = params[i];
            break;

        case LSeed:
            seed = atoi(params[i].c_str());
            break;
        }
    }

    std::cout << "Rule: " << currentRule << std::endl;
    std::cout << "Seed: " << seed << std::endl;

    srand(seed);

    ruleSet.push_back(Rule(fractal::alphabet[randomRange(0, 5)], currentRule));
    currentRule.clear();

    for(int i = 0; i < randomRange(2, 5); ++i)
    {
        for(int j = 0; j < randomRange(2, 10); ++j)
        {
            currentRule.append(1, fractal::alphabet[randomRange(0, 5)]);
        }

        ruleSet.push_back(Rule(fractal::alphabet[randomRange(0, 5)], currentRule));
        currentRule.clear();
    }

    currentRule.append(&fractal::alphabet[randomRange(0, 5)]);

    for(int j = 0; j < randomRange(2, 10); ++j)
    {
        currentRule.append(1, fractal::alphabet[randomRange(0, 5)]);
    }

    fractal::LSystem lSystem(currentRule, ruleSet);

    std::string sentence;
    int sentenceLength;
    //std::cout << "L-System string length: " << sentenceLength << std::endl;

    geometry::Vec3d coord(0, 0, 0);
    geometry::Vec3d minCoord, maxCoord;
    minCoord = maxCoord = coord;
    geometry::Vec3d direction(randomRange(-1, 1), randomRange(-1, 1), (-1, 1));

    std::vector< std::pair<geometry::Vec3d, geometry::Vec3d> > stack;
    stack.push_back(std::make_pair(coord, direction));

    std::vector<geometry::Vec3d> history;

    if(direction == geometry::Vec3d(0, 0, 0))
        direction = geometry::Vec3d(0, 1, 0);

    history.push_back(coord);

    int length;

    for(int i = 4; i >= 0; --i)
    {
        lSystem.generate();
        sentence = lSystem.getSentence();
        sentenceLength = sentence.length();
        length = pow(2.0, i + 1) / 2;

        for(int j = 0; j < sentenceLength; ++j)
        {
            char c = sentence.at(j);

            switch(c)
            {
            case 'F':

                for(int k = 0; k < length; ++k) // draw a line as along as our length
                {

                    coord = coord + direction;
                    // std::cout << "coord(" << coord.x << ", " << coord.y << ", " << coord.z << ")" << std::endl;
                    history.push_back(coord);

                    if(coord.x < minCoord.x)
                        minCoord.x = coord.x;
                    if(coord.y < minCoord.y)
                        minCoord.y = coord.y;
                    if(coord.z < minCoord.z)
                        minCoord.z = coord.z;

                    if(coord.x > maxCoord.x)
                        maxCoord.x = coord.x;
                    if(coord.y > maxCoord.y)
                        maxCoord.y = coord.y;
                    if(coord.z > maxCoord.z)
                        maxCoord.z = coord.z;
                }

                break;

            case 'G':

                for(int k = 0; k < length; ++k) // draw a line as along as our length
                {

                    coord += direction;
                    // std::cout << "coord(" << coord.x << ", " << coord.y << ", " << coord.z << ")" << std::endl;
                    history.push_back(coord);

                    if(coord.x < minCoord.x)
                        minCoord.x = coord.x;
                    if(coord.y < minCoord.y)
                        minCoord.y = coord.y;
                    if(coord.z < minCoord.z)
                        minCoord.z = coord.z;

                    if(coord.x > maxCoord.x)
                        maxCoord.x = coord.x;
                    if(coord.y > maxCoord.y)
                        maxCoord.y = coord.y;
                    if(coord.z > maxCoord.z)
                        maxCoord.z = coord.z;
                }

                break;

            case '+':

                // Rotate clockwise
                direction.rotateY(PI);

                // std::cout << "+" << " Direction(" << direction.x << ", " << direction.y << ", " << direction.z << ")"
                //          << std::endl;

                break;

            case '-':

                // Rotate counter clockwise
                direction.rotateY(-PI);

                // std::cout << "-" << " Direction(" << direction.x << ", " << direction.y << ", " << direction.z << ")"
                //          << std::endl;

                break;

            case '^': // Turn up
                direction.rotateX(PI);
                // std::cout << "^" << " Direction(" << direction.x << ", " << direction.y << ", " << direction.z << ")"
                //           << std::endl;
                break;

            case 'v': // turn down
                direction.rotateX(-PI);
                // std::cout << "v" << " Direction(" << direction.x << ", " << direction.y << ", " << direction.z << ")"
                //          << std::endl;
                break;

            case '[': // Push Matrix
                // std::cout << "[";
                stack.push_back(std::make_pair(coord, direction));
                break;

            case ']': // Pop Matrix
                // std::cout << "]";
                if(stack.size() > 0)
                {
                    coord = stack.back().first;
                    direction = stack.back().second;
                    stack.pop_back();
                }

                break;

            default:
                std::cout << "Default Char: " << c << std::endl;
            }
        }
    }


    geometry::Vec3d size = maxCoord - minCoord;
    geometry::Array3D grid(size.x + 1, size.y + 1, size.z + 1);
    // std::cout << "Array Size(" << size.x << ", " << size.y << ", " << size.z << ")" << std::endl;
    // std::cout << "maxCoord(" << maxCoord.x << ", " << maxCoord.y << ", " << maxCoord.z << ")" << std::endl;
    // std::cout << "minCoord(" << minCoord.x << ", " << minCoord.y << ", " << minCoord.z << ")" << std::endl;

    for(int i = 0; i < history.size(); ++i)
    {
        geometry::Vec3d offsetCoord = history[i] - minCoord;
        // std::cout << "offsetCoord(" << offsetCoord.x << ", " << offsetCoord.y << ", " << offsetCoord.z << ")" << std::endl;
        grid[offsetCoord.x][offsetCoord.y][offsetCoord.z] = 1;
    }

    int cubeSize = 1;
    geometry::Vec3d position(0, 0, 0);
    position -= ((size / 2) / 3.0);
    Ogre::Vector3 ogrePosition(position.x, position.y, position.z);
    std::vector<Ogre::Vector3> cubeMesh;
    std::vector<Ogre::Vector3> cubeNormals;
    std::vector<Ogre::Vector2> texCoords;
    int indices = 0;

    for(int x = 0; x < grid.width; ++x)
    {
        for(int y = 0; y < grid.height; ++y)
        {
            for(int z = 0; z < grid.depth; ++z)
            {
                //cubeMesh.clear();
                //cubeNormals.clear();
                //texCoords.clear();

                if(grid[x][y][z] == 1)
                {
                    Ogre::Vector3 ogreCurrentPosition(x / 3.0, y / 3.0, z / 3.0);
                    cube(ogrePosition + ogreCurrentPosition, cubeSize, astruct);
                }

                /*
                cube(
                            ogrePosition + ogreCurrentPosition,
                            cubeSize,
                            cubeMesh,
                            cubeNormals,
                            texCoords
                );

                for(int i = 0; i < cubeMesh.size(); ++i)
                {
                    astruct.indices.push_back(indices);
                    astruct.normals.push_back(cubeNormals.at(i));
                    astruct.texCoords.push_back(texCoords.at(i));
                    astruct.vertices.push_back(cubeMesh.at(i));
                    //std::cout << "cubeVector(" << cubeMesh[i].x << ", " << cubeMesh[i].y << ", "
                    //          << cubeMesh[i].z << ")" << std::endl;
                    ++indices;
                }*/
            }
        }
    }

    /*
    cube(
                Ogre::Vector3(0, 0, 0),
                20,
                cubeMesh,
                cubeNormals,
                texCoords
    );

    for(int i = 0; i < cubeMesh.size(); ++i)
    {
        astruct.indices.push_back(indices);
        astruct.normals.push_back(cubeNormals.at(i));
        astruct.texCoords.push_back(texCoords.at(i));
        astruct.vertices.push_back(cubeMesh.at(i));
        //std::cout << "cubeVector(" << cubeMesh[i].x << ", " << cubeMesh[i].y << ", "
        //          << cubeMesh[i].z << ")" << std::endl;
        ++indices;
    }*/

    //cube(ogrePosition, 20.0, astruct);

    astruct.mode = TriangleList;
    return astruct;
}

} // Degenerator namespace
