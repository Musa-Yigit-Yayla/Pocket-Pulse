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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <cstdlib>
#include <QLabel>


#ifndef PIECHART_H
#define PIECHART_H

using namespace std;
class PieChart: public QWidget{
private:
    vector<double> contentValues;
    vector<string> contentHeaders;
    bool displayValuesEnabled = true; //true by default
    bool redrawHeadersEnabled = false; //will be used to avoid redrawing header rects when paintEvent is not invoked by setContents
    QVBoxLayout* wrapper = new QVBoxLayout(this);
    QHBoxLayout* hboxHeaders = new QHBoxLayout(this);
    QScrollArea* headerSA = new QScrollArea(this);
    QScrollArea* valuesSA = new QScrollArea(this); //scroll area to hold the amount of values in piechart categories
    QVBoxLayout* vboxValues = new QVBoxLayout(this);

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
    QVBoxLayout* getContainerLayout();

    const int DEFAULT_WIDTH = 600;
    const int DEFAULT_HEIGHT = 600;
protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // PIECHART_H
