#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


/**
 * Constructor for the "view" object in Model-View architecture.  Contains setup for all graphics items, graphics scene, and a basic Box2D body (to be updated, will not consist of only one dynamicBody).
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , world(b2Vec2(0.0f, -10.f))
{
    //setting the model
    mainModel = &model;

    // ======== View setup (Qt) ======== //
    ui->setupUi(this);

    scene = new QGraphicsScene(this); //creating a new scene
    setSceneSize();
    ui->graphicsView->setScene(scene); //setting the scene of the graphics view


    //background image setup
    QString QImagePath =  ":/medicines/background";
    image.load(QImagePath);
    QPalette palette;
    palette.setBrush(QPalette::Window, image.scaled(this->width(), this->height()));
    setPalette(palette);

    //adds patient to scene
    scene->addItem(mainModel->newPatient);

    // ======== Create all inital 'treatments' ======== //

    //first shelf (left to right)
    createTreatment("cold-pack", ":/medicines/cold-pack.png", 145, -87, 80, 80);
    createTreatment("pepto-bismol", ":/medicines/pepto-bismol.png", 190, -105, 45, 110);
    createTreatment("water", ":/medicines/water.png", 225, -87, 60, 70);
    createTreatment("neosporin", ":/medicines/neosporin.png", 270, -92, 55, 80);

    //second shelf (left to right)
      createTreatment("aloe-vera",":/medicines/aloe-vera.png", 145, -47, 50, 125 );
      createTreatment("allergy" , ":/medicines/allergy.png" , 180, -25, 80, 80);
      createTreatment("bandage" , ":/medicines/bandage.png", 230, -24, 65, 70);
      createTreatment("band-aid", ":/medicines/band-aid.png", 270, -25, 60, 72);
    //third shelf (left to right)
      createTreatment("cough-drops", ":/medicines/cough-drops.png", 145, 30, 70, 80);
      createTreatment("hot-pack", ":/medicines/hot-pack.png", 190, 35, 80, 80);
      createTreatment("nasal-spray", ":/medicines/nasal-spray.png", 235, 25, 40, 90);
      createTreatment("ibuprofen", ":/medicines/ibuprofen.png", 265, 25, 70, 90);
      //fourth shelf (left to right)
     createTreatment("hydrogen-peroxide", ":/medicines/hydrogen-peroxide.png", 147, 78, 50, 115);
     createTreatment("acetaminophen", ":/medicines/acetaminophen.png", 184, 88, 70, 90);
     createTreatment("mask", ":/medicines/mask.png", 240, 110, 80, 50);


    // ======== Box2D initial settings ========

    // Define the gravity vector.
    b2Vec2 gravity(0.0f, -10.0f);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -13.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 8.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // ======== b2Body Set up ======== //

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 5.0f); // set initial positions

    body = world.CreateBody(&bodyDef);

    body->SetSleepingAllowed(true);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    //set restitution (bounciness)
    fixtureDef.restitution = 0.9f;

    //set its inital velocity to 0
    body->SetLinearVelocity(b2Vec2(0,0));

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    //add the body two the mainModel and map it to 'empty' since nothing has been clicked yet
    mainModel->currentTreatment.first = body;
    mainModel->currentTreatment.second = "empty";

    // ======== Connections between signals and slots ======== //


    //update world based off timer
    timer = new QTimer(this);
       connect(timer,
               &QTimer::timeout,
               this,
               &MainWindow::updateWorld);
       timer->start(10);


       for ( auto treatment : mainModel->treatments) {
          connect(treatment.second,
                  &MySquare::notifySceneToResize,
                  this,
                  &MainWindow::setSceneSize);
          }


    //loop through the 'treatments' from main model, i.e mySquare objects,
    //and connect the singal with slot in mainwindow to move update the object's positions when they're moved
    for ( auto treatment : mainModel->treatments) {
       connect(treatment.second,
               &MySquare::sendNewHeightSquare,
               this,
               &MainWindow::receiveNewHeightValue);

       connect(treatment.second, //connects initial collision detection signal from the caller to the slot in model
                   &MySquare::detectCollision,
                   mainModel,
                   &Model::collisionDetectionFromCaller);

       //connects mySquare signal to slot in model to handle incorrect answers
       connect(treatment.second,
               &MySquare::incorrectAnswerChosen,
               mainModel,
               &Model::handleIncorrectAnswer);
       }

    connect(mainModel,
            &Model::loadUI,
            this,
            &MainWindow::loadLevelUI);

    connect(mainModel,
            &Model::showPopSignal,
            this,
            &MainWindow::showPopup);



    mainModel->loadLevel();

    ui->StepPopup->setStyleSheet("background-color: rgb(249, 233, 216); border-radius: 30px;");
    ui->StepPopup->hide();
    ui->TeachPopup->setStyleSheet("background-color: rgb(249, 233, 216); border-radius: 30px;");

    ui->TeachPopup->hide();

    ui->symptomsList->setStyleSheet("background-color: rgb(249, 233, 216); border-radius: 30px;");

    //Instructions
    QString instructions = QString::fromStdString("Drag the correct treatment to the patient to heal them!");
    QLabel *instructionsLabel = new QLabel(instructions);

    instructionsLayout = new QVBoxLayout();

    instructionsLabel->setWordWrap(true);
    instructionsLabel->setStyleSheet("margin: 0px 25px 0px 25px; font-size: 20px; color: rgb(140, 111, 77); font-weight: 700;");
    instructionsLayout->addWidget(instructionsLabel);

    ui->instructionsWidget->setLayout(instructionsLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
* Creates a new treatment by creating a MySquare object that holds the image that represents this treatment.
*
* @brief MainWindow::createTreatment
* @param name - the name of the treatment object to be created
* @param imageLoc - the location of where the image you want to be show for the object is
* @param xLoc - the x coordinate of the inital position of the new treatment object
* @param yLoc - the y coordinate of the inital position of the new treatment object
*/
void MainWindow::createTreatment(std::string name , std::string imageLoc, int xLoc, int yLoc, int xDim, int yDim)
{
   //create a 'MySquare' object to represent a QGraphics object that can hold an image
   MySquare *newTreatment = new MySquare(imageLoc, name, xLoc, yLoc, xDim, yDim);

   //set the inital position of the treatment
   newTreatment->setPos(xLoc, yLoc);

   //create a pair that maps the name of the treatment to the graphic object that represents it
   std::pair <std::string,MySquare *> mySquarePair (name,newTreatment);

   //add the pair to the map in the model
   mainModel->treatments.insert(mySquarePair);

   //add the new graphics object to the scene
   scene->addItem(newTreatment);
}



