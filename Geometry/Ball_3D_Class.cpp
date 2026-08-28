/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: 3D CLOSEST PAIR OF POINTS (DIVIDE & CONQUER)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Minimum Euclidean distance in 3D", "Closest points in space", "N up to 10^5".
 * - Classic Scenarios: You are given N points in a 3D coordinate system. Finding the closest 
 *   pair naively takes O(N^2), which will Time Limit Exceed (TLE) for N > 10^4.
 * - The Magic: Divide and Conquer! We sort the points by the X-axis and recursively divide the 
 *   space into halves. The tricky part in 3D is merging the halves (the "strip"). If we just 
 *   check all points in the strip naively, it can still degrade to O(N^2). 
 *   To fix this, the algorithm groups the strip into 1D "columns" along the Z-axis using the 
 *   current minimum distance `d` as the column width. This brilliant trick guarantees that each 
 *   point only checks a strict O(1) number of neighboring points. The overall complexity 
 *   beautifully drops to O(N log N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Create a `BallManager` object. It automatically reads `N` from `std::cin`, 
 *   followed by the `X Y Z` coordinates of the `N` points.
 *       BallManager bm;
 *
 * - Execution: Call the main solver function to get the minimum distance.
 *       long double min_distance = bm.ClosestPair();
 *
 * - Complexity:
 *       Time: O(N log N) — The X and Y sorting takes O(N log N), and the recursive strip 
 *             processing takes O(N) per level.
 *       Space: O(N) to store the objects, vectors, and recursion stack.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Decoupling I/O: The constructor currently reads input directly. For better flexibility, 
 *   you can modify the constructor to accept a `vector<vector<ll>>` or `vector<Ball>` instead 
 *   of using `cin` inside the class.
 * - Precision Issues: The algorithm uses `long double` and `sqrtl`. While this is highly accurate, 
 *   some problems require exact integer squared distances. You can return the squared distance 
 *   at the very end by squaring `finalMinDist`, but do NOT remove `sqrtl` from `DistanceTo`, 
 *   as the `SearchStrip` bucketing logic mathematically relies on the true linear distance.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;
#define ll long long
#define ld long double

class Ball
{
public:
    ll posX, posY, posZ; //Position values in 3D space
    ll columnNumber[3]; //ID number of the column which ball belongs to (left, middle, right)
    ll columnIndex[3]; //Position inside the column (left, middle, right)
    
    // Time Complexity: O(1)
    // تقوم بحساب المسافة في وقت ثابت لأنها تنفذ عمليات حسابية بسيطة ومباشرة.
    ld DistanceTo(Ball* targetBall)
    {
        ll squareDistX = (posX - targetBall->posX) * (posX - targetBall->posX);
        ll squareDistY = (posY - targetBall->posY) * (posY - targetBall->posY);
        ll squareDistZ = (posZ - targetBall->posZ) * (posZ - targetBall->posZ);
        return sqrtl(squareDistX + squareDistY + squareDistZ);
    }
};

class BallManager
{
    ll ballCount = 0; //Total number of balls
    Ball* ballArray; //Dynamically allocated ball array which holds all balls inside
 
    // Time Complexity: O(1)
    static bool CompareX(Ball ball1, Ball ball2)
    {
        return (ball1.posX < ball2.posX);
    }
    
    // Time Complexity: O(1)
    static bool CompareY(Ball* ball1, Ball* ball2)
    {
        return (ball1->posY < ball2->posY);
    }
 
    // Time Complexity: O(N^2) 
    // حيث N هو حجم المتجه (balls.size()). تقارن كل نقطة بكل النقاط الأخرى.
    ld NaiveMethod(std::vector<Ball*> balls)
    {
        int size = balls.size();
        ld minDist = std::numeric_limits<double>::max();
 
        //Take balls one by one
        for (int i = 0; i < size - 1; ++i)
        {
            //Check other remaining balls
            for (int j = i + 1; j < size; ++j)
            {
                ld dist = balls[i]->DistanceTo(balls[j]);
                if (dist < minDist)
                    minDist = dist;
            }
        }
        return minDist;
    }
    
    // Time Complexity: O(S) 
    // حيث S هو عدد الكرات داخل الشريط (stripSize). 
    // بفضل بناء الأعمدة (Columns) بناءً على محور Z، الحلقة الداخلية المزدوجة لا تنفذ إلا لعدد 
    // ثابت وصغير جداً من النقاط المجاورة لكل نقطة (محدود هندسياً في الفضاء ثلاثي الأبعاد).
    ld SearchStrip(std::vector<Ball*> strip, ld stripDist)
    {
        //Save the strip distance value as minimum distance known
        ld minDist = stripDist;
 
        //If ball count in strip is less than 2, return strip distance
        int stripSize = strip.size();
        if (stripSize < 2)
            return minDist;
 
        //If ball count in strip is 2 or 3, find distance with naive method
        if (stripSize < 4)
        {
            ld newDist = NaiveMethod(strip);
            if (newDist < minDist)
                minDist = newDist;
            return minDist;
        }
 
        //If ball count is more than 3, start processing of creating columns
        //Find the smallest z value among balls -> O(S)
        int smallestZ = strip[0]->posZ;
        for (int i = 1; i < stripSize; i++)
            if (strip[i]->posZ < smallestZ)
                smallestZ = strip[i]->posZ;
 
        //Find the largest z value among balls -> O(S)
        int largestZ = strip[0]->posZ;
        for (int i = 1; i < stripSize; i++)
            if (strip[i]->posZ > largestZ)
                largestZ = strip[i]->posZ;
 
        //Find range between z values and column count
        int rangeZ = largestZ - smallestZ;
        int columnCount = (int)(rangeZ / stripDist) + 1;
 
        //Create an array of ball pointer vectors to put pointers of balls into proper columns
        std::vector<Ball*>* columns = new std::vector<Ball*>[columnCount];
 
        //Put all balls into proper columns (Phase 1) -> O(S)
        //Start picking balls one by one with the smallest Y first
        for (int i = 0; i < stripSize; i++)
        {
            //Find middle(main) column of ball, put ball into it, save index
            strip[i]->columnNumber[1] = (ll)((ld)(strip[i]->posZ - smallestZ) / stripDist);
            columns[strip[i]->columnNumber[1]].push_back(strip[i]);
            strip[i]->columnIndex[1] = columns[strip[i]->columnNumber[1]].size() - 1;
 
            //Find and save left and right columns of ball
            strip[i]->columnNumber[0] = strip[i]->columnNumber[1] - 1;
            strip[i]->columnNumber[2] = strip[i]->columnNumber[1] + 1;
 
            //There is no column at left of leftmost column, and right of rightmost column
            //Save indices in left and right columns
            if (strip[i]->columnNumber[1] != 0) //If not leftmost column
                strip[i]->columnIndex[0] = columns[strip[i]->columnNumber[0]].size() - 1;
 
            if (strip[i]->columnNumber[1] != columnCount - 1) //If not rightmost column
                strip[i]->columnIndex[2] = columns[strip[i]->columnNumber[2]].size() - 1;
        }
 
        //Find if such lesser distance exists between any pair of balls inside strip (Phase 2)
        // O(S) overall. The inner loops run a constant number of times O(1) per ball.
        //Start picking balls one by one with the smallest Y first
        for (int i = 0; i < stripSize; i++)
        {
            //For left, middle and right columns of the selected ball
            for (int j = 0; j < 3; j++)
            {
                //Skip invalid out of bounds columns
                if ((strip[i]->columnNumber[j] == -1) || (strip[i]->columnNumber[j] == columnCount))
                    continue;
 
                //Look through increasing Y value = O(1)
                for (int k = strip[i]->columnIndex[j] + 1; k < (ll)columns[strip[i]->columnNumber[j]].size(); k++)
                {
                    //If Y distance is already more than strip distance, no need to check remaning balls in column
                    if (columns[strip[i]->columnNumber[j]][k]->posY > strip[i]->posY + stripDist)
                        break;
 
                    //Check distance and if it is smaller than current minimum distance, save it as new minimum distance
                    ld newDist = strip[i]->DistanceTo(columns[strip[i]->columnNumber[j]][k]);
                    if (newDist < minDist)
                        minDist = newDist;
                }
            }
        }
 
        //Delete dynamically allocated columns array
        delete[] columns;
 
        //Return minimum distance found in strip or initially given strip distance
        return minDist;
    }
 
    // Time Complexity: T(N) = 2*T(N/2) + O(N) => O(N log N)
    // حيث N هو حجم المصفوفة الحالية (sortedY.size()). الدالة تقسم المسألة لنصفين، ثم تدمجهما في O(N).
    ld RecursiveCall(int sortedXPos, std::vector<Ball*> sortedY)
    {
        //If size is small enough, find distance with naive method -> O(1)
        int size = sortedY.size();
        if (size < 4)
            return NaiveMethod(sortedY);
 
        //Get middle ball for X axis
        int midIndex = sortedXPos + size / 2;
        Ball* midBall = &ballArray[midIndex];
 
        //Initiliaze left and right subarrays
        std::vector<Ball*> leftVector;
        std::vector<Ball*> rightVector;
 
        //In case of there are other balls which share same X position as middle point
        bool putToRightArr = true;
 
        //Distribute balls into left and right vectors -> O(N)
        for (int i = 0; i < size; i++)
        {
            if (sortedY[i]->posX < midBall->posX)
                leftVector.push_back(sortedY[i]);
            else if (sortedY[i]->posX > midBall->posX)
                rightVector.push_back(sortedY[i]);
            else //if (sortedY[i].posX == midBall.posX)
            {
                if (putToRightArr)
                    rightVector.push_back(sortedY[i]);
                else
                    leftVector.push_back(sortedY[i]);
                putToRightArr = !putToRightArr;
            }
        }
 
        //Pass subarrays to same function recursively -> 2 * T(N/2)
        ld minLeftDist = RecursiveCall(sortedXPos, leftVector);
        ld minRightDist = RecursiveCall(sortedXPos + leftVector.size(), rightVector);
 
        //Initiliaze strip vector
        std::vector<Ball*> strip;
        ld stripDist = std::min(minLeftDist, minRightDist);
 
        //Put balls which are close enough to middle into strip -> O(N)
        for (int i = 0; i < size; i++)
        {
            ll distToMid = sortedY[i]->posX - midBall->posX;
            if (distToMid < 0)
                distToMid *= -1;
            if (distToMid < stripDist)
                strip.push_back(sortedY[i]);
        }
 
        //Search in strip and return the minimum distance -> O(N) (Since strip size <= N)
        return SearchStrip(strip, stripDist);
    }
 
public:
    // Time Complexity: O(N) 
    // حيث N هو عدد الكرات (ballCount)، لأنها تقوم فقط بقراءة الدخل وتخصيص الذاكرة.
    BallManager(){
        cin >> ballCount;
        ballArray = new Ball[ballCount];
        for (int i = 0; i < ballCount; ++i) {
            cin >> ballArray[i].posX;
            cin >> ballArray[i].posY;
            cin >> ballArray[i].posZ;
        }
    }
    
    // Time Complexity: O(1)
    ~BallManager() { delete[] ballArray;};
    
    // Time Complexity: O(N log N)
    // الدالة الرئيسية التي تستدعي باقي الدوال وتتحكم في سير الخوارزمية الكلي.
    ld ClosestPair()
    {
        //Sort original ballArray by increasing X order = O(N log N)
        std::sort(ballArray, ballArray + ballCount, &BallManager::CompareX);
 
        //Initiliaze sortedY vector -> O(N)
        std::vector<Ball*> sortedY;
        for (int i = 0; i < ballCount; i++)
            sortedY.push_back(&ballArray[i]);
 
        //Sort sortedY by increasing Y order = O(N log N)
        std::sort(sortedY.begin(), sortedY.end(), &BallManager::CompareY);
 
        //Start recursive part of the program and get result -> O(N log N)
        ld finalMinDist = RecursiveCall(0, sortedY);
 
        //Return the final minimum distance
        return finalMinDist;
    }
};
