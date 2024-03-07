#ifndef LOCATION_H_
#define LOCATION_H_

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int _row, _col;

    Location()
    {
        _row = _col = 0;
    };

    Location(int r, int c)
    {
        _row = r;
        _col = c;
    };

    inline bool operator==(const Location& l2) const { return (this->_row == l2._row && this->_col == l2._col); }
    inline bool operator!=(const Location& l2) const { return !(*this==l2); }
    inline bool operator<(const Location& l2) const { return this->_row < l2._row || (l2._row >= this->_row && this->_col < l2._col); }
    friend std::ostream& operator<<(std::ostream& os, const Location& obj) {
        os << "row "<<obj._row<< " | col "<<obj._col;
        return os;
    }
};

#endif //LOCATION_H_
