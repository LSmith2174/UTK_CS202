/*Laura Smith 10/31/22 4:20 pm 
This is a header file for a 2048 game which had three private variables and 4 public variables and functions.
This is similar to the connect 4 lab in which there are columns that make up the board but instead of just stacking
this adds together the dropped value if its the same as the value below, otherwise it stacks the same. The game only
ends when a stack reaches the top. 
*/

#include <iostream>

class 2048col {
    private:
        //private variables and the columns of the board
        int num;
        int droppingNum;
        int currCol[6];

    public:
        //functions to keep the game running correctly
        2048Col();
        int isFull(); //checks if a column is full
        int total();  //adds the total when two of the same number touch
        void addNum(droppingNum);   //adds the number to the board
};