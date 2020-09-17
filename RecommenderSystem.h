//
// Created by nehor on 21/06/2020.
//
#include "string.h"
#include <iostream>

using namespace std;

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

/**
 * the main class
 */
class RecommenderSystem
{
public:

    /**
   *
   * @param moviesAttributesFilePath
   * @param userRanksFilePath
   * @return -1 in fail, 1 success
   */
    int loadData(string moviesAttributesFilePath, string userRanksFilePath);

    /**
    *
     * @param userName the user name
    * @return the best movie to the user
    */
    string recommendByContent(string userName);

    /**
    *
    * @param movieName the movie name
    * @param userName the user name
    * @param k the parameter to the algorithem
    * @return the number we think the user give to the movie
    */
    double predictMovieScoreForUser(string movieName, string userName, int k);

    /**
     *
     * @param userName the user name
     * @param k the parameter
     * @return the best movie to the user
     */
    string recommendByCF(string userName, int k);
};


#endif //EX5_RECOMMENDERSYSTEM_H
