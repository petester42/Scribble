/*
 *
 * File:   PalmRejection.cpp
 * Author: greg
 *
 * Created on May 23, 2012, 12:39 PM
 */

#include "PalmRejection.h"

/** Default constructor
 * 
 * @param scribbleA This is a pointer to the a ScribbleArea
 */
PalmRejection::PalmRejection(ScribbleArea* scribbleA) : scribble(scribbleA), stopRequest(false), position(0), sampling(0), penPresent(false), mX(-1), mY(-1)
{
    //for now limit vector to 5, analyze data and decide what to do with it
    pointToAnalyze.resize(ANALYZE_BUFFER);

    //Timer used for palm reset
    //The palm will be fully reset after x seconds of 0 activity on the screen
    palmResetTimer = new boost::asio::deadline_timer(io_service);
    palmResetTimer->expires_from_now(boost::posix_time::seconds(RESET_TIMER));
    palmResetTimer->async_wait(boost::bind(&PalmRejection::resetPalm, this,_1)); 
}

/** Default destructor
 *
 * Deletes all Point object available in the pointToAnalyze buffer
 */
PalmRejection::~PalmRejection()
{
    palmResetTimer->cancel();
    delete palmResetTimer;
    
    flushPointBuffer();
}

/** Reset palm
 * 
 * This function is called x seconds after the last event on the screen (when resetTimer expires). Once called it will reset the palm area.
 * 
 * The resetTimer is only set once there is a complete release on the screen and is stopped as soon as the a new activity occurs. If the timer doesn't have the time to expire
 * this function will not be called and no reset will occur. 
 */

/*void handler1(const boost::system::error_code &ec) 
{ 
    if (ec == boost::asio::error::operation_aborted)
    {
        std::cout << microsec_clock::local_time() << " Handler1: Timer 1 was cancelled or retriggered." << std::endl; 
    }
    else
    {
        std::cout << microsec_clock::local_time() << " Handler1: expired." << std::endl; 
    }
}*/

void PalmRejection::resetPalm(const boost::system::error_code &ec)
{
    mPalm.clearMatrix();
    //std::cout << "RESET PALM" << std::endl;

    //FOR TESTING
    //mPen.clearMatrix();
    
    palmResetTimer->cancel();
}

/** Touch event
 * 
 * @param mPointsQueue This is a queue of Point pointers (std::queue<Point* >)
 * 
 * This function is called when the first set of points is available.
 */
void PalmRejection::eventTouch(std::queue<Point* > *mPointsQueue)
{
    //Copy all points from the received queue to a local queue
    while (!mPointsQueue->empty())
    {
        pointToAnalyze[position].push_back(mPointsQueue->front());
        mPointsQueue->pop();
    }

    //Stop the palm reset timer
    palmResetTimer->cancel();

    //Remove possible duplicated points and find the palm and/or pen
    analyzeNewSetOfPoints();
    findPalm();
}

/** Move event
 * 
 * @param mPointsQueue This is a queue of Point pointers (std::queue<Point* >)
 * 
 * This function is called by the InpitData whenever a new set of points is available, however not when it is the first set of points available
 * Calling this function does not mean that a move occurred since all previous points could have been considered as the palm 
 */
void PalmRejection::eventMove(std::queue<Point* > *mPointsQueue)
{
    //Copy all points from the received queue to a local queue
    while (!mPointsQueue->empty())
    {
        pointToAnalyze[position].push_back(mPointsQueue->front());
        mPointsQueue->pop();
    }

    //Remove possible duplicated points
    analyzeNewSetOfPoints();

    if (!penPresent && !mPalm.isSet())
    {
        //std::cout<<"Find palm"<<std::endl;
        findPalm();
    }
    else if (!penPresent)
    {
        findPen();
        //std::cout<<"Find pen"<<std::endl;
    }
    else
    {
        findNextPoint();
        //std::cout<<"Find next point"<<std::endl;
    }

}

/** Release event
 *
 * This function is called by the inputData only when there was a complete release on the screen i.e: nothing is touching it.
 *
 * This function will reset the palm detection but keep the area where the pen was last. This is done so that the user can make small 
 * adjustments (points, commas...). If the pen area would not be saved then the pen would need to be re-detected causing a slowdown 
 * in user writing and not allowing the user to add small writing
 */
