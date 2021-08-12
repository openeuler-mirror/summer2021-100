#include "mainCalendar.h"
#include <QDebug>
#include <QMouseEvent>

MainCalendar::MainCalendar(QWidget *parent):
    QWidget(parent)
{

    bgColor = QColor(170, 255, 255, 255);
    textColor = QColor(0, 0, 0, 255);
    shadowColor = QColor(0, 255, 255, 255);
    selectColor = QColor(0, 255, 255, 255);

    updateCalendar(this->selectDate);

    //月份更新
    for(int i = 0; i < 3 ;i++){
       for(int j = 0; j < 4;j++){
            monthSelect[i][j] = i * 4 + j + 1;
       }
    }

    //年份更新
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            yearSelect[i][j] = this->today.year() + i * 3 + j;
        }
    }

}

void MainCalendar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //设置抗锯齿
    painter.setRenderHints(QPainter::Antialiasing);

    int sw = 336;
    int sh = 336;
    qreal scaleX = this->width() * 1.0 / sw;
    qreal scaleY = this->height() * 1.0 / sh;

    painter.scale(scaleX, scaleY);
    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, sw, sh, bgColor);

    //day_state比例尺
    qreal iw = sw / 7.0;
    qreal ih = sh / 7.0;
    //month_state比例尺
    qreal iwm = sw / 4.0;
    qreal ihm = sw / 3.0;
    //year_state比例尺
    qreal iwy = sw / 3.0;
    qreal ihy = sw / 3.0;

    //mask
    QPointF globalpoint = this->mapFromGlobal(QCursor::pos());
    const QPointF &point = QPointF(globalpoint.x() / scaleX, globalpoint.y() / scaleY);

    //绘制光晕背景

    switch (this->STATE) {
        default:

        //选择日期
        case day_state:{

            //渲染鼠标位置
            if (this->underMouse()) {
                int effectradius = 58;
                painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                QRadialGradient radialGrad(point, effectradius);
                radialGrad.setColorAt(0, QColor(0, 0, 0, 120));
                radialGrad.setColorAt(1, QColor(0, 0, 0, 255));
                painter.setBrush(radialGrad);
                painter.drawEllipse(point, effectradius, effectradius);

                painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
                painter.setBrush(Qt::NoBrush);

                for (int row = 0; row < 6; row++) {
                    for (int column = 0; column < 7; column++) {
                        QRectF rect = QRectF(column * iw, (row + 1) * ih, iw, ih).adjusted(3, 3, -3, -3);
                        if (rect.contains(point)) {
                            painter.save();
                            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                            painter.setPen(QPen(selectColor, 2));
                            painter.drawRoundedRect(rect, 2, 2);
                            painter.restore();
                            continue;
                        } else {
                            painter.setPen(QPen(shadowColor, 2));
                        }

                        painter.drawRoundedRect(rect, 2, 2);
                    }
                }

                //绘制圆形的光晕底层背景
                painter.fillRect(0, 0, sw, sh, bgColor);
            }



            //绘制头部中文数字,先设置图像叠加模式为源在上面
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.setPen(textColor);
            QStringList listHead;
            listHead << "一" << "二" << "三" << "四" << "五" << "六" << "日";
            for (int i = 0; i < 7; i++) {
                painter.drawText(i * iw, 0, iw, ih, Qt::AlignCenter, listHead.at(i));
            }

            //绘制日期
            for (int row = 0; row < 6; row++) {
                for (int column = 0; column < 7; column++) {
                    if (dateItem[row][column].day > 0) {
                        QRectF rect = QRectF(column * iw, (row + 1) * ih, iw, ih).adjusted(3, 3, -3, -3);

                        //如果是选中的日期则突出绘制背景
                        if (QDate::currentDate() == QDate(dateItem[row][column].year, dateItem[row][column].month, dateItem[row][column].day)) {
                            painter.setPen(QPen(selectColor, 2));
                            painter.setBrush(Qt::NoBrush);

                            //如果和光晕效果重叠则边框高亮
                            if (rect.contains(point)) {
                                painter.setPen(QPen(selectColor.lighter(), 2));
                            }

                            //绘制里边背景
                            painter.setPen(Qt::NoPen);
                            painter.setBrush(selectColor);
                            painter.drawRoundedRect(rect.adjusted(4, 4, -4, -4), 2, 2);
                        }

                        if (this->selectDate == QDate(dateItem[row][column].year, dateItem[row][column].month, dateItem[row][column].day)) {
                            painter.setPen(QPen(selectColor, 2));
                            painter.setBrush(Qt::NoBrush);

                            //如果和光晕效果重叠则边框高亮
                            if (rect.contains(point)) {
                                painter.setPen(QPen(selectColor.lighter(), 2));
                            }

                            //绘制圆角边框
                            painter.drawRoundedRect(rect, 2, 2);

                        }

                        painter.setPen(textColor);
                        painter.drawText(rect, Qt::AlignCenter, QString::number(dateItem[row][column].day));

                        if(this->dateItem[row][column].daily_ScheduleList.length() != 0){

                            painter.save();

                            QRectF RedPoint = QRectF(column * iw + (iw*2)/3, (row + 1) * ih + ih/6, iw/6, ih/6);
                            QFont RedPointFont;
                            RedPointFont.setPointSize(iw/8);
                            painter.setFont(RedPointFont);

                            painter.setPen(QColor(239, 41, 41));
                            painter.setBrush(QColor(239, 41, 41));

                            painter.drawEllipse(RedPoint);

                            painter.setPen(QColor(186, 189, 182));
                            painter.drawText(RedPoint.adjusted(0, -1, 0, -1), Qt::AlignCenter, QString::number(dateItem[row][column].daily_ScheduleList.length()));

                            painter.restore();
                        }
                    }
                }
            }
            break;
        }

        //选择月份
        case month_state:{

            if (this->underMouse()) {
                int effectradius = 58;
                painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                QRadialGradient radialGrad(point, effectradius);
                radialGrad.setColorAt(0, QColor(0, 0, 0, 120));
                radialGrad.setColorAt(1, QColor(0, 0, 0, 255));
                painter.setBrush(radialGrad);
                painter.drawEllipse(point, effectradius, effectradius);

                painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
                painter.setBrush(Qt::NoBrush);

                for (int row = 0; row < 3; row++) {
                    for (int column = 0; column < 4; column++) {
                        QRectF rect = QRectF(column * iwm, row * ihm, iwm, ihm).adjusted(3, 3, -3, -3);
                        if (rect.contains(point)) {
                            painter.save();
                            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                            painter.setPen(QPen(selectColor, 2));
                            painter.drawRoundedRect(rect, 2, 2);
                            painter.restore();
                            continue;
                        } else {
                            painter.setPen(QPen(shadowColor, 2));
                        }

                        painter.drawRoundedRect(rect, 2, 2);
                    }
                }

                //绘制圆形的光晕底层背景
                painter.fillRect(0, 0, sw, sh, bgColor);
            }



            //绘制头部中文数字,先设置图像叠加模式为源在上面
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.setPen(textColor);

            //绘制日期
            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 4; column++) {
                    QRectF rect = QRectF(column * iwm, row * ihm, iwm, ihm).adjusted(3, 3, -3, -3);

                    //如果是选中的日期则突出绘制背景
                    if (QDate::currentDate().month() == monthSelect[row][column] && QDate::currentDate().year() == this->selectDate.year()) {
                        painter.setPen(QPen(selectColor, 2));
                        painter.setBrush(Qt::NoBrush);

                        //如果和光晕效果重叠则边框高亮
                        if (rect.contains(point)) {
                            painter.setPen(QPen(selectColor.lighter(), 2));
                        }

                        //绘制里边背景
                        painter.setPen(Qt::NoPen);
                        painter.setBrush(selectColor);
                        painter.drawRoundedRect(rect.adjusted(4, 4, -4, -4), 2, 2);
                    }

                    painter.setPen(textColor);
                    painter.drawText(rect, Qt::AlignCenter, QString::number(monthSelect[row][column]).append("月"));
                }
            }
            break;
        }

    case year_state:{

        if (this->underMouse()) {
            int effectradius = 58;
            painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            QRadialGradient radialGrad(point, effectradius);
            radialGrad.setColorAt(0, QColor(0, 0, 0, 120));
            radialGrad.setColorAt(1, QColor(0, 0, 0, 255));
            painter.setBrush(radialGrad);
            painter.drawEllipse(point, effectradius, effectradius);

            painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
            painter.setBrush(Qt::NoBrush);

            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 3; column++) {
                    QRectF rect = QRectF(column * iwy, row * ihy, iwy, ihy).adjusted(3, 3, -3, -3);
                    if (rect.contains(point)) {
                        painter.save();
                        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                        painter.setPen(QPen(selectColor, 2));
                        painter.drawRoundedRect(rect, 2, 2);
                        painter.restore();
                        continue;
                    } else {
                        painter.setPen(QPen(shadowColor, 2));
                    }

                    painter.drawRoundedRect(rect, 2, 2);
                }
            }

            //绘制圆形的光晕底层背景
            painter.fillRect(0, 0, sw, sh, bgColor);
        }



        //绘制头部中文数字,先设置图像叠加模式为源在上面
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setPen(textColor);

        //绘制日期
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                QRectF rect = QRectF(column * iwy, row * ihy, iwy, ihy).adjusted(3, 3, -3, -3);

                //如果是选中的日期则突出绘制背景
                if (QDate::currentDate().year() == yearSelect[row][column]) {
                    painter.setPen(QPen(selectColor, 2));
                    painter.setBrush(Qt::NoBrush);

                    //如果和光晕效果重叠则边框高亮
                    if (rect.contains(point)) {
                        painter.setPen(QPen(selectColor.lighter(), 2));
                    }

                    //绘制里边背景
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(selectColor);
                    painter.drawRoundedRect(rect.adjusted(4, 4, -4, -4), 2, 2);
                }

                painter.setPen(textColor);
                painter.drawText(rect, Qt::AlignCenter, QString::number(yearSelect[row][column]).append("年"));
            }
        }
        break;
    }
    }

}

