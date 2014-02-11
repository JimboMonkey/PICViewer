#include "ui_PICViewer.h"

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QDateTime>
#include <QFileDialog>
#include <QPushButton>
#include <QImage>  
#include <QScrollArea>  


#include <stdio.h>
#include <iostream>
#include <string.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0); // constructor
        ~MainWindow();
		void updateLabel(QImage myImage, QString myInfo);        

	private:

    private slots:
       

		//moved to public void updateLabel();

		
        
              
};