void PalmRejection::eventRelease(/*Points *point*/)
{
    //Disable penPresent, send release event to scribbleArea, clean buffer, and start palm reset timer
    scribble->screenReleaseEvent();
    penPresent = false;
    flushPointBuffer();
    palmResetTimer->expires_from_now(boost::posix_time::seconds(RESET_TIMER));
    palmResetTimer->async_wait(boost::bind(&PalmRejection::resetPalm, this,_1));
}

/** Modulo function (always positive)
 * 
 * @param x The number to be divided (dividend)
 * @param m The "limit" (divisor) - If not entered it will automatically be the value of ANALYZEBUFFER
 * @return The positive modulo of input x
 * 
 * This function returns a positive modulo of x.
 * Default divisor is ANALYZEBUFFER however passing a second parameter to the function will overwrite it
 */
int PalmRejection::mod(int x, const int m)
{
    return (x % m + m) % m;
}

/** Absolute value function
 * 
 * @param x The integer of which we want the absolute value
 * @return The absolute value of x
 * 
 * This function return the absolute value of any integer
 */
int PalmRejection::abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

/** Square root of an integer
 * 
 * @param x The integer that we need the square root of
 * @return The square root of the x parameter
 * 
 * This function is about 5 times faster than the sqrt() on the cmath library. It is less precise however this is not an issue in our case
 */
inline float PalmRejection::sqrt(const int x)
{

    union
    {
        int i;
        float x;
    } u;
    u.x = x;
    u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
    return u.x;
}

/** Find the Pen
 *
 * This function determines which 4 consecutive points can be assumed to be a pen.
 *
 * The presence of a pen can be assumed whenever there are four points (in four different sets) that meet the following criteria:
 * - Two consecutive points have to be a minimum distance from each other without exceeding a maximum distance
 * - The total total distance between the four points need to be a minimum of 6 times the minimum distance between two consecutive points. 
 *
 */