/**
 * Slot for update world.
 * @brief updateWorld
 * @param height
 */
void MainWindow::updateWorld(){

   // Instruct the world to perform a single step of simulation.
   // It is generally best to keep the time step and iterations fixed.
   world.Step(timeStep, velocityIterations, positionIterations);

   //the body is moving towards the ground, i.e it having a negative velocity, indicates we need to update an objects position
   if(body->GetLinearVelocity().y < 0 ){
       //first we check if there is a current treatment, if there is then update it
       if(mainModel->currentTreatment.second != "empty"){
           //get the b2Body's position
           b2Vec2 position = body->GetPosition();

           //updating the QGraphicsItem that is selected to have the b2Body's position
           mainModel->treatments.at(mainModel->currentTreatment.second)->setPos(position.x*75, -position.y*75);

        //next we check if there are any hints available to drop
       }else if(mainModel->hints.size() != 0){
           b2Vec2 position = body->GetPosition();

           //updating the QGraphicsItem to have the bodies' properties
           mainModel->treatments.at(mainModel->hints.back())->setPos(position.x*75, -position.y*75);
       }
   }
   else{
       //reset the b2Body to have zero velocity
       body->SetLinearVelocity(b2Vec2(0,0));

       //reset the current treatment to empty
       mainModel->currentTreatment.second = "empty";

       //check to see if there are remaining hints left to fall off the shelf
       if(mainModel->hints.size() >0){
           //set the field in hasDropped to true so that it cannot be selected as a hint again
           mainModel->treatments.at(mainModel->hints.back())->hasDropped = true;
           //start by removing a hint because if there was one present, it has already been updated
           mainModel->hints.pop_back();
           //if there are still hints remaining, drop start to drop them
           if(mainModel->hints.size() >0){
               int hintXLoc = mainModel->treatments.at(mainModel->hints.back())->initialXLoc;
               int hintYLoc = mainModel->treatments.at(mainModel->hints.back())->initialYLoc;
               startDroppingTreatment(hintXLoc, hintYLoc);
           }
       }
   }
}


