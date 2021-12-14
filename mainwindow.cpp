#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <stack>
#include <QDebug>
#include <QGraphicsEllipseItem>
//2020302111158陈慧南人工智能引论大作业 A*算法部分
//无向图2点最短路径问题


using namespace std;

#define MAX_NUM 100
int NUM_NODE = 20;
int  NUM_EDGE = 25;
class node {
public:
    int num;  //编号
    float x;  //坐标
    float y;
    node(int nodei,float xi = 0, float yi = 0) :num(nodei),x(xi), y(yi) {}
};

//边
class edge {
public:
    int from;
    int to;
    edge(int fromi, int toi) :from(fromi), to(toi) {}
};
//点集和边集
vector<node> nodes;
vector<edge> edges;
int previousNode[MAX_NUM];//存储最优的上一个节点。
float g[MAX_NUM];//g（x）


int indexOfStart;//起始点
int indexOfEnd;//终点
bool whetherContinue = true; //（是否达到目标）

//求欧拉距离
float distance(const node& a, const node& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
//求欧拉距离
float distance(int a, int  b) {
    return distance(nodes[a], nodes[b]);
}


QGraphicsScene *scene;
//初始化数据
void initData() {
    QPen dashdot_line;
    dashdot_line.setStyle(Qt::DashDotLine);
    nodes.clear();
    edges.clear();
    if(scene!=nullptr)
        delete scene;
    QRect rect(0,0,380,380);   //坐标空间380*380
    scene = new QGraphicsScene(rect);
    srand(rand());
    whetherContinue = true;
    for (int i = 0; i < NUM_NODE; ++i) {
        node tmp(i, (380 * rand())/RAND_MAX, (380 * rand()) / RAND_MAX); //随机生成点的坐标
        if(i==0||i==NUM_NODE-1)
            scene->addEllipse(tmp.x-5,tmp.y-5,10,10);   //大⚪代表起点和终点
        else scene->addEllipse(tmp.x-2.5,tmp.y-2.5,5,5);//小⚪代表普通的节点
        nodes.push_back(tmp);
    }
    for (int i = 0; i < MAX_NUM; ++i){
        previousNode[i] = -1;
        g[i]=0;
    }

    for (int i = 0; i < NUM_EDGE; ++i) {
        edge temp(rand() % NUM_NODE, rand() % NUM_NODE); //随机产生边
        edges.push_back(temp);
        edge temp2(temp.to,temp.from);
        edges.push_back(temp2);
        scene->addLine(nodes[temp.from].x,nodes[temp.from].y,nodes[temp.to].x,nodes[temp.to].y,dashdot_line);
    }

//    for (auto i : edges) {
//        cout << "(" << i.from << " to " << i.to << ")" << endl;
//    }
//    for (auto i : nodes) {
//        cout << i.num << ":";
//        cout << "(" << i.x << "," << i.y << ")" << endl;
//    }
    indexOfEnd = NUM_NODE -1; //设置终点，因为电和边都是随机产生，所以起点终点不需要随机
    indexOfStart = 0;//设置起点
    g[indexOfStart] = 0;  //起点代价设为0


}

//h(x)为直线距离，永远小于等于实际上的最优解的费用,所以为Astar
float h(int indexOfNode) {
    return distance(nodes[indexOfEnd], nodes[indexOfNode]);
}

// f(x)=g(x)+h(x)
float f(int indexOfNode) {
    return g[indexOfNode] + h(indexOfNode);
}

//用于从大到小排序open表的节点
bool compare(int a, int b) {
    return g[a] > g[b];
}

void Astar(MainWindow* mainwindow) {
    mainwindow->ui->textBrowser->setText("");
    vector<int> open; //open表
    vector<int> close;//close表
    open.push_back(indexOfStart);
    int times = 0;
    while (!open.empty() && whetherContinue) {
        node temp = open.back(); //选最优节点
        open.pop_back();
        close.push_back(temp.num);
        if (temp.num == indexOfEnd) {  //是否是目标节点
            whetherContinue = false;
            break;
        }
        vector<int> sons;   //后继节点集
        for (unsigned int i = 0; i < edges.size(); ++i) {
            if (edges[i].from == temp.num) {
                if (find(begin(close), end(close), edges[i].to) == end(close)&& find(begin(open), end(open), edges[i].to) == end(open)) {
                    open.push_back(edges[i].to);
                    //未在open close表内，计算g（x）且加入open表

                    g[edges[i].to] = distance(edges[i].to, edges[i].from)+g[edges[i].from];
                    previousNode[edges[i].to] = edges[i].from;
                }else {
                    //在open或者close表里，计算当前g（x），与历史值比较
                    if (previousNode[edges[i].to]==-1||distance(edges[i].to, edges[i].from) + g[edges[i].from] < g[edges[i].to]){
                        g[edges[i].to] = distance(edges[i].to, edges[i].from) + g[edges[i].from];
                        previousNode[edges[i].to] = edges[i].from;
                    }
                }
            }
        }
        //open表排序
        sort(begin(open), end(open), compare);


        //输出日志信息
        string s = "Iteration ";
        s+=to_string(++times);
        s+=":\nOpen: ";
        for (auto i : open) {
            s+= to_string(i);
            s+=" ";
        }
        s+="\nClosed: ";
        for (auto i : close) {
            s+= to_string(i);
            s+=" ";
        }
//        s+= "\n g(n): ";
//        for (int i = 0;i<NUM_NODE;++i)
//        {
//            s+= to_string(g[i]);
//            s+=" ";
//        }
        s+= "\n\n";
        mainwindow->show_text(s);


    }
    mainwindow->show_text("A-star complete");

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    //失败，弹窗
    if (whetherContinue)
    {
        cout << "No solution" << endl;
        Dialog* a = new Dialog();
        a->show();
    }
    //成功，画出最优路径
    else {
        int i = indexOfEnd;
        do {
            cout << i ;
            scene->addLine(nodes[i].x,nodes[i].y,nodes[previousNode[i]].x,nodes[previousNode[i]].y,pen);
            i = previousNode[i];
        }
        while (indexOfStart != i);
        mainwindow->show_text("\nShortest distance:");mainwindow->show_text(to_string(g[indexOfEnd]));
        mainwindow->show_text("\nPath: \n");
        stack<int> path;
        int jj = indexOfEnd;
        do{
            path.push(jj);
            jj = previousNode[jj];
        }while(jj!=indexOfStart);
        path.push(jj);
        while(!path.empty()){
            mainwindow->show_text(to_string(path.top()));
            path.pop();
            mainwindow->show_text("->");
        }
        mainwindow->show_text("OK");
        mainwindow->ui->lcdNumber_3->display(g[indexOfEnd]);
    }
}
//a*算法结束，之后就是qt框架相关代码

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    Astar(this);

}
void stdString2QString(std::string str, QString &qs)
{
    qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
}

void MainWindow::on_pushButton_2_clicked()
{
    initData();
    ui->lcdNumber->display(NUM_NODE);
    ui->lcdNumber_2->display(NUM_EDGE);
    ui->graphicsView->setScene(scene);
}

void MainWindow::show_text(std::string a){
    QString tmp;
    stdString2QString(a,tmp);
    //cout<<a<<endl;
    qDebug()<<tmp<<'\n';
    ui->textBrowser->insertPlainText(tmp);
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->repaint();
}


void MainWindow::on_textBrowser_textChanged()
{
    cout<<"changed"<<endl;
}

