#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <random>
#include <time.h>

struct City
{
    int id;
    int X;
    int Y;
};



const std::string outFileName =  "tsp_example_3.txt.tour";
const std::string fileName =  "tsp_example_3.txt";



int shortDistance = INT32_MAX;
std::vector<City> shortPath;
bool isLowerFlag = false;
void PrintDistance(int currDistance, std::vector<City> currPath, int itr)
{
   
    
    if(isLowerFlag)
       {
           isLowerFlag = false;
            std::ofstream outfile(outFileName);
        std::cout << itr << " Distance is now :> " << shortDistance << std::endl;
      
        
        outfile << shortDistance << std::endl;
        for (int i = 0; i < shortPath.size(); i++)
        {
            outfile << shortPath.at(i).id << std::endl;
        }
       
        outfile.close();
    }

   
}

void holdDistance(int currDistance, std::vector<City> currPath)
{
   
    if(currDistance < shortDistance)
    {
        isLowerFlag = true;
        shortPath = currPath;
        std::cout  << " Distance is now :> " << currDistance << std::endl;
        shortDistance = currDistance;
     
    }
        
        
    
   
}
int GetDistance(std::vector<City> path )
{
    int retDist = 0;
    for(int i = 0; i < path.size()-1; i++)
    {
        double dist =sqrt(pow((path[i].X - path[i+1].X), 2) + pow((path[i].Y- path[i+1].Y), 2));
        int d = round(dist);
        retDist+= d;
    }
    
    double dist =sqrt(pow((path[path.size()-1].X - path[0].X), 2) + pow((path[path.size()-1].Y- path[0].Y), 2));
    int d = round(dist);
    retDist+= d;
return retDist;
}


std::vector<City> GetRandomPath(std::vector<City> oldPath)
{
    
    
    std::vector<City> newPath;
    int randPoint = rand() % (oldPath.size()-1);
    int randPoint2 = rand() % (oldPath.size()-1 - randPoint) + randPoint;
   // std::cout << "swapping at " << randPoint << "at " << randPoint +1 << std::endl;
    int k=1;
    for(int i =0; i < randPoint; i++)
    {
        newPath.push_back(oldPath.at(i));
    }
    for(int i=randPoint; i < randPoint2; i++)
    {
    
        newPath.push_back(oldPath.at(randPoint2-k));
        k++;
    }
    for(int i=randPoint2; i < oldPath.size(); i++)
    {
        newPath.push_back(oldPath.at(i));
    }

    
    
    return newPath;

}

float SimulatedAnealing(double ti, std::vector<City> cities, double coolingRate, double tempLimit  )
{
    clock_t t1, t2;
    t1 = clock();
    double currTemp = ti;
    std::vector<City> currPath = GetRandomPath(cities);
    int currDistance = GetDistance(currPath);
    
    int worseCount = 0;
    int itr=0;
    while(currTemp > tempLimit)
    {
        itr++;
        std::vector<City> nextPath = GetRandomPath(currPath);
        int nextDistance =GetDistance(nextPath);
        int diff = currDistance - nextDistance;
        if(diff >= 0)
        {
            currPath = nextPath;
            currDistance = nextDistance;
        }
        else
        {
            double prob = rand() /(RAND_MAX) +1;
           //std:: cout <<  std::exp(-diff/ currTemp) << std::endl;
            if(std::exp(-diff/ currTemp)> (prob) )
            {
                worseCount++;
                currPath = nextPath;
                currDistance = nextDistance;
            }
            
        }
        
        currTemp = currTemp * coolingRate;
        holdDistance(currDistance, currPath);
      // PrintDistance(currDistance, currPath, itr);
        
    }
    PrintDistance(currDistance, currPath, itr);
  std::cout << itr << ":" << worseCount << std::endl;
t2= clock();
float diff = t2-t1;
float runTime = diff/ CLOCKS_PER_SEC;
//std::cout << "Runtime:> " << runTime  << std::endl;
return runTime;
}

int main()
{
   
    
    srand (time(NULL));
    
    std::ifstream file(fileName);
    std::string line;
    std::vector<City> values;
    std::stringstream ss;
    std::string tempVal;
    int intVal;
    while (!file.eof())
    {

        //get one line of file
        std::getline(file, line);

        ss << line;
       // std::cout << line << std::endl;

        while (!ss.eof())
        {

            //get one charecter
            ss >> tempVal;
            //ignore the first char
            City newCity;
            std::stringstream(tempVal) >> newCity.id;
            ss >> tempVal;
            std::stringstream(tempVal) >> newCity.X;
            ss >> tempVal;
            std::stringstream(tempVal) >> newCity.Y;
            values.push_back(newCity);
            //std::cout << newCity.id << newCity.X << newCity.Y << std::endl; 
            tempVal = "";
        }
    
        //std::cout << std::endl;
        ss.clear();
        line.clear();
    }
    values.pop_back();
    double temperature = 999999999999999999999999999999999999999999.0;
   float currTime=0;
   float timeLimt = 10 * 60;
   int runCount = 0;
   shortPath = values;
   while(currTime < timeLimt)
   {
    std::vector<City> runValues = shortPath;
      currTime+=  SimulatedAnealing(temperature, runValues, 0.99, .0000001);
      runCount++;
   }
        
      std::cout << "Ran SA " << runCount << " times in " << currTime << " sec." << std::endl;
        file.close();
        
    }
   

