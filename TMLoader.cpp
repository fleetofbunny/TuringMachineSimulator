#include <sstream>
#include <fstream>
#include <string>
#include "TMLoader.h"
#include "StringExtensions.h"



TMLoader::TMLoader()
{
}


TMLoader::~TMLoader()
{
}

bool TMLoader::loadTFFile(std::string fileName, std::vector<TransitionFunction*> &output, std::string &startState, std::vector<std::string> &endStates)
{
    std::vector<TransitionFunction*> lstRetVal;
    int readingState = 0;
    /*
    * 0: Reading Name
    * 1: Reading Start State
    * 2: Reading End States
    * 3: Reading Domain
    * 4: Reading Range
    */

    std::ifstream infile(fileName);
    std::string line;

    std::string name;
//    std::string startState;
//    std::vector<std::string> endStates;
    
    std::string domainState;
    std::string rangeState;
    std::vector<DChar*> domainHeadValues;
    std::vector<DChar*> rangeHeadValues;
    std::vector<TRANSITION> rangeHeadMove;
    int numTapes;
    std::vector < std::string> splitLine;
    
    while (std::getline(infile, line))
    {
        if (readingState == 0)
        {
            splitLine = splitString(line, ':');
            if (splitLine.size() == 2 && trim(splitLine[0]) == "name")
            {
                name = trim(splitLine[1]);
                readingState++;
            }
           
        }
        else if (readingState == 1)
        {
            splitLine = splitString(line, ':');
            if (splitLine.size() == 2 && trim(splitLine[0]) == "init")
            {
                startState = trim(splitLine[1]);
                readingState++;
            }
        }
        else if (readingState == 2)
        {
            splitLine = splitString(line, ':');
            if (splitLine.size() >= 2 && trim(splitLine[0]) == "accept")
            {
                for (int i = 1; i < splitLine.size(); i++)
                {
                    endStates.push_back(trim(splitLine[i]));
                }
                readingState++;
            }
        }
        else if (readingState >= 3)
        {
            splitLine = splitString(line, ',');
            if (splitLine.size() > 0)
            {
                if (readingState == 3)
                {
                    domainHeadValues.clear();
                    rangeHeadMove.clear();
                    rangeHeadValues.clear();
                    numTapes = splitLine.size() - 1;
                    domainState = splitLine[0];
                    for (int i = 1; i < splitLine.size(); i++)
                    {
                        domainHeadValues.push_back(new DChar(splitLine[i][0], splitLine[i].size()-1));
                    }
                    readingState = 4;
                }
                else
                {
                    rangeState = splitLine[0];
                    for (int i = 1; i <= numTapes; i++)
                    {
                        rangeHeadValues.push_back(new DChar(splitLine[i][0], splitLine[i].size()-1));
                    }
                    for (int i = numTapes + 1; i < splitLine.size(); i++)
                    {
                        rangeHeadMove.push_back(this->convertCharToTranstion(splitLine[i][0]));
                    }
                    TransitionFunction* newTf = new TransitionFunction();
                    newTf->define(numTapes);
                    newTf->initialize(domainState, domainHeadValues,
                        rangeState, rangeHeadValues, rangeHeadMove);
                    lstRetVal.push_back(newTf);
                    readingState= 3;
                } // End if (readingState == 3)
            }
        } // End if (readingState >= 3)
    }

    output = lstRetVal;

    return readingState == 3;
}

std::vector<std::string> TMLoader::splitString(const std::string szInString, char splitChar) const
{
    std::stringstream test(szInString);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(test, segment, splitChar))
    {
        seglist.push_back(segment);
    }

    return seglist;
}

TRANSITION TMLoader::convertCharToTranstion(char cMap) const
{
    TRANSITION tRetVal;
    if (cMap == '<')
    {
        tRetVal = LEFT;
    }
    else if (cMap == '>')
    {
        tRetVal = RIGHT;
    }
    else
    {
        tRetVal = STAY;
    }

    return tRetVal;
}