void PalmRejection::findPen()
{

    //check in a point in the current set falls into the mPen area. If yes, assume it is the pen
    for (int i = 0; i < (int) pointToAnalyze[position].size(); i++)
    {
        if (pointToAnalyze[position][i] != NULL)
        {
            //std::cout << "(pointToAnalyze[position][i]->getRow(): " << pointToAnalyze[position][i]->getRow() << " pointToAnalyze[position][i]->getColumn(): " << pointToAnalyze[position][i]->getColumn() << std::endl;
            if ((pointToAnalyze[position][i] != NULL) && ((mPen.isSet(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow())) || (pointToAnalyze[position][i]->getRow() > 4)))
            {
                scribble->screenPressEvent(pointToAnalyze[position][i]);
                //std::cout<<"Press -         findPen() first loop"<<std::endl;
                mPen.setArea(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow());
                pointToAnalyze[position][i] = NULL;
                penPresent = true;

                updatePosition();
                return;
            }
        }
    }

    //Used for set comparison to find possible points that will result in a pen. 
    int firstSet = mod(position - 3);
    int secondSet = mod(firstSet + 1);
    int thirdSet = mod(secondSet + 1);
    int forthSet = mod(thirdSet + 1);

    bool completeBreak = false;

    //for each point is the first set
    for (ushort i = 0; i < pointToAnalyze[firstSet].size(); i++)
    {
        if ((pointToAnalyze[firstSet][i] != NULL))
        {
            //check if the point is in a possible pen area. If yes keep comparing points, otherwise set it as Palm area
            if (mPalm.possiblePen(pointToAnalyze[firstSet][i]->getColumn(), pointToAnalyze[firstSet][i]->getRow()))
            {
                //for each point in the second set
                for (ushort j = 0; j < pointToAnalyze[secondSet].size(); j++)
                {
                    if ((pointToAnalyze[secondSet][j] != NULL))
                    {
                        //check if the point is in a possible pen area. If yes keep comparing points, otherwise set it as Palm area
                        if (mPalm.possiblePen(pointToAnalyze[secondSet][j]->getColumn(), pointToAnalyze[secondSet][j]->getRow()))
                        {
                            //find which point corresponds to a possible pen, with limits (max and min)
                            //if found  do the same comparison between second and third
                            int deltaX_ij = pointToAnalyze[firstSet][i]->getX() - pointToAnalyze[secondSet][j]->getX();
                            int deltaY_ij = pointToAnalyze[firstSet][i]->getY() - pointToAnalyze[secondSet][j]->getY();
                            int radius_ij = sqrt(deltaX_ij * deltaX_ij + deltaY_ij * deltaY_ij);

                            //within acceptable movement of a pen
                            if (radius_ij > MINIMUM_RADIUS && radius_ij < MAXIMUM_RADIUS)
                            {
                                //found 2 points that can be considered as a pen
                                //for each point in the third set
                                for (ushort k = 0; k < pointToAnalyze[thirdSet].size(); k++)
                                {
                                    if ((pointToAnalyze[thirdSet][k] != NULL))
                                    {
                                        //check if the point is in a possible pen area. If yes keep comparing points, otherwise set it as Palm area
                                        if (mPalm.possiblePen(pointToAnalyze[thirdSet][k]->getColumn(), pointToAnalyze[thirdSet][k]->getRow()))
                                        {
                                            //find which point corresponds to a possible pen, with limits (max and min)
                                            //if found do the same comparison between third and forth
                                            int deltaX_jk = pointToAnalyze[secondSet][j]->getX() - pointToAnalyze[thirdSet][k]->getX();
                                            int deltaY_jk = pointToAnalyze[secondSet][j]->getY() - pointToAnalyze[thirdSet][k]->getY();
                                            int radius_jk = sqrt(deltaX_jk * deltaX_jk + deltaY_jk * deltaY_jk);

                                            //within acceptable movement of a pen
                                            if (radius_jk > MINIMUM_RADIUS && radius_jk < MAXIMUM_RADIUS)
                                            {
                                                //found 3 points that can be considered as a pen
                                                //for each point in the forth set
                                                for (ushort l = 0; l < pointToAnalyze[forthSet].size(); l++)
                                                {
                                                    if ((pointToAnalyze[forthSet][l] != NULL))
                                                    {
                                                        //check if the point is in a possible pen area. If yes keep comparing points, otherwise set it as Palm area
                                                        if (mPalm.possiblePen(pointToAnalyze[forthSet][l]->getColumn(), pointToAnalyze[forthSet][l]->getRow()))
                                                        {
                                                            //find which point corresponds to a possible pen, with limits (max and min)
                                                            int deltaX_kl = pointToAnalyze[thirdSet][k]->getX() - pointToAnalyze[forthSet][l]->getX();
                                                            int deltaY_kl = pointToAnalyze[thirdSet][k]->getY() - pointToAnalyze[forthSet][l]->getY();
                                                            int radius_kl = sqrt(deltaX_kl * deltaX_kl + deltaY_kl * deltaY_kl);

                                                            int distance = radius_ij + radius_jk + radius_kl;

                                                            //Four points in four consecutive sets are determined to be a pen, use the point 
                                                            if ((radius_kl > MINIMUM_RADIUS) && (radius_kl < MAXIMUM_RADIUS) && (distance > (5 * MINIMUM_RADIUS)))
                                                            {
                                                                //found 4 points that look like a pen
                                                                penPresent = true;

                                                                //Save the last point coordinates and set the pen area
                                                                mPen.setArea(pointToAnalyze[forthSet][l]->getColumn(), pointToAnalyze[forthSet][l]->getRow());
                                                                mX = pointToAnalyze[forthSet][l]->getX();
                                                                mY = pointToAnalyze[forthSet][l]->getY();

                                                                //Send the four points to the acribbleArea for drawing
                                                                scribble->screenPressEvent(pointToAnalyze[firstSet][i]);
                                                                // std::cout<<"Press -         findPen() second loop"<<std::endl;
                                                                scribble->screenMoveEvent(pointToAnalyze[secondSet][j]);
                                                                scribble->screenMoveEvent(pointToAnalyze[thirdSet][k]);
                                                                scribble->screenMoveEvent(pointToAnalyze[forthSet][l]);

                                                                //Update palm matrix
                                                                mPalm.clearMatrix();

                                                                updatePalmMatrix(firstSet, i);
                                                                updatePalmMatrix(secondSet, j);
                                                                updatePalmMatrix(thirdSet, k);
                                                                updatePalmMatrix(forthSet, l);

                                                                mPalm.resetArea(pointToAnalyze[forthSet][l]->getColumn(), pointToAnalyze[forthSet][l]->getRow());

                                                                //set the used points to NULL since they belong to the scribble area now
                                                                pointToAnalyze[firstSet][i] = NULL;
                                                                pointToAnalyze[secondSet][j] = NULL;
                                                                pointToAnalyze[thirdSet][k] = NULL;
                                                                pointToAnalyze[forthSet][l] = NULL;

                                                                completeBreak = true;
                                                                break;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            mPalm.set(pointToAnalyze[forthSet][l]->getColumn(), pointToAnalyze[forthSet][l]->getRow());
                                                        }
                                                    }
                                                }
                                                if (completeBreak)
                                                {
                                                    break;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            mPalm.set(pointToAnalyze[thirdSet][k]->getColumn(), pointToAnalyze[thirdSet][k]->getRow());
                                        }
                                    }
                                }
                                if (completeBreak)
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            mPalm.set(pointToAnalyze[secondSet][j]->getColumn(), pointToAnalyze[secondSet][j]->getRow());
                        }
                    }
                }
                if (completeBreak)
                {
                    break;
                }
            }
            else
            {
                mPalm.set(pointToAnalyze[firstSet][i]->getColumn(), pointToAnalyze[firstSet][i]->getRow());
            }
        }
    }

    //if we didn't completely break it means we didn't find a pen, setting the value accordingly
    if (!completeBreak)
    {
        scribble->screenReleaseEvent();
        penPresent = false;
    }

    updatePosition();
}

/** Find adjacent points
 *
 * This function looks at a set of points and eliminates the adjacent ones.
 * The function will eliminate all the points in adjacent columns and/or rows with the exception of the one that is the furthest with respect to the previous acknowledge point
 */
void PalmRejection::analyzeNewSetOfPoints()
{
    int size = pointToAnalyze[position].size();

    //For each pair of point, verify if they are in adjacent areas
    // if yes:
    //  - if they are at a distance less that MIN_R eliminate one (eliminating the one furthest from the last point and keeping the closest one)
    //if no:
    //  - leave both points and let other functions (findPalm(), findPen()) take care of it
    for (int i = 0; i < size - 1; i++)
    {
        if (pointToAnalyze[position][i] != NULL)
        {
            for (int j = i + 1; j < size; j++)
            {
                if (pointToAnalyze[position][j] != NULL)
                {
                    int deltaColumn = abs(pointToAnalyze[position][i]->getColumn() - pointToAnalyze[position][j]->getColumn());
                    int deltaRow = abs(pointToAnalyze[position][i]->getRow() - pointToAnalyze[position][j]->getRow());

                    //If the points in the same set are in adjacent areas
                    if (deltaColumn == 1 || deltaRow == 1)
                    {
                        //If the radius is less than MIN_R, delete the point that is the furthest from last touch point
                        //and if that point does not exist eliminate the second point from the set
                        int deltaX = pointToAnalyze[position][i]->getX() - pointToAnalyze[position][j]->getX();
                        int deltaY = pointToAnalyze[position][i]->getY() - pointToAnalyze[position][j]->getY();

                        int radius = sqrt(deltaX * deltaX + deltaY * deltaY);

                        if (radius < MIN_R)
                        {
                            if (penPresent == true)// pen exists
                            {
                                int deltaXi = mX - pointToAnalyze[position][i]->getX();
                                int deltaYi = mY - pointToAnalyze[position][i]->getY();

                                int distance_i = sqrt(deltaXi * deltaXi + deltaYi * deltaYi);

                                int deltaXj = mX - pointToAnalyze[position][j]->getX();
                                int deltaYj = mY - pointToAnalyze[position][j]->getY();

                                int distance_j = sqrt(deltaXj * deltaXj + deltaYj * deltaYj);

                                if (distance_i < distance_j)
                                {
                                    delete pointToAnalyze[position][j];
                                    pointToAnalyze[position][j] = NULL;
                                }
                                else
                                {
                                    delete pointToAnalyze[position][i];
                                    pointToAnalyze[position][i] = NULL;
                                    break;
                                }
                            }//If no previous point then delete always delete the second point (how accurate is that?)
                            else // No Pen
                            {
                                delete pointToAnalyze[position][j];
                                pointToAnalyze[position][j] = NULL;
                            }
                        }
                    }
                }
            }
        }
    }
}

/** Update palm matrix
 * 
 * @param slot slot This variable represents the position in pointToAnalyze from which we need to update the palm matrix. Usually it will
 * correspond to the position variable however, at occasions, it can represent an older position
 * @param ignore ignore This variable represents which position in a given set of points corresponds to the pen point and should not be present on the palm matrix
 * 
 * This function updates the palm matrix by setting high all the areas pressed on the last sampling set with the exception
 * of the area where the pen has been determined to be present
 */
void PalmRejection::updatePalmMatrix(ushort slot, ushort ignore)
{
    for (ushort i = 0; i < pointToAnalyze[slot].size(); i++)
    {
        if ((pointToAnalyze[slot][i] != NULL) && (i != ignore))
        {
            mPalm.setArea(pointToAnalyze[slot][i]->getColumn(), pointToAnalyze[slot][i]->getRow());
        }
    }
}

/** Update variable position
 *
 * This function updates the position variable while making sure to delete all Point object that will no longer be needed
 */
void PalmRejection::updatePosition()
{
    position = mod(++position);

    for (uint i = 0; i < pointToAnalyze[position].size(); i++)
    {
        //Do not need to check for NULL since delete NULL has not effect
        delete pointToAnalyze[position][i];
    }
    pointToAnalyze[position].clear();
}

/** Find the next valid point
 *
 * This function iterates through the new set of points and determines which is the next valid point
 * It is possible that we do not find the next point which will cause a new search for the pen
 */
void PalmRejection::findNextPoint()
{
    //for all points available in pointToAnalyze[position]
    //compare distance to last saved mX and mY in mPen area
    //choose min?
    bool found = false;
    for (ushort i = 0; i < pointToAnalyze[position].size(); i++)
    {
        if (pointToAnalyze[position][i] != NULL)
        {
            //if the point is in the pen area and not in the palm area
            if (mPen.isSet(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow()) || mPalm.possiblePen(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow())) // && !mPalm.isSet(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow()))
            {
                // We should find the radius from the last point and make sure that we do not exceed a MAXIMUM (minimum not necessary
                int Dx = mX - pointToAnalyze[position][i]->getX();
                int Dy = mY - pointToAnalyze[position][i]->getY();

                mX = pointToAnalyze[position][i]->getX();
                mY = pointToAnalyze[position][i]->getY();

                //Move the pen area to correspond to the newly found pen point
                mPen.clearMatrix();
                mPen.setArea(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow());

                //Update the palm area with the point(s) that do not correspond to the pen
                //mPalm.clearMatrix();

                updatePalmMatrix(position, i);
                mPalm.reset(pointToAnalyze[position][i]->getColumn(), pointToAnalyze[position][i]->getRow());

                //if the new pen point is within the 
                if (sqrt(Dx * Dx + Dy * Dy) < MAXIMUM_RADIUS) //This was 2*MAXIMUM_RADIUS and worked well. testing with different values
                {
                    scribble->screenMoveEvent(pointToAnalyze[position][i]);
                }//otherwise assume the new point represents a "new" pen by sending a release followed by a press events to the scribble area
                else
                {
                    scribble->screenReleaseEvent();
                    scribble->screenPressEvent(pointToAnalyze[position][i]);
                    //std::cout<<"Press -         findNextPoint"<<std::endl;
                }

                //Set the sent point pointer to NULL since it is own by the scribble area now
                pointToAnalyze[position][i] = NULL;
                found = true;
                break;
            }
        }
    }

    //if the pen was not found in the above loop then set penPresent to false which will initialize a search for a pen at the next event
    if (found == false)
    {
        scribble->screenReleaseEvent();
        penPresent = false;
    }

    updatePosition();
}

/** Flush the pointToAnalyze buffer
 *
 * This function is called whenever we want to completely flush all the points in the whole pointToAnalyze buffer
 */
void PalmRejection::flushPointBuffer()
{
    for (uint i = 0; i < ANALYZE_BUFFER; i++)
    {
        for (uint j = 0; j < pointToAnalyze[i].size(); j++)
        {
            delete pointToAnalyze[i][j];
        }
        pointToAnalyze[i].clear();
    }
    position = 0;
}

/** Find palm
 * 
 * This function determines if the screen touches correspond to a palm or a pen.
 * 
 * A pen will be determined from this function if and only if there is only 1 point present in four consecutive sets
 * 
 * A palm will be determined if and only if there will be at least 2 points in four consecutive sets
 */
void PalmRejection::findPalm()
{
    //Used to access the four last consecutive sets of points
    int firstSet = mod(position - 3);
    int secondSet = mod(firstSet + 1);
    int thirdSet = mod(secondSet + 1);
    int forthSet = mod(thirdSet + 1);

    //check # of point in last 4 sets
    //if 1 in each, found pen, 
    //if more than 1 in each found palm
    if ((pointToAnalyze[firstSet].size() == 1) && (pointToAnalyze[secondSet].size() == 1) && (pointToAnalyze[thirdSet].size() == 1) && (pointToAnalyze[forthSet].size() == 1))
    {
        penPresent = true;

        //There is no need to check for NULL since we are guarantee that there is 1 point in each set
        mPen.setArea(pointToAnalyze[forthSet][0]->getColumn(), pointToAnalyze[forthSet][0]->getRow());
        mX = pointToAnalyze[forthSet][0]->getX();
        mY = pointToAnalyze[forthSet][0]->getY();

        scribble->screenPressEvent(pointToAnalyze[firstSet][0]);
        //std::cout<<"Press -         findPalm()"<<std::endl;
        pointToAnalyze[firstSet][0] = NULL;

        scribble->screenMoveEvent(pointToAnalyze[secondSet][0]);
        pointToAnalyze[secondSet][0] = NULL;

        scribble->screenMoveEvent(pointToAnalyze[thirdSet][0]);
        pointToAnalyze[thirdSet][0] = NULL;

        scribble->screenMoveEvent(pointToAnalyze[forthSet][0]);
        pointToAnalyze[forthSet][0] = NULL;

        updatePosition();

        return;
    }

    int count = 0;
    if (pointToAnalyze[firstSet].size() > 1)
    {
        count++;
    }

    if (pointToAnalyze[secondSet].size() > 1)
    {
        count++;
    }

    if (pointToAnalyze[thirdSet].size() > 1)
    {
        count++;
    }

    if (pointToAnalyze[forthSet].size() > 1)
    {
        count++;
    }
    
    /*
    count = (pointToAnalyze[firstSet].size() > 1) ? count++ : count;
    count = (pointToAnalyze[secondSet].size() > 1) ? count++ : count;
    count = (pointToAnalyze[thirdSet].size() > 1) ? count++ : count;
    count = (pointToAnalyze[forthSet].size() > 1) ? count++ : count;
     */
    
    //Check if more than one point is each of the four last sets
    //if true set all the points in the four consecutive sets as part of the palm
    if (count > 2)//else //if ((pointToAnalyze[firstSet].size() > 1) && (pointToAnalyze[secondSet].size() > 1) && (pointToAnalyze[thirdSet].size() > 1) && (pointToAnalyze[forthSet].size() > 1))
    {
        for (ushort i = 0; i < pointToAnalyze[firstSet].size(); i++)
        {
            if (pointToAnalyze[firstSet][i] != NULL)
            {
                mPalm.set(pointToAnalyze[firstSet][i]->getColumn(), pointToAnalyze[firstSet][i]->getRow());
            }
        }

        for (ushort j = 0; j < pointToAnalyze[secondSet].size(); j++)
        {
            if (pointToAnalyze[secondSet][j] != NULL)
            {
                mPalm.set(pointToAnalyze[secondSet][j]->getColumn(), pointToAnalyze[secondSet][j]->getRow());
            }
        }

        for (ushort k = 0; k < pointToAnalyze[thirdSet].size(); k++)
        {
            if (pointToAnalyze[thirdSet][k] != NULL)
            {
                mPalm.set(pointToAnalyze[thirdSet][k]->getColumn(), pointToAnalyze[thirdSet][k]->getRow());
            }
        }

        for (ushort l = 0; l < pointToAnalyze[forthSet].size(); l++)
        {
            if (pointToAnalyze[forthSet][l] != NULL)
            {
                mPalm.set(pointToAnalyze[forthSet][l]->getColumn(), pointToAnalyze[forthSet][l]->getRow());
            }
        }
    }

    updatePosition();
}