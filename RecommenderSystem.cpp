#include <fstream>
#include <vector>
#include "RecommenderSystem.h"
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cmath>

#define FAILTURE -1
#define SUCCESS 0
#define MIN -2

using namespace std;

/**
 * movie object
 */
class movie
{
public:
    string name;
    vector<double> vec;
    int index = 0;
};

/**
 * user object
 */
class user
{
public:

    string name;
    vector<double> vec;
    vector<double> normVec;
    vector<double> preferencesVector;
};


unordered_map<string, movie> movieMap;
unordered_map<string, user> userMap;
vector<string> movieVec;

/**
 * norm vector
 * @param vec the vector to norm
 * @return the normed vector
 */
vector<double> normVector(vector<double> &vec)
{
    vector<double> toReturn = vec;
    double sum = 0;
    double index = 0;
    for (int i: vec)
    {
        if (i != 0)
        {
            sum += i;
            index++;
        }
    }
    if (index == 0)
    {
        return vec;
    }
    double average = sum / index;
    for (int i = 0; i < (int) vec.size(); i++)
    {
        if (vec[i] != 0)
        {
            toReturn[i] = vec[i] - average;
        }
    }
    return toReturn;
}


/**
 * split dtring by white-space into vector
 * @param str the string to split
 * @param vec the vector to split to
 */
void split(const string &str, vector<double> &vec)
{
    istringstream ss(str);

    double i = 0;
    string word;
    for (auto x : str)
    {
        if (x == ' ' || x == '\r' || x == '\n')
        {
            if (word == "NA")
            {
                i = 0;
            }
            else
            {
                i = stod(word);
            }

            vec.push_back(i);
            word = "";
        }
        else
        {
            word += x;
        }
    }
}

/**
 *
 *
 * @param line  the first line
 */
void firstLineHandler(string &line)
{
    line = line.substr(0, line.find_first_of('\r'));
    line = line.substr(0, line.find_first_of('\n'));
    line += " ";

    int index = 0;
    string temp;
    while (!line.empty())
    {
        temp = line.substr(0, line.find_first_of(' '));
        movieMap.find(temp)->second.index = index;
        movieVec.push_back(temp);
        index++;
        line = line.substr(line.find_first_of(' ') + 1);
    }
}

/**
 *
 * @param line line of movie
 *
 */
void movieLineHandler(string &line)
{
    movie temp;

    int pos = line.find(' ');
    temp.name = line.substr(0, line.find_first_of(' '));

    split(line.substr(pos + 1), temp.vec);

    movieMap.insert({temp.name, temp});

}

/**
 *
 * @param line the line of the user
 *
 */
void userLineHandler(string &line)
{
    user temp;

    int pos = line.find(' ');
    temp.name = line.substr(0, line.find_first_of(' '));

    split(line.substr(pos + 1), temp.vec);

    userMap.insert({temp.name, temp});
}

/**
 *
 * @param vec1 the first vector to add
 * @param vec2 the second vector to add
 * @return the sum of vec1 and vec2
 */
vector<double> addVectors(vector<double> &vec1, vector<double> &vec2)
{
    vector<double> toReturn = vec1;
    for (int i = 0; i < (int) vec1.size(); i++)
    {
        toReturn[i] = vec1[i] + vec2[i];
    }
    return toReturn;
}

/**
 *
 * @param num some scalar
 * @param vec vector
 * @return the multiply vector
 */
vector<double> multiplyVector(double num, vector<double> &vec)
{
    vector<double> toReturn;
    toReturn = vec;
    for (int i = 0; i < (int) vec.size(); i++)
    {
        toReturn[i] = num * vec[i];
    }
    return toReturn;
}

/**
 *
 * @param normedVec the normed vector
 * @return the preferences vector
 */
vector<double> preferencesVector(vector<double> &normedVec)
{
    vector<double> temp;
    vector<double> toReturn;
    int i = 0;
    for (const string &s:movieVec)
    {
        if (i == 0)
        {
            temp = multiplyVector(normedVec[i], movieMap.find(s)->second.vec);
            toReturn = temp;
        }
        else
        {
            temp = multiplyVector(normedVec[i], movieMap.find(s)->second.vec);
            toReturn = addVectors(temp, toReturn);
        }
        i++;
    }
    return toReturn;
}

/**
 *
 * @param moviesAttributesFilePath
 * @param userRanksFilePath
 * @return -1 in fail, 1 success
 */
int RecommenderSystem::loadData(const string moviesAttributesFilePath, const string userRanksFilePath)
{
    string line;
    ifstream file1(moviesAttributesFilePath);
    if (!file1.is_open())
    {
        cout << "Unable to open file <" + moviesAttributesFilePath + ">\n";
        return FAILTURE;
    }

    while (getline(file1, line))
    {
        movieLineHandler(line);
    }

    int i = 0;
    ifstream file2(userRanksFilePath);
    if (!file2.is_open())
    {
        cout << "Unable to open file <" + moviesAttributesFilePath + ">\n";
        return FAILTURE;
    }
    while (getline(file2, line))
    {
        if (i == 0)
        {
            firstLineHandler(line);
        }
        else
        {
            userLineHandler(line);
        }
        i++;
    }
    return SUCCESS;
}


