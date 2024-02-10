#ifndef LOCATION_H_
#define LOCATION_H_

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    bool operator==(const Location& l2) const { return (this->row == l2.row && this->col == l2.col); }
    bool operator!=(const Location& l2) const { return (this->row != l2.row || this->col != l2.col); }
    bool operator<(const Location& l2) const { return (this->row < l2.row && this->col < l2.col); }
    void operator=(const Location& l2) { this->row = l2.row; this->col = l2.col; }
    void operator++() { this->row++; this->col++; }
};

#endif //LOCATION_H_
