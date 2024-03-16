/**
 * Functional and reusable class for representing a pie chart widget
 * Component Developer: Musa Yiğit Yayla
 * !!!Important!!!
 * The given input vectors are expected to be of the same length and must represent the values regarding each slice
 * */

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <vector>
#include <string>
#include <cstdlib>


#ifndef PIECHART_H
#define PIECHART_H

using namespace std;
class PieChart: public QWidget{
private:
    vector<double> contentValues;
    vector<string> contentHeaders;
    bool displayValuesEnabled = true; //true by default

public:
    PieChart(QWidget* parent = nullptr);
    PieChart(vector<double>& contentValues, vector<string>& contentHeaders, QWidget* parent = nullptr);
    PieChart(vector<double>& contentValues, vector<string>& contentHeaders, int width, int height, QWidget* parent = nullptr);

    vector<double> getContentValues() const;
    vector<string> getContentHeaders() const;
    void setContents(vector<double>& contentValues, vector<string>& contentHeaders);
    void setSize(int width, int height);
    int getWidth() const;
    int getHeight() const;
    void refresh(bool displayValues);

    const int DEFAULT_WIDTH = 600;
    const int DEFAULT_HEIGHT = 600;
protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // PIECHART_H