void MainCalendar::updateCalendar(const QDate &selectDate)
{
    for(int i = 0;i < 6;i++){
        for(int j = 0;j < 7;j++){
            this->dateItem[i][j].year = -1;
            this->dateItem[i][j].month = -1;
            this->dateItem[i][j].day = -1;
        }
    }

    QDate today = selectDate;
    QDate theFirstDay = QDate(today.year(), today.month(), 1);

    int allDays = theFirstDay.daysInMonth();
    int row = 0;
    int col = 0;

    for(int i = 0;i < allDays;i++){
        col = theFirstDay.addDays(i).dayOfWeek() - 1;
        this->dateItem[row][col].year = theFirstDay.year();
        this->dateItem[row][col].month = theFirstDay.month();
        this->dateItem[row][col].day = theFirstDay.addDays(i).day();
        if(theFirstDay.addDays(i).dayOfWeek() == 7)
            row++;
    }
}

MainCalendar::~MainCalendar()
{

}


void MainCalendar::setSelectDate(const QDate &selectDate)
{
    this->selectDate = selectDate;
    updateCalendar(this->selectDate);
}

void MainCalendar::mousePressEvent(QMouseEvent *event)
{
    int sw = 336;
    int sh = 336;
    qreal scaleX = this->width() * 1.0 / sw;
    qreal scaleY = this->height() * 1.0 / sh;

    //day_state比例尺
    qreal iw = sw / 7.0;
    qreal ih = sh / 7.0;
    //month_state比例尺
    qreal iwm = sw / 4.0;
    qreal ihm = sw / 3.0;
    //year_state比例尺
    qreal iwy = sw / 3.0;
    qreal ihy = sw / 3.0;

    //mask
    switch (this->STATE) {
    default:
    case day_state:{

        QPointF globalpoint = this->mapFromGlobal(QCursor::pos());
        const QPointF &point = QPointF(globalpoint.x() / scaleX, globalpoint.y() / scaleY);

        if(event->button() == Qt::LeftButton){
            for (int row = 0; row < 6; row++) {
                for (int column = 0; column < 7; column++) {
                    QRectF rect = QRectF(column * iw, (row + 1) * ih, iw, ih).adjusted(3, 3, -3, -3);
                    if (rect.contains(point) && dateItem[row][column].day != -1) {
                        this->selectDate.setDate(this->selectDate.year(), this->selectDate.month(), this->dateItem[row][column].day);
                        break;
                    }
                }
            }
        }
        break;
    }

    case month_state:{

        QPointF globalpoint = this->mapFromGlobal(QCursor::pos());
        const QPointF &point = QPointF(globalpoint.x() / scaleX, globalpoint.y() / scaleY);

        if(event->button() == Qt::LeftButton){
            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 4; column++) {
                    QRectF rect = QRectF(column * iwm, row * ihm, iwm, ihm).adjusted(3, 3, -3, -3);
                    if (rect.contains(point)) {
                        this->selectDate.setDate(this->selectDate.year(), this->monthSelect[row][column], 1);
                        qDebug() << this->monthSelect[row][column];
                        break;
                    }
                }
            }
        }

        updateCalendar(this->selectDate);
        this->STATE = day_state;
        break;
    }
    case year_state:{

        QPointF globalpoint = this->mapFromGlobal(QCursor::pos());
        const QPointF &point = QPointF(globalpoint.x() / scaleX, globalpoint.y() / scaleY);

        if(event->button() == Qt::LeftButton){
            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 3; column++) {
                    QRectF rect = QRectF(column * iwy, row * ihy, iwy, ihy).adjusted(3, 3, -3, -3);
                    if (rect.contains(point)) {
                        this->selectDate.setDate(this->yearSelect[row][column], this->selectDate.month(), this->selectDate.day());
                        break;
                    }
                }
            }
        }


        this->STATE = month_state;
        break;
    }
    }

}

void MainCalendar::setPageState()
{
    if(this->STATE == day_state){
        this->STATE = month_state;
    }
    else if(this->STATE == month_state){
        this->STATE = year_state;
    }
}

QDate MainCalendar::readSelectDate()
{
    return this->selectDate;
}

QString MainCalendar::readPageState()
{
    QString page_state = "day_select";

    if(this->STATE == year_state){
        page_state.clear();
        page_state = "year_select";
    }
    else if(this->STATE == month_state){
        page_state.clear();
        page_state = "month_select";
    }
    else if(this->STATE == day_state){
        page_state.clear();
        page_state = "day_select";
    }
    return page_state;
}

void MainCalendar::setBgColor(const QColor &bgcolor)
{
    this->bgColor = bgcolor;
}

void MainCalendar::setTextColor(const QColor &textcolor)
{
    this->textColor = textcolor;
}

void MainCalendar::setSelectColor(const QColor &selectColor)
{
    this->selectColor = selectColor;
}

void MainCalendar::setShadowColor(const QColor &shadowcolor)
{
    this->shadowColor = shadowcolor;
}