/**
 * calculate the dot-product
 * @param vec1 the first vector
 * @param vec2 the second vector
 * @return the dot-product
 */
double dotProduct(vector<double> &vec1, vector<double> &vec2)
{
    double toReturn = 0.0;
    for (int i = 0; i < (int) vec1.size(); i++)
    {
        toReturn += (vec1[i] * vec2[i]);
    }
    return toReturn;
}

/**
 * calculate the similarity
 * @param vec1 the first vector
 * @param vec2 the second vector
 * @return the similarity
 */
double similarityVectors(vector<double> &vec1, vector<double> &vec2)//its OK!
{

    double numerator = dotProduct(vec1, vec2);
    double denominator = sqrt(dotProduct(vec1, vec1)) * sqrt(dotProduct(vec2, vec2));
    return (numerator / denominator);
}


/**
 *
 * @param userName the user name
 * @return the best movie to the user
 */
string RecommenderSystem::recommendByContent(const string userName)
{
    if (userMap.find(userName) == userMap.end())
    {
        return "USER NOT FOUND";
    }

    double max = MIN;
    double temp;
    string bestMovie;
    user client = userMap.find(userName)->second;
    client.normVec = normVector(client.vec);

    vector<double> clientVec;

    if (!client.preferencesVector.empty())
    {
        clientVec = client.preferencesVector;
    }
    else
    {
        clientVec = preferencesVector(client.normVec);
    }

    for (const string &s : movieVec)
    {
        int index = movieMap.find(s)->second.index;
        if (client.vec[index] == 0)
        {
            temp = similarityVectors(clientVec, movieMap.find(s)->second.vec);
            if (temp > max)
            {
                bestMovie = s;
                max = temp;
            }
        }
    }
    return bestMovie;
}

/**
 *
 * @param movieName the name of the movie
 * @param userName the name of the user
 * @param k the parameter to the algorithem
 * @return the number we think the user give to the movie
 */
double findMatching(const string &movieName, const string &userName, int k)
{
    multimap<double, string> scores;
    vector<double> givenMovieVector = movieMap.find(movieName)->second.vec;
    user client = userMap.find(userName)->second;

    if (movieMap.find(movieName) == movieMap.end())
    {
        return MIN;
    }

    int index = movieMap.find(movieName)->second.index;

    int t = client.vec.size();

    if (index < t && client.vec[index] != 0)
    {
        return MIN;
    }

    for (int i = 0; i < (int) client.vec.size(); i++)
    {
        if (client.vec[i] != 0 && i != index)
        {
            double similiar = similarityVectors(givenMovieVector, movieMap.find(movieVec[i])->second.vec);
            scores.insert(pair<double, string>(similiar, movieVec[i]));
        }
    }

    double similiar;
    double second;
    string movie;
    double numerator = 0;
    double denominator = 0;

    auto it = scores.end();
    it--;
    int i = 0;
    for (i = 0; i < k; i++)
    {
        similiar = it->first;
        movie = scores.find(similiar)->second;
        index = movieMap.find(movie)->second.index;
        second = client.vec[index];
        numerator += similiar * second;
        denominator += similiar;
        it--;
    }
    return (numerator / denominator);
}

/**
 *
 * @param movieName the movie name
 * @param userName the user name
 * @param k the parameter to the algorithem
 * @return the number we think the user give to the movie
 */
double RecommenderSystem::predictMovieScoreForUser(const string movieName, const string userName, int k)
{
    if (userMap.find(userName) == userMap.end() || movieMap.find(movieName) == movieMap.end())
    {
        return FAILTURE;
    }
    return findMatching(movieName, userName, k);
}

/**
 *
 * @param userName the user name
 * @param k the parameter
 * @return the best movie to the user
 */
string RecommenderSystem::recommendByCF(const string userName, int k)
{
    if (userMap.find(userName) == userMap.end())
    {
        return "USER NOT FOUND";
    }

    user client = userMap.find(userName)->second;

    double max = MIN;
    string bestMovie;
    double temp = MIN;
    for (int i = 0; i < (int) movieVec.size(); i++)
    {
        int index = movieMap.find(movieVec[i])->second.index;

        int t = client.vec.size();

        if (index < t && client.vec[index] == 0)
        {
            temp = findMatching(movieVec[i], userName, k);
            if (temp > max)
            {
                max = temp;
                bestMovie = movieVec[i];
            }
            else if (temp == max)
            {
                if (movieMap.find(movieVec[i])->second.index < movieMap.find(bestMovie)->second.index)
                {
                    max = temp;
                    bestMovie = movieVec[i];
                }
            }
        }
    }
    return bestMovie;
}