/**
 * Slot for receiving a new height value.  Resets body heights to height.
 * @brief MainWindow::receiveNewHeightValue
 * @param height
 */
void MainWindow::receiveNewHeightValue(float x, float height, std::string name)
{
    //set the box2D body to assoiciate with the 'treatment' that was clicked on
    mainModel->currentTreatment.second = name;
    //start dropping the b2Body
    startDroppingTreatment(x, height);
}

/**
 * Slot for scene resize events.  Connects to MySquare notifySceneToResize signal.
 * @brief MainWindow::callSceneResize
 */
void MainWindow::callSceneResize()
{
    setSceneSize();
}

/**
 * Function for activating 'gravity' on the b2Body such that it starts falling.
 *
 * @brief MainWindow::startDroppingTreatment
 * @param x - the x coordinate of where you want the b2Body  to drop from
 * @param height - the height of where you want the b2Body  to drop from
 */
void MainWindow::startDroppingTreatment(float x, float height)
{
    b2Vec2 newPos(x/75, -height/75);
    // the real gravity set in startup is never used so im doing this for now
    b2Vec2 fakeGravity(0.0f, -5.0f);

    body->SetTransform(newPos, body->GetAngle());
    body->SetLinearVelocity(fakeGravity);
}


/**
 * Resets the size of the GraphicView's scene.  Used to repaint the scene so that treatments overlapping the patient don't paint over the patient.
 * @brief MainWindow::resetSceneSize
 */
void MainWindow::setSceneSize()
{
    if (expandedX % 2 == 0){
        expandedX ++;
    }
    else{
        expandedX --;
    }
    QPoint sizeConstraint1 = QPoint(-500,500);
    QPoint sizeConstraint2 = QPoint(expandedX, -500);
    QRectF sceneSize = QRectF(sizeConstraint1, sizeConstraint2);
    scene->setSceneRect(sceneSize);
}


void MainWindow::on_hintButton_clicked()
{
    mainModel->showHint();
}


void MainWindow::on_nextButton_clicked()
{
    ui->TeachPopup->hide();
    clearLayout(ui->TeachPopup->layout());
    delete ui->TeachPopup->layout();
    clearLayout(ui->StepPopup->layout());
    delete ui->StepPopup->layout();
    clearLayout(ui->symptomsList->layout());
    delete ui->symptomsList->layout();
    mainModel->loadNextLevel();
}


