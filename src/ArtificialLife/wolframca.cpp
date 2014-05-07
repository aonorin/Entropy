#include <iostream>
#include <set>
#include <algorithm>

#include "wolframca.h"

namespace alife {

namespace cell {


WolframCA::WolframCA(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int size):
    AbstractStream(ruleSet, possibleStates, initialState, size)
{

}

void WolframCA::advance()
{
    if(possibleStates == 2) // Binary states
    {
        for(int i = 0; i < futureState.size(); ++i) // iterate over the values to produce the next generation
        {
            int neighbors[3]; // Create the neighbor array, then populate it

            if(i - 1 < 0)
                neighbors[2] = 0;
            else
                neighbors[2] = currentState[i - 1];

            neighbors[1] = currentState[i];

            if(i + 1 >= currentState.size())
                neighbors[0] = 0;
            else
                neighbors[0] = currentState[i + 1];

            int index = 0; // Used to index the ruleArray

            // convert to an index
            for(int j = 0; j < 3; ++j)
            {
                index += neighbors[j] << j;
            }

            index = abs((int)ruleArraySize - 1 - index);
            futureState[i] = ruleArray[index];
        }
    }

    else
    {
        for(int i = 0; i < futureState.size(); ++i)
        {
            double average = 0;

            if(i - 1 >= 0)
                average += currentState[i - 1];

            average += currentState[i];

            if(i + 1 < currentState.size())
                average += currentState[i + 1];

            average /= 3.0;
            futureState[i] = ruleMap[average];
        }
    }

    AbstractStream::advance(); // Remember to call parent's advance when we're done
}

///////////////
// WolframCA3D
///////////////

WolframCA3D::WolframCA3D(unsigned int ruleSet, unsigned int possibleStates, unsigned int initialState, unsigned int width,
                         unsigned int depth) :
    AbstractAutomata(ruleSet, possibleStates),
    width(width),
    depth(depth)
{
    if(width % 2 == 0) // if even, make odd
    {
        this->width = width - 1;
    }

    if(depth % 2 == 0)
    {
        this->depth = depth - 1;
    }

    createRuleArray();

    futureState.resize(width, std::vector<unsigned int>(depth));

    int initialX = ((double) width / 2.0) - 0.5;
    int initialZ = ((double) depth / 2.0) - 0.5;

    futureState[initialX][initialZ] = initialState;
    currentState = futureState;
}

void WolframCA3D::advance()
{
    if(possibleStates == 2) // Binary states
    {
        for(int x = 0; x < futureState.size(); ++x) // iterate over the values to produce the next generation
        {
            for(int y = 0; y < futureState[x].size(); ++y)
            {
                 int neighbors[3][3]; // Create the neighbor array, then populate it

                 for(int i = -1; i < 2; ++i)
                 {
                     for(int j = -1; j < 2; ++j)
                     {
                         if((x + i >= 0) && (x + i < width) && (y + j >= 0) && (y + j < depth))
                         {
                             neighbors[i + 1][j + 1] = currentState[x + i][y + j];
                         }

                         else
                         {
                             neighbors[i + 1][j + 1] = 0;
                         }
                     }
                 }

                 int index = 0; // Used to index the ruleArray
                 int iter = 0;

                 // convert to an index
                 for(int i = 0; i < 3; ++i)
                 {
                     for(int j = 0; j < 3; ++j)
                     {
                         // std::cout << "neighbors[i][j]: " << neighbors[i][j] << std::endl;
                        index += neighbors[i][j] << iter;
                        ++iter;
                     }
                 }

                 // std::cout << "Pre altered index: " << index << std::endl;
                 index = abs((int)ruleArraySize - 1 - index);
                 // std::cout << "Post alteredindex: " << index << std::endl;
                 futureState[x][y] = ruleArray[index];
            }
        }
    }

    else
    {
        for(int x = 0; x < futureState.size(); ++x)
        {
            for(int y = 0; y < futureState[x].size(); ++y)
            {
                double average = 0;

                for(int i = -1; i < 2; ++i)
                {
                    for(int j = -1; j < 2; ++j)
                    {
                        if((x + i >= 0)  && (x + i < width) && (y + i >= 0)  && (y + i < depth))
                        {
                            average += currentState[x + i][y + j];
                        }
                    }
                }

                average /= 9.0;
                futureState[x][y] = ruleMap[average];
            }
        }
    }

    currentState = futureState;
    ++iteration;
}

const slice_t& WolframCA3D::getCurrentState()
{
    return currentState;
}

void WolframCA3D::randomizeCurrentState()
{
    for(int x = 0; x < currentState.size(); ++x)
    {
        for(int y = 0; y < currentState[x].size(); ++y)
        {
            currentState[x][y] = rand() % possibleStates;
        }
    }
}

void WolframCA3D::calculateRuleArraySize()
{
    std::set<double> uniqueAverages;
    int neighbors = 9;
    unsigned int stateArray[9];

    for(int i = 0; i < neighbors; ++i)
    {
        stateArray[i] = 0;
    }

    for(int i = 0; i < neighbors; ++i)
    {
        for(int j = 0; j < possibleStates; ++j)
        {
            stateArray[i] = j;
            double average = 0;

            for(int k = 0; k < neighbors; ++k)
            {
                average += stateArray[k];
            }

            average /= (double) neighbors;

            uniqueAverages.insert(average);
        }
    }

    ruleArraySize = uniqueAverages.size();

    // Populate the rule averages vector
    std::set<double>::iterator iter = uniqueAverages.begin();
    while(iter != uniqueAverages.end())
    {
        ruleAverages.push_back(*iter);
        ++iter;
    }

    // Sort the vector
    std::sort(ruleAverages.begin(), ruleAverages.end());
}

void WolframCA3D::createRuleArray()
{
    if(possibleStates == 2)
    {
        ruleArraySize = 512;

        for(int i = 0; i < ruleArraySize; ++i)
        {
            unsigned int rule = (ruleSet >> (ruleArraySize - i - 1)) & 1;
            ruleArray.push_back(rule);
        }
    }

    else
    {
        calculateRuleArraySize();

        for(int i = ruleArraySize - 1; i >= 0; --i)
        {
            unsigned int rule = fmod(((double) ruleSet / pow((double)possibleStates, i)), (double)possibleStates);
            ruleArray.push_back(rule);
            ruleMap[ruleAverages[i]] =  rule; // Map for extracting the correct rule
        }
    }
}

} // cell namespace

} // alife namespace