void MainWindow::loadLevelUI(){
    Level *level = mainModel->currentLevel;
    stepsPopLayout = new QVBoxLayout();
    QLabel *stitle = new QLabel();
    stitle->setText("Steps to Solve: " + QString::fromStdString(level->title));
    stitle->setStyleSheet("font-size: 25px; color: rgb(140, 111, 77); font-weight: 700;");
    QGridLayout *gl = new QGridLayout();
    for (int i = 0; i < level->stepsImages.size(); i++)
    {
        QLabel *label = new QLabel();
        label->setFixedHeight(80);
        label->setFixedWidth(80);
        QString s = QString::fromStdString(level->stepsImages[i].second);
        QPixmap pixmap(s);
        pixmap = pixmap.scaled(label->size(),Qt::KeepAspectRatio);
        label->setPixmap(pixmap);
        gl->addWidget(label, i, 0, Qt::AlignCenter);

        QLabel *tLabel = new QLabel();
        tLabel->setStyleSheet("font-size: 15px; color: rgb(168, 139, 106); font-weight: 600;");
        tLabel->setWordWrap(true);
        tLabel->setText(QString::fromStdString(level->stepsText[i]));
        gl->addWidget(tLabel, i, 1);
    }

    mainModel->levelPassed->setStyleSheet("font-size: 15px; color: rgb(168, 139, 106); font-weight: 600;");
    stepsPopLayout->addWidget(mainModel->levelPassed);
    stepsPopLayout->addWidget(stitle);
    stepsPopLayout->setAlignment(Qt::AlignCenter);
    gl->setAlignment(Qt::AlignCenter);
    gl->setVerticalSpacing(20);
    gl->setHorizontalSpacing(10);
    stepsPopLayout->addLayout(gl);

    QPushButton *nextBtn = new QPushButton();
    nextBtn->setText("NEXT");
    nextBtn->setStyleSheet("margin-left: 130px; background-color: green; color: white; font-size: 20px; font-weight: 700; border: none; border-radius: 10px;");
    nextBtn->setFixedWidth(300);
    nextBtn->setFixedHeight(50);
    stepsPopLayout->addWidget(nextBtn);
    stepsPopLayout->setSpacing(20);

    connect(nextBtn, SIGNAL(clicked()), this, SLOT(toTeach()));
    ui->StepPopup->setLayout(stepsPopLayout);


    // TEACH LAYOUT
    teachPopLayout = new QVBoxLayout();
    for (int i = 0; i < level->teachText.size(); i++)
    {
        QString str = QString::fromStdString(level->teachText[i]);
        QStringList list1 = str.split(QLatin1Char(' '));
        if(list1.count() <= 5)
        {
            QLabel *t = new QLabel(str);
            t->setStyleSheet("margin: 0px 50px 0px 50px; font-size: 25px; color: rgb(140, 111, 77); font-weight: 700;");
            teachPopLayout->addWidget(t);
        }
        else
        {
            QLabel *l = new QLabel(str);
            l->setWordWrap(true);
            l->setStyleSheet("margin: 0px 50px 0px 50px; font-size: 15px; color: rgb(168, 139, 106); font-weight: 600;");
            teachPopLayout->addWidget(l);
        }
    }

    QHBoxLayout *hl = new QHBoxLayout();
    for (int i = 0; i < level->teachImages.size(); i++)
    {
        QLabel *label = new QLabel();
        label->setFixedHeight(70);
        label->setFixedWidth(70);
        QString s = QString::fromStdString(level->teachImages[i].second);
        QPixmap pixmap(s);
        pixmap = pixmap.scaled(label->size(),Qt::KeepAspectRatio);
        label->setPixmap(pixmap);
        hl->addWidget(label, Qt::AlignCenter);
    }

    teachPopLayout->addLayout(hl);

    QPushButton *nextLBtn = new QPushButton();
    nextLBtn->setText("NEXT LEVEL");
    nextLBtn->setFixedWidth(150);
    nextLBtn->setFixedHeight(50);
    nextLBtn->setStyleSheet("background-color: green; color: white; font-size: 20px; font-weight: 700; border: none; border-radius: 10px;");
    teachPopLayout->addWidget(nextLBtn);
    teachPopLayout->setAlignment(nextLBtn, Qt::AlignHCenter);

    connect(nextLBtn,
            &QPushButton::clicked,
            this,
            &MainWindow::on_nextButton_clicked);

    ui->TeachPopup->setLayout(teachPopLayout);
    // NEXT LEVEL BUTTON CONNECT


    symptomsLayout = new QVBoxLayout();

    QString symptomsTitle = QString::fromStdString(level->title + "\n" + "Symptoms");
    QLabel *symptomsLabelTitle = new QLabel(symptomsTitle);
    symptomsLabelTitle->setWordWrap(true);
    symptomsLabelTitle->setStyleSheet("margin: 0px 25px 0px 25px; font-size: 20px; color: rgb(140, 111, 77); font-weight: 700;");
    symptomsLayout->addWidget(symptomsLabelTitle);

    for (string symptom : level->symptomsList){
        cout << symptom << endl;
        QString str = QString::fromStdString(symptom);
        QStringList list1 = str.split(QLatin1Char(' '));

        QLabel *symptomsLabel = new QLabel(str);
        symptomsLabel->setWordWrap(true);
        symptomsLabel->setStyleSheet("margin: 0px 25px 0px 25px; font-size: 14px; color: rgb(140, 111, 77); font-weight: 700;");
        symptomsLayout->addWidget(symptomsLabel);
    }

    ui->symptomsList->setLayout(symptomsLayout);

}

void MainWindow::toTeach(){
    ui->StepPopup->hide();
    ui->TeachPopup->show();
}

void MainWindow::showPopup(){
    ui->StepPopup->show();
}

void MainWindow::clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget()){
                if (widget != mainModel->levelPassed)
                    widget->deleteLater();
            }
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}
